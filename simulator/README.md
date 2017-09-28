This is a small simavr-based emulator that allows you to quickly test
firmware builds for the weatherbox firmware.

TODO:

* biggest goal: having an EASY framework to script tests with
* Cleanup some of the scripts .. we still don't have proper classes
    for everything
* Integrate attached devices that can be simulated; for example, if we tune the
    battery voltage what happens?
* use SWIG?


## Running the GUI

* Open up a terminal in the emu/ folder and run `python server.py`
* Go to localhost:5000 on your browser
* To exit, go to the terminal you created and hit Ctrl+C (you may need to do this multiple times)


## To setup the emulator

    make init

This will download v1.3 of simavr, build it as well as the cooresponding base model.
Make sure you have the following dependencies installed:

* avr-libc
* libelf-dev
* freeglut3-dev
* pkg-config

## To the run emulator and load a specific build**

    python run_emu.py ga_stub   # you can specify any other build here

The python script will automatically collect serial output from the device that is
currently under emulation. More interactivity (and possibly a UI) will be added in a later
patch.

Note: you may have to use the `python2` command instead, depending on what your
system expects

