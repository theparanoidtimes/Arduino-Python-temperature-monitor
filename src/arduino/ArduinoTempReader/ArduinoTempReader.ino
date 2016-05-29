/*
  SD card attached to SPI bus as follows:
  - MOSI - pin 11
  - MISO - pin 12
  - CLK - pin 13
  - CS - pin 4

  Temperature sensor DS18S20 attach as follows:
  - Red wire - 5V vnc
  - Blue wire - GRD
  - Yellow wire - Data - pin 2
  See: http://bildr.org/2011/07/ds18b20-arduino/

  Real time module attach as follows:
  - Chip Enable - pin 8
  - Input/Output - pin 9
  - Serial Clock - pin 10
*/
#include <SPI.h>
#include <SD.h>
#include <OneWire.h>
#include <stdio.h>
#include <DS1302.h>

bool debug = false;
String fileName = "Temp.txt";
long readDelay = 120000;
String delimeter = ";";

int cycle = 0;

const int DS18S20_Pin = 2;
const int SDCardPin = 4;
const int kCePin   = 8;
const int kIoPin   = 9;
const int kSclkPin = 10;

File myFile;
OneWire ds(DS18S20_Pin); // Temp reader object.
DS1302 rtc(kCePin, kIoPin, kSclkPin); // Real time clock reader object.

void setup() {
  debugSerialWrite("Starting initialization...");
  Serial.begin(9600);

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

  myFile = SD.open(fileName, FILE_WRITE);
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

  byte present = ds.reset();
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

