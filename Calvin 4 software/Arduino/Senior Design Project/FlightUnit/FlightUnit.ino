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
  
  currentTime = RTC.now();
  if (currentTime.hour() - startTime.hour() == 3)
  {
    sendRate = 60000;
  }
}

//Obtains I2C data from sensors
void getI2CData()
{
  mux(0);
  BaroSensor.begin();
  float baro0Temp = BaroSensor.getTemperature();
  baro0Temp = convertCToF(baro0Temp);
  float baro0Press = BaroSensor.getPressure();
  tempFloats[0] = baro0Temp;
  pressureFloats[0] = baro0Press;
  mux(1);
  BaroSensor.begin();
  float baro1Temp = BaroSensor.getTemperature();
  baro1Temp = convertCToF(baro1Temp);
  float baro1Press = BaroSensor.getPressure();
  tempFloats[1] = baro1Temp;
  pressureFloats[1] = baro1Press;
  tempSend[0] = baro1Temp;
  mux(2);
  BaroSensor.begin();
  float baro2Temp = BaroSensor.getTemperature();
  baro2Temp = convertCToF(baro2Temp);
  float baro2Press = BaroSensor.getPressure();
  tempFloats[2] = baro2Temp;
  pressureFloats[2] = baro2Press;
  mux(3);
  BaroSensor.begin();
  float baro3Temp = BaroSensor.getTemperature();
  baro3Temp = convertCToF(baro3Temp);
  float baro3Press = BaroSensor.getPressure();
  tempFloats[3] = baro3Temp;
  pressureFloats[3] = baro3Press;

  if(bnoIsOK)
  {
    /*
    uint8_t system, gyro, accel, mag;
    system = gyro = accel = mag = 0;
    bno.getCalibration(&system, &gyro, &accel, &mag);
  
    Serial.print("Sys:");
    Serial.println(system, DEC);
    */
    
    //Get Euler vector orientation (360 sphere)
    sensors_event_t event;
    bno.getEvent(&event);
    float x_Euler_orient = event.orientation.x;
    float y_Euler_orient = event.orientation.y;
    float z_Euler_orient = event.orientation.z;
    accelFloats[0] = x_Euler_orient;
    accelFloats[1] = y_Euler_orient;
    accelFloats[2] = z_Euler_orient;
    
    //Get Quaterion vector orientation (four point quaterion)
    imu::Quaternion quat_vector = bno.getQuat();
    float x_Quat_orient = quat_vector.x();
    float y_Quat_orient = quat_vector.y();
    float z_Quat_orient = quat_vector.z();
    accelFloats[3] = x_Quat_orient;
    accelFloats[4] = y_Quat_orient;
    accelFloats[5] = z_Quat_orient;

    //Get linear acceleration vector (acceleration minus gravity) m/s^2
    imu::Vector<3> linear_acceleration = bno.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL);
    float linearAccel_x  = linear_acceleration.x();
    float linearAccel_y  = linear_acceleration.y();
    float linearAccel_z  = linear_acceleration.z();
    accelFloats[6] = linearAccel_x;
    accelFloats[7] = linearAccel_y;
    accelFloats[8] = linearAccel_z;
    
    //Get angular velocity in rad/s
    imu::Vector<3> angularVelocity = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    float angularVelocity_x = angularVelocity.x();
    float angularVelocity_y = angularVelocity.y();
    float angularVelocity_z = angularVelocity.z();
    accelFloats[9] = angularVelocity_x;
    accelFloats[10] = angularVelocity_y;
    accelFloats[11] = angularVelocity_z;
    
    //Get acceleration vector (gravity + linear motion)
    imu::Vector<3> acceleration = bno.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
    float acceleration_x = acceleration.x();
    float acceleration_y = acceleration.y();
    float acceleration_z = acceleration.z();
    accelFloats[12] = acceleration_x;
    accelFloats[13] = acceleration_y;
    accelFloats[14] = acceleration_z;
    
    //Get gravity vector (gravitational acceleration with out movement) m/s^2
    imu::Vector<3> gravity_vector = bno.getVector(Adafruit_BNO055::VECTOR_GRAVITY);
    float gravityVector_x = gravity_vector.x();
    float gravityVector_y = gravity_vector.y();
    float gravityVector_z = gravity_vector.z();
    accelFloats[15] = acceleration_x;
    accelFloats[16] = acceleration_y;
    accelFloats[17] = acceleration_z;

    //Get magnetic field strength in micro Tesla
    imu::Vector<3> magneticField = bno.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER);
    float magneticField_x = magneticField.x();
    float magneticField_y = magneticField.y();
    float magneticField_z = magneticField.z();
    accelFloats[18] = acceleration_x;
    accelFloats[19] = acceleration_y;
    accelFloats[20] = acceleration_z;    
    
    float BNO_temp = bno.getTemp();
    BNO_temp = convertCToF(BNO_temp);
    tempFloats[4] = BNO_temp;
    tempSend[1] = BNO_temp;
  }
}

