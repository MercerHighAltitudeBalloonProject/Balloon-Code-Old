void sendMessageToAll(String message){
  for (int indexAddr = 0; indexAddr < sizeof(deviceAddresses)/sizeof(deviceAddresses[0]); indexAddr++) 
  {
     int currentAddr = deviceAddresses[indexAddr];
     
     String trimmed = "#" + message;
     //trimmed.replace(" ","");
     //trimmed.trim();
     for(int x=0; x<trimmed.length();x=x+32)
     {
       
       int endIndex = x+32;
       if(endIndex>trimmed.length())
       {
         endIndex = trimmed.length();
       }
       
       
       Wire.beginTransmission(currentAddr);
       Serial.print("ADDR: " + String(currentAddr) + " ");
       Serial.println(trimmed.substring(x,endIndex));
       Wire.write(trimmed.substring(x,endIndex).c_str());
       Wire.endTransmission();
     }
  }
}
