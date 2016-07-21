String getStorageString()
{
  String storageString = "";
    //Appending accelerometer data
  for (int i = 0; i < sizeof(accelFloats)/sizeof(float); i++)
  {
    char buff[10+1];
    float val = accelFloats[i];
    String appendage = dtostrf(val, 7, 2, buff);
    storageString += appendage + ",";
  }
 
  //Appending temperature data
    for (int i = 0; i < sizeof(tempFloats)/sizeof(float); i++)
  {
    char buff[6+1];
    float val = tempFloats[i];
    String appendage = dtostrf(val, 7, 2, buff);
    storageString += appendage + ",";
  }
  
  //Appending pressure data
  for (int i = 0; i < sizeof(pressureFloats)/sizeof(float); i++)
  {
    char buff[7+1];
    float val = pressureFloats[i];
    String appendage = dtostrf(val, 8, 2, buff);
    storageString += appendage + ",";
  }
  
  //Appending humidity data
    for (int i = 0; i < sizeof(humidityFloats)/sizeof(float); i++)
  {
    char buff[5];
    float val = humidityFloats[i];
    String appendage = dtostrf(val, 5, 2, buff);
    storageString += appendage + ",";
  }
  
  //Appending GPS information
  char buff[20];
  String appendage = dtostrf(alti, 5, 0, buff);
  storageString += appendage + ",";
  
  //Coordinates
  appendage = dtostrf(latitude, 15, 7, buff);
  storageString += appendage+ ",";
  
  appendage = dtostrf(longitude, 15, 7, buff);
  storageString += appendage + ",";
 
  //Velocity
  appendage = dtostrf(velocity, 8, 3, buff);
  storageString += appendage + ",";
  
  //Heading
  appendage = dtostrf(heading, 5, 1, buff);
  storageString += appendage + ",";
  
  //Satellite count
  String sat = dtostrf(satCount, 2, 0, buff);
  storageString += sat;
  
  return storageString;
}

//Output a strings contents to an SD Card
void writeStringToFile(String fileName, String data, bool appendTimeBool)
{
  //Open the SD Card to the file given
  dataFile = SD.open(fileName.c_str(), FILE_WRITE);
  if (dataFile)
  {
    //Append the time to the file
    if(appendTimeBool)
    {
      appendTime(dataFile);
      dataFile.print(",");
    }
    
    //Print the data to the file and close. 
    dataFile.println(data);
    dataFile.close();
  }
}

void appendTime(File file)
{
     logTime = RTC.now();
    
    if (logTime.hour() < 10)
      file.print("0");    
    file.print(logTime.hour(), DEC);
    file.print(":");
    if (logTime.minute() < 10)
      file.print("0");
    file.print(logTime.minute(), DEC);
    file.print(":");
    if (logTime.second() < 10)
      file.print("0");
    file.print(logTime.second(), DEC); 
}

void setHeader()
{
  dataFile = SD.open("data.txt", FILE_WRITE);
  dataFile.print("Time, Euler Orient X, Euler Orient Y, Euler Orient Z, Quat Orient X, Quat Orient Y, Quat Orient Z, Linear Accel X, Linear Accel Y, Linear Accel Z, Ang Velocity X, Ang Velocity Y, Ang Velocity Z, Accel X, Accel Y, Accel Z, Gravity X, Gravity Y, Gravity Z, Magnetic X, Magnetic Y, Magnetic Z,"); 
  dataFile.println("Baro 0 Temp, Baro 1 Temp, Baro 2 Temp, Baro 3 Temp, BNO Temp, DHT Temp Outside, DHT Temp Inside, Baro 0 Pressure, Baro 1 Pressure, Baro 2 Pressure, Baro 3 Pressure, Humidity Outside, Humidity Inside, Altitude, Latitude, Longitude, Velocity, Heading, Sat Count");
  dataFile.close();
}
