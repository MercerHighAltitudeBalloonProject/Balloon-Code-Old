

void sendMessageToAll(String message){
  for (int indexAddr = 0; indexAddr < sizeof(deviceAddresses); indexAddr++) {
     int currentAddr = deviceAddresses[indexAddr];
     Wire.beginTransmission(8);
     Wire.write(message.c_str());
     Wire.endTransmission();
  }
}