void obtainHumidityInfo()
{
  float humidityOutside = DHTOutside.readHumidity();
  float humidityInside = DHTInside.readHumidity();
  float dhtTempOutside = DHTOutside.readTemperature();
  dhtTempOutside = convertCToF(dhtTempOutside);
  float dhtTempInside = DHTInside.readTemperature();
  dhtTempInside = convertCToF(dhtTempInside);
  tempFloats[5] = dhtTempOutside;
  tempFloats[6] = dhtTempInside;
  humidityFloats[0] = humidityOutside;
  humidityFloats[1] = humidityInside;
  tempSend[2] = dhtTempInside;
  tempSend[3] = dhtTempOutside;
}

void obtainGPSInfo()
{
  static const float LONDON_LAT = 51.508131, LONDON_LON = -0.128002;
  unsigned long age;
  gps.f_get_position(&latitude, &longitude, &age);
  satCount = gps.satellites();
  alti = gps.f_altitude();
  velocity = gps.f_speed_kmph();
  heading = gps.f_course();
}

void writeDataToFile()
{
  //Appending accelerometer data
  for (int i = 0; i < sizeof(accelFloats)/sizeof(float); i++)
  {
    char buff[10];
    float val = accelFloats[i];
    String appendage = dtostrf(val, 7, 2, buff);
    storeData += appendage + ",";
  }
   
  //Appending temperature data
    for (int i = 0; i < sizeof(tempFloats)/sizeof(float); i++)
  {
    char buff[6];
    float val = tempFloats[i];
    String appendage = dtostrf(val, 6, 2, buff);
    storeData += appendage + ",";
  }  
  
  //Appending pressure data
  for (int i = 0; i < sizeof(pressureFloats)/sizeof(float); i++)
  {
    char buff[7];
    float val = pressureFloats[i];
    String appendage = dtostrf(val, 7, 2, buff);
    storeData += appendage + ",";
  }
  
  //Appending humidity data
    for (int i = 0; i < sizeof(humidityFloats)/sizeof(float); i++)
  {
    char buff[5];
    float val = humidityFloats[i];
    String appendage = dtostrf(val, 5, 2, buff);
    storeData += appendage + ",";
  }
  
  //Appending GPS information
  char buff[20];
  String appendage = dtostrf(alti, 5, 0, buff);
  storeData += appendage + ",";
  //Coordinates
  appendage = dtostrf(latitude, 15, 7, buff);
  storeData += appendage+ ",";
  appendage = dtostrf(longitude, 15, 7, buff);
  storeData += appendage + ",";
  //Velocity
  appendage = dtostrf(velocity, 8, 3, buff);
  storeData += appendage + ",";
  //Heading
  appendage = dtostrf(heading, 5, 1, buff);
  storeData += appendage + ",";
  //Satellite count
  String sat = String(satCount);
  storeData += sat;
  Serial.println(storeData);
  
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
    dataFile.print(",");
    dataFile.println(storeData);
    dataFile.close();
  }
}

