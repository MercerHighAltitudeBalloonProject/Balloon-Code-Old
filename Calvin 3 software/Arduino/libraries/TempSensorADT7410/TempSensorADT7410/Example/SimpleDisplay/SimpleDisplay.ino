#include <Wire.h>
#include <TempSensorADT7410.h>

TempSensorADT7410 mySensorConfigType1;
double temp;

void setup()
{
  Wire.begin();
  // Required - Setup each sensor configuration separately
  mySensorConfigType1.setSensorConfiguration(1);

  // For this example  
  Serial.begin(9600);
}

void loop()
{
  Serial.print(mySensorConfigType1.getFahrenheit());
  Serial.println("    f");
  
  Serial.print(mySensorConfigType1.getCelcius());
  Serial.println("   c");
  
  delay(2000);

}




