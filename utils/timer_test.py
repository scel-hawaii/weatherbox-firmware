import serial
import sys
import datetime
import time

# /dev/cu.usbmodem1421
ser = serial.Serial(port='/dev/cu.usbmodem1421', baudrate=9600)
print("connected to: " + ser.portstr)
count=1

while True:
    line = ser.readline()
    time = str(datetime.datetime.now())
    print line
    sys.stdout.write(time)
    sys.stdout.write('  ')
