# Arduino - Python temperature monitor

Version: 1.0

This is an Arduino and Python project/setup for monitoring temperature.
Idea is to monitor temperature transition during transport in real time and record
data to a persistent memory (SD card).

Arduino Nano is used with DS18S20 temperature cable sensor.
DS1302 is the real time module used and an SC card module. One 4.7k resistor
is needed. See doc/sketch_final.png for details.

External Arduino libraries used are OneWire and DS1302.

Python part includes a script for plotting the temperature data on demand.
Script is designed to be transported within the SD card like a standalone
executable (created by using py2exe library). It creates a temperature
transition plot as a .png image also within the SD card. Script uses
matplotlib library.

## Usage

<img src="/doc/sketch_final.png" height="400" width="500" align="center" alt="Arduino schema" />

- Connect Arduino and the required modules like defined in the picture.
Use `ArduinoRealTimeClockModuleConfigurator` to setup the current time
on the real time module. After that upload the `ArduinoTempReader` script.
In the script delay between temperature measuring can be adjusted (currently
it is set to 120000 millisecond). Also set the `debug` field to `true` if
serial output is needed.

- On Python environment in the location of the script run `python setup.py py2exe`
and copy the whole `dist` folder to the SC card.

- To record temperature data just plug the Arduino to the power supply and it will
start to record. Temperature will be recorded on SC card in `TEMP.TXT` file.

- When recording is done, load SD card on the presenting environment and run `dist/main.exe`.
The temperature plot will be saved in the file on SD card. File name will be in the format
temp_&lt;last time recorded&gt;.png.

**TODO** - Implement independent power supply for Arduino and a proper case.

## License

Distributed under GNU GENERAL PUBLIC LICENSE Version 2.

Copyright © Dejan Josifovic, theparanoidtimes 2016.
