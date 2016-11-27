#!/usr/bin/python
from subprocess import call
from subprocess import Popen
import time
import serial
import struct
import sys
import os
import datetime

#
# Check arguments and files
#

if len(sys.argv) < 2:
    sys.exit("Error, not build specified")

firmware_build = sys.argv[1]
build_dir = "../.pioenvs/" + firmware_build

if os.path.isdir(build_dir) == False:
    sys.exit("Error, firmware build does not exist: " + firmware_build)

if os.path.isfile(build_dir + "/firmware.hex") == False:
    sys.exit("Error, firmware file does not exist. Maybe try compiling? " + firmware_build)

#
# Main execution
#

print "Running emulator with build: " + firmware_build

launch_emu_cmd = "emu_base_model/obj-*/emu_base_model.elf emu_base_model/*.ihex"
emu_proc = Popen([launch_emu_cmd], shell=True)

# Sleep to give some time for the emulator model to come online
time.sleep(2)

print "Running upload command"

upload_cmd = "cd " + build_dir + " && avrdude -p m328p -c arduino -P /tmp/simavr-uart0 -U flash:w:firmware.hex"
# Use the 'call' command here to block execution until we want to
# start reading from the virtual serial port
upload_proc = call([upload_cmd], shell=True)

ser = serial.Serial('/tmp/simavr-uart0', 9600)

while True:
    result = ser.read()
    sys.stdout.write(result)
