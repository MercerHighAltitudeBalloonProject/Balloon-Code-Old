int x = 0;

#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Wire.begin(8);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Wire.onRequest(requestEvent); // register event
  Serial.println("Setup Complete");
}

void loop() {
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) {
  while (0 < Wire.available()) { // loop through all but the last
    Serial.println("\n----------");
    Serial.println(Wire.available());
    char c = Wire.read(); // receive byte as a character
    Serial.print(int(c));         // print the character
  }
  Serial.println("."); 
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  Wire.write("hello 123 "); // respond with message of 6 bytes
  // as expected by master
}
