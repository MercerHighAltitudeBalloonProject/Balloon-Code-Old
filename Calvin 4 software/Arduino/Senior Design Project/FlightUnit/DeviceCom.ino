

void sendMessageToAll(String message){
  for (int indexAddr = 0; indexAddr < sizeof(deviceAddresses)/sizeof(deviceAddresses[0]); indexAddr++) 
  {
     int currentAddr = deviceAddresses[indexAddr];
     
     Wire.beginTransmission(currentAddr);
     Serial.println(message);
     Wire.write(message.c_str());
     Wire.endTransmission();
  }
}
