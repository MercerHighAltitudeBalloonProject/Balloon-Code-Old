#include "Wire.h"
#include "BaroSensor.h"

#define BARO 0x76
#define MUX 0x70


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  mux(0);
  BaroSensor.begin();
  mux(1);
  BaroSensor.begin();
  mux(2);
  BaroSensor.begin();
  mux(3);
  BaroSensor.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  mux(0);
  BaroSensor.begin();
  Serial.print("Baro 0 Temp: ");
  Serial.print(BaroSensor.getTemperature());
  Serial.print("   Baro 0 Pressure: ");
  Serial.println(BaroSensor.getPressure());
  mux(1);
  BaroSensor.begin();
  Serial.print("Baro 1 Temp: ");
  Serial.print(BaroSensor.getTemperature());
  Serial.print("   Baro 1 Pressure: ");
  Serial.println(BaroSensor.getPressure());
  mux(2);
  BaroSensor.begin();
  Serial.print("Baro 2 Temp: ");
  Serial.print(BaroSensor.getTemperature());
  Serial.print("   Baro 2 Pressure: ");
  Serial.println(BaroSensor.getPressure());
  mux(3);
  BaroSensor.begin();
  Serial.print("Baro 3 Temp: ");
  Serial.print(BaroSensor.getTemperature());
  Serial.print("   Baro 3 Pressure: ");
  Serial.println(BaroSensor.getPressure());
  
  delay(4000);
}

void mux(byte channel)
{
  byte controlRegister = 0x04;  
  controlRegister |= channel;
  Wire.beginTransmission(MUX);
  Wire.write(controlRegister);    //set to selected channel
  Wire.endTransmission();
}
