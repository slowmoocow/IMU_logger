import serial
from datetime import datetime

if __name__ == '__main__':
  ser = serial.Serial('/dev/ttyACM0', 57600, timeout = 1)
  ser.flush()
  
  while True:
    if ser.in_waiting > 0:
      data = ser.readline().decode('utf-8).rstrip()
      
      print(data)
      file = open("log.csv", "a")
      file.write(datetime.today().strftime('%Y-%m-%d'+", "'%H:%M:%S:%f')[:-3])
      file.write(" " + data + "\n")
