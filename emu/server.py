from flask import Flask
from flask import request
from flask import render_template
from multiprocessing import Process
from subprocess import call
from subprocess import Popen
app = Flask(__name__)
import subprocess
import os
import signal

prev_count = 0;
current_count = 0;
max_length = 0;
emu_state = "STOPPED"

emu_proc = 0;


@app.route("/")
def index():
    return render_template('index.html')

@app.route("/stdout/length")
def file_length():
    length = Popen("wc -l < emu_output.txt", shell=True, stdout=subprocess.PIPE).stdout.read()
    return length

# This route grabs new data that is written to the logfile
@app.route("/stdout")
def tail():
    tail_ptr = request.args.get("tail_ptr")
    output = Popen("tail emu_output.txt -n " + str(tail_ptr), shell=True, stdout=subprocess.PIPE).stdout.read()
    return output

@app.route("/control/state")
def state():
    global emu_state

    return emu_state

@app.route("/control/start")
def start():
    global emu_proc
    global emu_state
    firmware_build = request.args.get("firmware_build")

    cmd = "python2 run_emu.py {fw}".format(fw=firmware_build)
    emu_proc = Popen([cmd], shell=True)

    emu_state = "RUNNING"
    return "start"

@app.route("/control/stop")
def stop():
    global emu_proc
    global emu_state

    print "terminating emulator proc"
    emu_proc.terminate()

    emu_state = "STOPPED"
    return "stop"

@app.route("/control/clear")
def clear_txt():
    call(["cat /dev/null > emu_output.txt"], shell=True)

    return "clear"


app.run(debug=True)
