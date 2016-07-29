int x = 0;
String lastReceived;
String building;
float receivedValues[50];
int currentValue = 0;

#include <Wire.h>

void setup() {
  Serial.begin(9600);
  lastReceived = "";
  Wire.begin(7);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.println("Setup Complete");
  }

void loop() {
  delay(1000);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last
    //Serial.println("\n----------");
    //Serial.println(Wire.available());

    char c = Wire.read(); // receive byte as a character
    //Serial.print(c);
    if(c=='#'){
      receivedMessage();
      currentValue = 0;
      building = "";
      memset(receivedValues, 0, sizeof(receivedValues));
    }
    else if(c == ',')
    {
      //Serial.print("[");
      //Serial.print(currentValue);
      //Serial.print(",");
      //Serial.print(building);
      //Serial.println("]");
      if(building != "")
      {
       receivedValues[currentValue] = building.toFloat();
      }
      else
      {
       receivedValues[currentValue] = 0.0;
      }
      currentValue++;
      building = "";
    }
    //48 = '0' and 57 = '9'
    else if( (c >= 48 && c <= 57) || c=='-' || c=='.' ) 
    {
      building += c;
    }
  }
}

void receivedMessage()
{
  Serial.println("----------------");
  char buff[10];
  for (int i = 0; i < 50; i++) {
    Serial.println(receivedValues[i]);  
  }
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() 
{
  Wire.write("kI am address 7");
  Serial.println("Sending Message");
}
