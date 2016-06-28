// Required libraries
#include <Wire.h>
#include <TempSensorADT7410.h>

// Required - Use multiple declarations for each sensor configurations (1-4)
TempSensorADT7410 mySensorConfigType1;

// Declarations for this example
int readCounter;
unsigned long beginTime;
unsigned long endTime;
double averageTemp;
unsigned long averageTime;


void setup() {
  // Required
  Wire.begin();
  // Required - Setup each sensor configuration separately
  mySensorConfigType1.setSensorConfiguration(1);

  // For this example  
  Serial.begin(9600);
  readCounter = 0;
  timeFahrenheit();
  timeCelcius();
}

void loop() {
  // For this example
  readCounter+=2;
  Serial.print(readCounter);
  Serial.print(":    ");
  
  // Required to get a reading:
  // mySensorConfigType2.getFahrenheit() 
  Serial.print(mySensorConfigType1.getFahrenheit());
  Serial.print("f    ");
  
  // or 
  // mySensorConfigType2.getCelcius()
  Serial.print(mySensorConfigType1.getCelcius());
  Serial.println("c");
  
  // Delay can be as short as 1 millisecond - delay(1);
  delay(2000);
}

void timeFahrenheit()
{
  int i;
  averageTime = 0;
  averageTemp = 0;
  for(i=1; i<2500; i++)
  {
    beginTime = micros();
    averageTemp += mySensorConfigType1.getFahrenheit();
    endTime = micros();
    averageTime += endTime-beginTime;
    delay(1);
  }
  Serial.print("Average time per read (F): ");
  Serial.print(averageTime/2500);
  Serial.print("us  Average temperature reading (F): ");
  Serial.println(averageTemp/2500);
}

void timeCelcius()
{
  int i;
  averageTime = 0;
  averageTemp = 0;
  for(i=1; i<2500; i++)
  {
    beginTime = micros();
    averageTemp += mySensorConfigType1.getCelcius();
    endTime = micros();
    averageTime += endTime-beginTime;
    delay(1);
  }
  Serial.print("Average time per read (C): ");
  Serial.print(averageTime/2500);
  Serial.print("us  Average temperature reading (C): ");
  Serial.println(averageTemp/2500);
}
