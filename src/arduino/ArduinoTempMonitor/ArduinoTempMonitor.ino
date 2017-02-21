/*
  Complete temperature monitor code.
  Reads data from DS18S20 temperature sensor and from
  DS1302 real time module and writes it to a file on
  SD card via SD card module.
  All pins are digital, no analog pins are used.

  Phase 2 configuration:

  1. SD card module should be attached as follows:
  - MOSI - pin 11 ------\
  - MISO - pin 12--------> Standard Arduino SPI interface - must not be changed.
  - SCK (CLK) - pin 13--/
  - CS - pin 7 - Data, this is configurable (SDCardPin).
  - VCC - 5V power.
  - GND - Ground.

  2. Temperature sensor (DS18S20) attach as follows:
  - Red wire - 5V power (VCC).
  - Blue/gray wire - Ground (GND).
  - Yellow wire - pin 8 - Data, this is configurable (DS18S20_Pin).
  One 4.7k resistor goes between data and power wires.
  See: http://bildr.org/2011/07/ds18b20-arduino/

  3. Real time module (DS1302) attach as follows:
  - Chip Enable (RST) - pin 2 - (kCePin)
  - Input/Output (DAT) - pin 3 - (kIoPin)
  - Serial Clock (CLK) - pin 4 - (kSclkPin)
  - VCC - 5V power.
  - GND - Ground.

  See doc folder for more details.

  -----------------------------------------------------------------------------

  Phase 1 configuration:
  1.MOSI - pin 11
    MISO - pin 12
    SCK (CLK) - pin 13
    CS - pin 4
    VCC - 5V power.
    GND - Ground.
  2.Red wire - 5V power (VCC).
    Blue/gray wire - Ground (GND).
    Yellow wire - pin 2
  3.Chip Enable (RST) - pin 8
    Input/Output (DAT) - pin 9
    Serial Clock (CLK) - pin 10
    VCC - 5V power.
    GND - Ground.
*/
#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <stdio.h>
#include <DS1302.h>

bool debug = true;
String fileName = "Temp.txt";
long readDelay = 10000;
String delimeter = ";";

int cycle = 0;

const int DS18S20_Pin = 8;
const int SDCardPin = 7;
const int kCePin = 2;
const int kIoPin = 3;
const int kSclkPin = 4;

OneWire ds(DS18S20_Pin); // Temp reader object.
DS1302 rtc(kCePin, kIoPin, kSclkPin); // Real time clock reader object.

void setup() {
  debugSerialWrite("Starting initialization...");
  if (debug) {
    Serial.begin(9600);
    while (!Serial) {
      ;
    }
  }

  debugSerialWrite("Initializing real time module...");
  debugSerialWrite("Current time: " + getTime());

  debugSerialWrite("Initializing temperature sensor...");
  String tmp = "Current temperature: ";
  tmp += getTemp();
  debugSerialWrite(tmp);

  debugSerialWrite("Initializing SD card...");
  if (!SD.begin(SDCardPin)) {
    debugSerialWrite("SD initialization failed!");
    return;
  }
  debugSerialWrite("SD initialization done.");

  debugSerialWrite("Initialization finished.");
}

void loop() {
  String textHolder = "";
  File myFile = SD.open(fileName, FILE_WRITE);
  if (myFile) {
    debugSerialWrite("Writing to test.txt...");
    textHolder += cycle;
    textHolder += delimeter;
    textHolder += getTime();
    textHolder += delimeter;
    textHolder += getTemp();
    debugSerialWrite(textHolder);
    myFile.println(textHolder);
    myFile.close();
    debugSerialWrite("done.");
  } else {
    debugSerialWrite("Error opening test.txt!");
  }

  cycle++;
  delay(readDelay);
}

float getTemp() {
  // Returns the temperature from one DS18S20 in DEG Celsius.

  byte data[12];
  byte addr[8];

  if ( !ds.search(addr)) {
    // No more sensors on chain, reset search.
    ds.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
    debugSerialWrite("CRC is not valid!");
    return -1000;
  }

  if ( addr[0] != 0x10 && addr[0] != 0x28) {
    debugSerialWrite("Device is not recognized!");
    return -1000;
  }

  ds.reset();
  ds.select(addr);
  ds.write(0x44, 1); // Start conversion, with parasite power on at the end.

  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad...

  for (int i = 0; i < 9; i++) { // We need 9 bytes.
    data[i] = ds.read();
  }

  ds.reset_search();

  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); // Using two's compliment...
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
}

String getTime() {
  // Get the current time and date from the chip.
  Time t = rtc.time();

  // Format the time and date and insert into the buffer.
  char buf[50];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);
  return String(buf);
}

void debugSerialWrite(String text) {
  if (debug) {
    Serial.println(text);
  }
}

