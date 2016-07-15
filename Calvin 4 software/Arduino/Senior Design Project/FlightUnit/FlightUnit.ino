#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <BaroSensor.h>
#include <SPI.h>
#include <SD.h>
#include <TinyGPS.h>
#include "RTClib.h"
#include "DHT.h"

//Datalogger variables
RTC_DS1307 RTC;
DateTime logTime;
DateTime sendTime;
DateTime startTime;
DateTime currentTime;

//BNO055 variables
Adafruit_BNO055 bno = Adafruit_BNO055(55);

//DHT22 variables
#define DHTPin1 22
#define DHTPin2 24
#define DHTType1 DHT22
#define DHTType2 DHT22

//Initiliaze DHT sensors
DHT DHTOutside(DHTPin1, DHTType1);
DHT DHTInside(DHTPin2, DHTType2);

//BNO055 variables
bool bnoIsOK;

//SD card variables
File dataFile;
const int chipSelect = 10;
bool fileExists;

//I2C Mux variables
#define BARO 0x76
#define MUX 0x70

//Relay variables
#define belowShootPin 26
#define aboveShootPin 28

//GPS variables
TinyGPS gps;
float latitude, longitude;
int satCount;
float alti, velocity, heading;

//Array variables for data send
float tempSend[4];
String sendData;
int sendRate;
int storeRate;

//Array variables for data store
int storeIntegers[1];
float accelFloats[21];
float tempFloats[7];
float pressureFloats[4];
float humidityFloats[2];
String storeData;

/**************************************************************************/
/*
    Display sensor calibration status
*/
/**************************************************************************/
void displayCalStatus(void)
{
  /* Get the four calibration values (0..3) */
  /* Any sensor data reporting 0 should be ignored, */
  /* 3 means 'fully calibrated" */
  uint8_t system, gyro, accel, mag;
  system = gyro = accel = mag = 0;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  /* The data should be ignored until the system calibration is > 0 */
  Serial.print("\t");
  if (!system)
  {
    Serial.print("! ");
  }

  /* Display the individual values */
  Serial.print("Sys:");
  Serial.print(system, DEC);
  Serial.print(" G:");
  Serial.print(gyro, DEC);
  Serial.print(" A:");
  Serial.print(accel, DEC);
  Serial.print(" M:");
  Serial.print(mag, DEC);
}

//Arduino setup function that occurs at initiation of program.
void setup(void)
{ 
  sendRate = 20000; 
  storeRate = 3000;
  Wire.begin();
  Serial.begin(9600);
  
  //GPS Setup
  Serial1.begin(4800);
  
  //Radio Setup
  Serial2.begin(9600);
  
  //Relay variables
  pinMode(aboveShootPin, OUTPUT);
  pinMode(belowShootPin, OUTPUT);
  digitalWrite(aboveShootPin, HIGH);
  digitalWrite(belowShootPin, HIGH);
  
  //Datalogger Setup
  RTC.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
  startTime = RTC.now();
  
  //Humidity Sensors Setup
  DHTOutside.begin();
  DHTInside.begin();
  
  //SD Card Setup
  pinMode(SS, OUTPUT);
  SD.begin(10, 11, 12, 13);
  
  if(! SD.exists("data.CSV"))
  {
    setHeader();
  }
  dataFile = SD.open("data.CSV", FILE_WRITE);
  //Accelerometer Setup
  if(bno.begin())
    bnoIsOK = true;
  else
    bnoIsOK = false;
  bno.setExtCrystalUse(true);
}

//Arduino loop function that repeats after setup function finishes.
void loop(void)
{
  unsigned long start = millis();
  while (millis() - start < sendRate)
  {
    newGPSData();
    if ((millis()%storeRate == 0))
    {
      sensors_event_t event;
      getI2CData();
      obtainHumidityInfo();
      obtainGPSInfo();
      writeDataToFile();
      storeData = "";
    }
  }
  sensors_event_t event;
  getI2CData();
  obtainHumidityInfo();
  obtainGPSInfo();
  sendDataToGround();
  sendData = "";
  storeData = "";
  Serial.println("Transmitted");
  
  
  currentTime = RTC.now();
  if (currentTime.hour() - startTime.hour() == 3)
  {
    sendRate = 60000;
  }
}
