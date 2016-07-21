//Obtains I2C data from sensors
void getI2CData()
{
  for(int i=0; i<=3; i++)
  {
    mux(i);
    BaroSensor.begin();
    float baroTemp = BaroSensor.getTemperature();
    baroTemp = convertCToF(baroTemp);
    float baroPress = BaroSensor.getPressure();
    tempFloats[i] = baroTemp;
    pressureFloats[i] = baroPress;  
    
    if(i==1)
    {
      tempSend[0] = baroTemp;
    }
  }
  
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





//set to selected channel
void mux(byte channel)
{
  byte controlRegister = 0x04;  
  controlRegister |= channel;
  Wire.beginTransmission(MUX);
  Wire.write(controlRegister);    
  Wire.endTransmission();
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

float convertCToF(float celsius)
{
  float fahrentheit = 1.8*celsius + 32.0; 
  return fahrentheit;
}
