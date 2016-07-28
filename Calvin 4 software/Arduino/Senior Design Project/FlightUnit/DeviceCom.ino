//Sends a message to each of the addresses in (deviceAddresses)
//The message is broken up into 32byte blocks with a terminator of #
void sendMessageToAll(String message){
  for (int indexAddr = 0; indexAddr < sizeof(deviceAddresses)/sizeof(deviceAddresses[0]); indexAddr++) 
  {
     int currentAddr = deviceAddresses[indexAddr];
     
     String terminatedMessage = message + "#";
     //trimmed.replace(" ","");
     //trimmed.trim();
     for(int x=0; x<terminatedMessage.length();x=x+32)
     {
       //Ensure there is 32 bytes left, else get the correct end. 
       int endIndex = x+32;
       if(endIndex>terminatedMessage.length())
       {
         endIndex = terminatedMessage.length();
       }
       
       //Transmit the 32byte fragment
       Wire.beginTransmission(currentAddr);
       Serial.print("ADDR: " + String(currentAddr) + " ");
       Serial.println(terminatedMessage.substring(x,endIndex));
       Wire.write(terminatedMessage.substring(x,endIndex).c_str());
       Wire.endTransmission();
     }
  }
}

void receiveMessageFromAll()
{
  int command = -1;
  Serial.println("----Receive-----");
  for (int indexAddr = 0; indexAddr < sizeof(deviceAddresses)/sizeof(deviceAddresses[0]); indexAddr++) 
  {
    int currentAddr = deviceAddresses[indexAddr];
    Serial.print("Address:");
    Serial.print(currentAddr);
    Serial.print(" - ");
    
    Wire.requestFrom(currentAddr, 15);    // request 6 bytes from slave device #8
    
    while (Wire.available()) { // slave may send less than requested
      char c = Wire.read(); // receive a byte as character
      if(command== -1)
      {
        if(c == 'k'){
          command = 1;
        }
        else if(c == 's'){
          command = 2;
        }
        continue;
      }
      receiveData += c;
      //Serial.print(c);         // print the character
    }
    Serial.println(receiveData);
    if(command == 1){
      String thisFile = String(currentAddr) + ".txt";
      Serial.println(thisFile);
      writeStringToFile(thisFile,receiveData,true);
    }
    receiveData = "";
  }
  
}

