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