void sendDataToGround()
{
  //Appending pressure information
  for (int i = 0; i < sizeof(pressureFloats)/sizeof(float); i++)
  {
    char buff[4];
    float val = pressureFloats[i];
    String appendage = dtostrf(val, 7, 2, buff);
    sendData += appendage + ",";
  }

  //Appending temperature information
   for (int i = 0; i < sizeof(tempSend)/sizeof(float); i++)
  {
    char buff[6];
    float val = tempSend[i];
    String appendage = dtostrf(val, 6, 2, buff);
    sendData += appendage + ",";
  }

  //Appending humidity information
   for (int i = 0; i < sizeof(humidityFloats)/sizeof(float); i++)
  {
    char buff[5];
    float val = humidityFloats[i];
    String appendage = dtostrf(val, 5, 2, buff);
    sendData += appendage + ","  ;
  }
  //Appending GPS information
  //altitude
  char buff[20];
  String appendage = dtostrf(alti, 5, 0, buff);
  sendData += appendage + ",";
  //Coordinates
  appendage = dtostrf(latitude, 15, 7, buff);
  sendData += appendage+ ",";
  appendage = dtostrf(longitude, 15, 7, buff);
  sendData += appendage + ",";
  //Velocity
  appendage = dtostrf(velocity, 8, 3, buff);
  sendData += appendage + ",";
  //Heading
  appendage = dtostrf(heading, 5, 1, buff);
  sendData += appendage + ",";
  //Satellite count
  appendage = dtostrf(satCount, 2, 0, buff);
  sendData += appendage;
  Serial.println("Sat count: " + String(satCount));
  Serial.println(sendData);
  Serial2.println(sendData);
}

//set to selected channel
void mux(byte channel)
{
  byte controlRegister = 0x04;  
  controlRegister |= channel;
  Wire.beginTransmission(MUX);
  Wire.write(controlRegister);    
  Wire.endTransmission();
}

static bool newGPSData()
{
  String input;
  while (Serial1.available())
  {
    if (gps.encode(Serial1.read()))
      return true;
  }
  if (Serial2.available() > 0)
  {
    String input = Serial2.readString();
    if (input.substring(0,7).equals("cutdown"))
      cutDown(2);
    else if (input.substring(0,4).equals("data"))
    {
      Serial.println("sending data");
      sensors_event_t event;
      getI2CData();
      obtainHumidityInfo();
      obtainGPSInfo();
      sendDataToGround();
    }  
    else if (input.substring(0,7).equals("setRate"))
    {
      String rate = input.substring(8, input.length());
      char holder[20];
      rate.toCharArray(holder, 20);
      int rateValue = atoi(holder);
      sendRate = rateValue;
    }
  }
  return false;
}

void cutDown(int val)
{
  if (val == 1)
  {
   digitalWrite(aboveShootPin, LOW);
   delay(8000);
   digitalWrite(aboveShootPin, HIGH);
  }
  else if (val == 2)
  {
   digitalWrite(belowShootPin,LOW);
   delay(8000);
   digitalWrite(belowShootPin, HIGH);
  }
}

void setHeader()
{
  dataFile = SD.open("data.txt", FILE_WRITE);
  dataFile.print("Time, Euler Orient X, Euler Orient Y, Euler Orient Z, Quat Orient X, Quat Orient Y, Quat Orient Z, Linear Accel X, Linear Accel Y, Linear Accel Z, Ang Velocity X, Ang Velocity Y, Ang Velocity Z, Accel X, Accel Y, Accel Z, Gravity X, Gravity Y, Gravity Z, Magnetic X, Magnetic Y, Magnetic Z,"); 
  dataFile.println("Baro 0 Temp, Baro 1 Temp, Baro 2 Temp, Baro 3 Temp, BNO Temp, DHT Temp Outside, DHT Temp Inside, Baro 0 Pressure, Baro 1 Pressure, Baro 2 Pressure, Baro 3 Pressure, Humidity Outside, Humidity Inside, Altitude, Latitude, Longitude, Velocity, Heading, Sat Count");
  dataFile.close();
}

float convertCToF(float celsius)
{
  float fahrentheit = 1.8*celsius + 32.0; 
  return fahrentheit;
}
