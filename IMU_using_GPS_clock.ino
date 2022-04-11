#include <DFRobot_BMX160.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_GPS.h>

#define TIME_24_HOUR      true //24-timers klokke
#define HOUR_OFFSET       -9    // Bruk 2 for CEST
#define DISPLAY_ADDRESS   0x70

SoftwareSerial gpsSerial(8, 7);  
Adafruit_GPS gps(&gpsSerial);
DFRobot_BMX160 bmx160;

void setup(){
  while (!Serial);
  Serial.begin(115200);
  
  gps.begin(9600);
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  enableGPSInterrupt();
  
  if (bmx160.begin() != true){
    Serial.println("init false");
    while(1);
  }
}

void loop(){

   if (gps.newNMEAreceived()) {
    gps.parse(gps.lastNMEA());
  }

  int hours = gps.hour + HOUR_OFFSET;  
  int day = gps.day;
  int month = gps.month;
  int year = gps.year;
  
  if (hours < 0) {
    hours = 24+hours;
  }
  if (hours > 23) {
    hours = 24-hours;
  }
  
  int minutes = gps.minute;
  int seconds = gps.seconds;
  int displayValue = hours*100 + minutes;

  if (!TIME_24_HOUR) {
    if (hours > 12) {
      displayValue -= 1200;
    }
    else if (hours == 0) {
      displayValue += 1200;
    }
  }
  
  
  sBmx160SensorData_t Omagn, Ogyro, Oaccel;

  bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);

  printTimeandDate(displayValue, hours, minutes, seconds, day, month, year);
  Serial.print("M ");
  Serial.print("X: "); Serial.print(Omagn.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Omagn.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Omagn.z); Serial.print("  ");
  Serial.println("uT");

  printTimeandDate(displayValue, hours, minutes, seconds, day, month, year);
  Serial.print("G ");
  Serial.print("X: "); Serial.print(Ogyro.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Ogyro.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Ogyro.z); Serial.print("  ");
  Serial.println("g");

  printTimeandDate(displayValue, hours, minutes, seconds, day, month, year);
  Serial.print("A ");
  Serial.print("X: "); Serial.print(Oaccel.x    ); Serial.print("  ");
  Serial.print("Y: "); Serial.print(Oaccel.y    ); Serial.print("  ");
  Serial.print("Z: "); Serial.print(Oaccel.z    ); Serial.print("  ");
  Serial.println("m/s^2");
}

SIGNAL(TIMER0_COMPA_vect) {
  gps.read();
}

void enableGPSInterrupt() {
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

void printTimeandDate(int displayValue, int hours, int minutes, int seconds, int day, int month, int year) {
  Serial.print("TIME: ");
  if (TIME_24_HOUR && 0 < hours < 10) {
    Serial.print("0");
    if (TIME_24_HOUR && hours == 0) {
      Serial.print("0");
    }
  }
  Serial.print(displayValue, DEC);
  if(seconds < 10){
    Serial.print("0");
  }
  Serial.print(seconds, DEC);
  Serial.print(" | DATE: ");
  if(day < 10){
    Serial.print("0");
  }
  Serial.print(day, DEC);
  if(month < 10){
    Serial.print("0");
  }
  Serial.print(month, DEC);
  Serial.print(year, DEC);
  Serial.print(" | ");

  
}
