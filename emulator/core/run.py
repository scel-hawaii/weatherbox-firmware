#!/usr/bin/python
from subprocess import call
from subprocess import Popen
import time
import serial
import struct
import sys
import os
import datetime
import signal
from emulator import Emulator

#
# Check arguments and files
#

if len(sys.argv) < 2:
    sys.exit("Error, not build specified")

firmware_build = sys.argv[1]
build_dir = "../../.pioenvs/" + firmware_build

if os.path.isdir(build_dir) == False:
    sys.exit("Error, firmware build does not exist: " + firmware_build)

if os.path.isfile(build_dir + "/firmware.hex") == False:
    sys.exit("Error, firmware file does not exist. Maybe try compiling? " + firmware_build)

if( not os.path.exists("emu_output.txt")):
    print("emu_output.txt does not exist")
    call(["touch emu_output.txt"], shell=True)

if( os.path.exists("emu_online.txt")):
    call(["rm emu_online.txt"], shell=True)

#
# Main execution
#

emulator = Emulator()
emulator.start()
emulator.upload(build_dir, "firmware.hex")

ser = serial.Serial('/tmp/simavr-uart0', 9600)

# Define a term handler here or else the emu_proc won't get
# killed properly when SIGTERM gets called in the server
"""
def signal_term_handler(signal, frame):
    global emu_proc
    emu_proc.terminate()
    # print 'got SIGTERM'
    sys.exit(0)

signal.signal(signal.SIGTERM, signal_term_handler)
"""

while True:
    result = ser.read()

    # Note: we need to specify utf-8 because python3 handles strings
    # differently: http://stackoverflow.com/questions/21689365/python-3-typeerror-must-be-str-not-bytes-with-sys-stdout-write
    sys.stdout.write( result.decode('utf-8') )
