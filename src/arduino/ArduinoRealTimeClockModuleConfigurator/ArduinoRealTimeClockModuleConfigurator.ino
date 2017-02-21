/*
  Code for setting the time on DS1302 module.

  See ArduinoTempReader to more details.
*/
#include <SPI.h>
#include <SD.h>
#include <stdio.h>
#include <DS1302.h>

const int kCePin   = 2;
const int kIoPin   = 3;
const int kSclkPin = 4;

int yearToSet = 2017;
int monthToSet = 2;
int dayToSet = 19;
int hourToSet = 0;
int minuteToSet = 0;
int secondToSet = 0;
Time::Day dayNameToSet = Time::kThursday;

DS1302 rtc(kCePin, kIoPin, kSclkPin);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  rtc.writeProtect(false);
  rtc.halt(false);
  Time t(yearToSet, monthToSet, dayToSet,
         hourToSet, minuteToSet, secondToSet, dayNameToSet);
  rtc.time(t);
}

void loop() {
  Serial.println(getTime());
  delay(1000);
}

String getTime() {
  Time t = rtc.time();
  char buf[50];
  snprintf(buf, sizeof(buf), "%04d-%02d-%02d %02d:%02d:%02d",
           t.yr, t.mon, t.date,
           t.hr, t.min, t.sec);
  return String(buf);
}

