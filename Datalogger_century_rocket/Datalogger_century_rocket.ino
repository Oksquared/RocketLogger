#include <Adafruit_NeoPixel.h>

/*
  SD card datalogger for Century College rocket IMU datalogger
  This program logs the IMU data every (writeFrequency) miliseconds to the Adafruit SPI Flash SD Card - XTSD 512 MB memory card under the file name (fileName)

code written by Owen Krumm 2023 for a Adafruit Featherwing M0 with a Adafruit LSM6DSOX + LIS3MDL FeatherWing - Precision 9-DoF IMU and  Adafruit SPI Flash SD Card - XTSD 512 MB chip.

Green light = good and running
Red light = no IMU found
Purple light = no SD memory found
*/

#include <SPI.h>
#include <SD.h>
#include <Adafruit_LSM6DSOX.h>
const int chipSelectSD = A5;
#define PIN 8        // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 1  // Popular NeoPixel ring size
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_LSM6DSOX sox;

const int writeFrequency = 200; //log data every X ms

String fileName = "data.txt";

void setup() {
  pixels.begin();  // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();  // Set all pixel colors to 'off'
  // Open serial communications and wait for port to open:

  Serial.begin(9600);
  // while (!Serial) {
  //   ;  // wait for serial port to connect. Needed for native USB port only
  // }
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelectSD)) {
    Serial.println("Card failed, or not present");
    pixels.setPixelColor(0, pixels.Color(25, 0, 25)); // purple means no memory card found.
    pixels.show();
    // don't do anything more:
    while (true) {
    }
    
  }

  Serial.println("card initialized.");
  configureLSM6DSOX();
}
bool firstLine = true;
void loop() {
  String dataString = "";
  if (firstLine) {  // if first time in loop, print a header
    dataString = "X accel, Y accel, Z accel, Gyro X, Gyro Y, Gyro Z, Temperature";
    File dataFile = SD.open(fileName, FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      // Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
    firstLine = false;
  }


  // make a string for assembling the data to log:
  sensors_event_t accel;
  sensors_event_t gyro;
  sensors_event_t temp;
  sox.getEvent(&accel, &gyro, &temp);


  // read three sensors and append to the string:

  dataString = "";
  dataString += String(accel.acceleration.x);
  dataString += ",";
  dataString += String(accel.acceleration.y);
  dataString += ",";
  dataString += String(accel.acceleration.z);
  dataString += ",";
  dataString += String(gyro.gyro.x);
  dataString += ",";
  dataString += String(gyro.gyro.x);
  dataString += ",";
  dataString += String(gyro.gyro.x);
  dataString += ",";
  dataString += String(temp.temperature);






  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  File dataFile = SD.open(fileName, FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
    //Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
  delay(writeFrequency);
}



void configureLSM6DSOX() {
  // Serial.println("Adafruit LSM6DSOX test!");
  sox.setAccelRange(LSM6DS_ACCEL_RANGE_16_G);

  if (!sox.begin_I2C()) {
    //if (!sox.begin_SPI(LSM_CS)) {
    // if (!sox.begin_SPI(LSM_CS, LSM_SCK, LSM_MISO, LSM_MOSI)) {
    Serial.println("Failed to find LSM6DSOX chip");
    pixels.setPixelColor(0, pixels.Color(25, 0, 0)); //red is bad
    pixels.show();
    while (1) {
      delay(10);
    }
  }
  pixels.setPixelColor(0, pixels.Color(0, 25, 0)); // green is good
  pixels.show();
}
