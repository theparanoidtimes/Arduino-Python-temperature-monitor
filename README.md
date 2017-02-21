# Arduino - Python temperature monitor

Version: 2.0

This project represents the hardware and software setup for a portable
temperature monitor. It's job is to record temperature in real time to a
persistent medium and provide a visual interpretation of the collected data.

## Usage 

The initial idea is to use the monitor to record temperature changes during
cargo transport by refrigerator car/truck. Of course it can be used in any other
suitable situation.

By turning on the power source monitor will start to collect temperature data
and store it in a format *cycle;timestamp;measured-temperature* in file
(called *TEMP.TXT*) on SD card. Cycle is a number that is incremented each time
the temperature data is read.

On that same card an executable file is located that will, when run, parse the
collected data and provide a visual representation as a graph stored in a
separate file.

## Project organization

Project is made of two parts - Arduino related part which has both code and
hardware schema and Python part that has the code related to data visualization.

### Arduino

Arduino code and setup is responsible for reading and storing temperature data.

Modules currently used are DS18S20 temperature cable sensor, DS1302 real time
module and regular SD card module. Also one 4.7k resistor is needed for
temperature sensor. For assembly details see
[Development / assembly](#development-/-assembly).

The main code is located in *ArduinoTempMonitor* sketch. It uses a very simple
approach - while looping it reads temperature data, reads current time and
writes the collected data to a file on SD card in CSV format. Everything
is 'configurable' (pin configuration, output file name, file data separator,
read delay, etc.) in the beginning of the file so it is more maintainable. The
debug flag indicates if Arduino should communicate to *Serial* bus or not.

Communication with the temperature sensor is done via
[OneWire](https://github.com/PaulStoffregen/OneWire) library and the
communication with real time module is done via
[DS1302](https://github.com/msparks/arduino-ds1302). Both libraries are stored
in *lib/* directory.

To set the current time on real time module use
*ArduinoRealTimeClockModuleConfigurator* sketch. Before loading current time
values need to be adjusted in the beginning of the file.

Other sketches that are used for testing are also present:
*ArduinoReadTemperatureTest* for testing the communication with temperature
sensor and *ArduinoSDCardWriteReadTest* for testing the communication with SD
card module.

### Python

Python code is used for visualizing data as a graph of temperature transition
over time for which it uses [matplotlib](http://matplotlib.org/) library. The
script implies that the data file is located one level above and is named
*TEMP.TXT*, however this can be changed if needed. All the code is in `main.py`
file.

`setup.py` is provided to be used with [py2exe](http://www.py2exe.org/) to
create a standalone executable of this script. This executable should be stored
in the same SC card where the temperature data will be written. When the data
is collected executable can be run to provide the graph. See the *doc/*
directory for a sample graph.

Since *py2exe* is used this only works on Windows and currently system on which
this is supposed to be used must have *Visual C++ 2008 redistributable*. See
py2exe [site](http://www.py2exe.org/) for more information.

Python version with which this works is 2.7.13, py2exe 0.6.9.win32-py2.7 and
matplotlib is 2.0.0.

## Development / assembly

Here is the chronological overview of the Arduino hardware part of the
development.

### Phase 1

The first phase was a proof-of-concept build. Both Arduino Uno and Arduino Nano
were used in this phase. All modules were connected via Arduino breadboard. See
*doc/* directory for layout schema and snapshots. Pin configuration is
documented in *ArduinoTempMonitor* sketch.

### Phase 2

Current phase. In this phase all modules all soldered on the board along with
Arduino itself and are connected with copper isolated wires. Arduino Nano only
is used. Configuration is a bit different from the Phase 1 as it is shown on the
schema bellow.

![Phase 2 schema](/doc/layout_sketch_phase_2_final.png "Phase 2 schema")

All connections are straightforward, the only 'complicated' part is with the
temperature sensor. It requires a 4,7k pull-up resistor to be attached between
the data and power wires so it can provide correct temperature data. More
details [here](http://bildr.org/2011/07/ds18b20-arduino/).
Power wires are soldered together and conveyed to Arduino 5V power pin. Same
thing is done with ground wires.

See *doc/* directory for some snapshots. Pin configuration is documented in
*ArduinoTempMonitor* sketch.

### Phase 3

*TODO* for phase 3:
* Mobile power supply - battery;
* On/off switch;
* Detachable temperature sensor;
* Proper case;
* etc.

## License

Distributed under GNU GENERAL PUBLIC LICENSE Version 2.

Copyright © Dejan Josifović, theparanoidtimes 2016.
