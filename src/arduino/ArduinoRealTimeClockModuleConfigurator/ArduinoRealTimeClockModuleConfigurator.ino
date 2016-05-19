#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include <DS1302.h>

const int kCePin   = 8;
const int kIoPin   = 9;
const int kSclkPin = 10;

DS1302 rtc(kCePin, kIoPin, kSclkPin); // Real time clock reader object.

void setup() {
  Serial.begin(9600);
  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(2016, 5, 20, 0, 0, 0, Time::kThursday);
  rtc.time(t);
}

void loop() {
  // Not used...
}

