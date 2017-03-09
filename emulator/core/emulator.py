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

#
# This is an interface class for the emulator that is written in C.
#

class Emulator:
    def __init__(self):
        self.firmware_build = "ga_stub"
        self.launch_emu_cmd = "emu_base_model/obj-*/emu_base_model.elf emu_base_model/*.ihex"

    def start(self):
        self.emu_proc = Popen([self.launch_emu_cmd], shell=True)

        print("started emulator process on pid: " + str(self.emu_proc.pid))

    def stop(self):
        self.emu_proc.kill()

    # Upload to the serial port of the device
    def upload(self, build_path, build_filename):
        upload_cmd = "cd " + str(build_path) + " && avrdude -p m328p -c arduino -P /tmp/simavr-uart0 -U flash:w:" + build_filename

        # Use the 'call' command here to block execution until we want to
        # start reading from the virtual serial port
        while( not os.path.exists("emu_online.txt") ):
            time.sleep(0.01)

        call([upload_cmd], shell=True)

    # Upload directly into the MCU memory;
    # Not implemented yet.
    def upload_to_memory():
        pass


class TestEmulator:
    def __init__(self):
        pass

    def start(self):
        emulator = Emulator()
        emulator.start()

        time.sleep(2)

        firmware_build = "ga_stub"
        build_path = "../../.pioenvs/" + firmware_build
        emulator.upload(build_path)

        time.sleep(2)
        emulator.stop()
        pass

if __name__ == "__main__":
    test_emulator = TestEmulator()
    test_emulator.start()
