/*
  Code for testing writing and reading
  data from SD card via SD card module.

  See ArduinoTempReader for more details.
*/
#include <SPI.h>
#include <SD.h>

const int SDCardPin = 7;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  Serial.print("Initializing SD card...");
  if (!SD.begin(SDCardPin)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  File myFile = SD.open("test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println("test");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening test.txt");
  }
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt: ");
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}

void loop() {}

