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

**TBD**

## License

Distributed under GNU GENERAL PUBLIC LICENSE Version 2.

Copyright © Dejan Josifovic, theparanoidtimes 2016.
