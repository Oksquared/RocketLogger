/*
This program dumps the SD card file contents over the serial port. It waits for a serial connection to dump the data.
I would recommend using Coolterm (link below) for writing the contents to a local file on your computer.
!
THE FILE NAME MUST BE THE SAME AS WHAT WAS WRITTEN!!!!!!
!
http://freeware.the-meiers.org

  This example shows how to read a file from the SD card using the
  SD library and send it over the serial port.

  The circuit:
   SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4 (for MKRZero SD: SDCARD_SS_PIN)

  created  22 December 2010
  by Limor Fried
  modified 9 Apr 2012
  by Tom Igoe

  This example code is in the public domain.
  code written by Owen Krumm 2023 for a Adafruit Featherwing M0 with a Adafruit LSM6DSOX + LIS3MDL FeatherWing - Precision 9-DoF IMU.
*/

#include <SPI.h>
#include <SD.h>

const int chipSelect = A5;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("data4.txt");

  // if the file is available, write to it:
  if (dataFile) {
    while (dataFile.available()) {
      Serial.write(dataFile.read());
    }
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening data.txt");
  }
}

void loop() {
}

