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
