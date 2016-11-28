from flask import Flask
from flask import request
from flask import render_template
from flask_socketio import SocketIO

from multiprocessing import Process
from subprocess import call
from subprocess import Popen
import subprocess
import os
import signal
import time
import eventlet

from threading import Thread

app = Flask(__name__)
app.config['SECRET_KEY'] = 'secret!'
socketio = SocketIO(app)


prev_count = 0;
current_count = 0;
max_length = 0;
emu_state = "STOPPED"

emu_proc = 0;


#
# Normal WSGI routes
#

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


    # For some reason on debian, running the Popen with shell=True
    # spawns a sh/ process that then runs the given command. It doesn't do
    # this in Arch. So when we attempt to terminate the emulator model process
    # doesn't terminate properly.
    #
    # So running exec (which will replace the current running process) will
    # maintain compatability with debian.
    cmd = "exec python2 run_emu.py {fw}".format(fw=firmware_build)
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


#
# Websocket Interface
#


@socketio.on('connect')
def handle_connect():
    socketio.send("Hello, client!")

def background_emit():
    global socketio
    global app

    eventlet.sleep(1)
    while True:
        socketio.emit("message", {'data': 42}, broadcast=True)
        print "Emitting message"
        eventlet.sleep(1)

eventlet.spawn(background_emit)
socketio.run(app)

