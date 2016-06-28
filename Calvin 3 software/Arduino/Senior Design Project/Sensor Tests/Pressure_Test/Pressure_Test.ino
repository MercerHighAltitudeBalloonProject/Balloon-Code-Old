#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <BaroSensor.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"
#include "DHT.h"

float baro0Temp;
float baro0Press;
float baro1Temp;
float baro1Press;
float baro2Temp;
float baro2Press;
float baro3Temp;
float baro3Press;

#define BARO 0x76
#define MUX 0x70

//SD card variables
File dataFile;
const int chipSelect = 10;
String data;
RTC_DS1307 RTC;
DateTime logTime;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  SD.begin(10, 11, 12, 13);
  dataFile = SD.open("data.txt", FILE_WRITE);
  dataFile.println("pressure0,pressure1,pressure2,pressure3");
  Serial.println("pressure0,pressure1,pressure2,pressure3");
  dataFile.close();
  
}

void loop() {
  // put your main code here, to run repeatedly:
    
  mux(0);
  BaroSensor.begin();
  baro0Temp = BaroSensor.getTemperature();
  baro0Press = BaroSensor.getPressure();
  mux(1);
  BaroSensor.begin();
  baro1Temp = BaroSensor.getTemperature();
  baro1Press = BaroSensor.getPressure();
  mux(2);
  BaroSensor.begin();
  baro2Temp = BaroSensor.getTemperature();
  baro2Press = BaroSensor.getPressure();
  mux(3);
  BaroSensor.begin();
  baro3Temp = BaroSensor.getTemperature();
  baro3Press = BaroSensor.getPressure();
  
  
  
  //data = baro0Temp.toString() + ", " + baro0Press +  baro1Temp  + ", " +  baro1Press  + ", " + baro2Temp  + ", " + baro2Press  + ", " + baro3Temp  + ", " + baro3Press;
  dataFile = SD.open("data.txt", FILE_WRITE);
  if (dataFile)
  {
    logTime = RTC.now();
    if (logTime.hour() < 10)
      dataFile.print("0");    
    dataFile.print(logTime.hour(), DEC);
    dataFile.print(":");
    if (logTime.minute() < 10)
      dataFile.print("0");
    dataFile.print(logTime.minute(), DEC);
    dataFile.print(":");
    if (logTime.second() < 10)
      dataFile.print("0");
    dataFile.print(logTime.second(), DEC);
    dataFile.print(" ");
    dataFile.print(baro0Press);
    dataFile.print(", ");
    dataFile.print(baro1Press);
    dataFile.print(", ");
    dataFile.print(baro2Press);
    dataFile.print(", ");
    dataFile.print(baro3Press);
    dataFile.println();
    dataFile.close();
    Serial.print(logTime.hour(), DEC);
    Serial.print(":");
    Serial.print(logTime.minute(), DEC);
    Serial.print(":");
    Serial.print(logTime.second(), DEC);
    Serial.print(" ");
    dataFile.close();
  }
  
  delay(2000);

}

void mux(byte channel)
{
  byte controlRegister = 0x04;  
  controlRegister |= channel;
  Wire.beginTransmission(MUX);
  Wire.write(controlRegister);    //set to selected channel
  Wire.endTransmission();
}

