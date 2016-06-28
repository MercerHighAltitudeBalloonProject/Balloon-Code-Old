/**
 * Version 1.0
 *
 * Brandon Bearden
 * July 08, 2014
 *
 * Conversion Notes (These are applied within this class):
 * RAW 16-bit, signed data from ADT7410
 *    (tempReading / 128) for positive temps in Celcius
 *    ((tempReading - 65536) / 128) for negative temps in Celcius
 * For Farenheight ((ABOVERESULT * 1.8) + 32)
 *
 * 0x48 - Configuration 1 : A0 -> GND, A1 -> GND
 * 0x49 - Configuration 2 : A0 -> GND, A1 -> Vcc
 * 0x4A - Configuration 3 : A0 -> Vcc, A1 -> GND
 * 0x4B - Configuration 4 : A0 -> Vcc, A1 -> Vcc
 * 0x00 - ADT7410 Temperature Register
 * 0x03 - ADT7410 Configuration Regsiter
 */
 
#include "Arduino.h"
#include "Wire.h"
#include "TempSensorADT7410.h"

/******************************************************************************
 * Public
 */
TempSensorADT7410::TempSensorADT7410() {

}

bool TempSensorADT7410::setSensorConfiguration(int sensorConfiguration) {
    switch (sensorConfiguration) {
        case 2:
            _sensorAddress = 0x49;
            break;
        
        case 3:
            _sensorAddress = 0x4A;
            break;
        
        case 4:
            _sensorAddress = 0x4B;
            break;
        
        default:
            _sensorAddress = 0x48;
    }
    initializeSensor();
    
    return true;
}

double TempSensorADT7410::getCelcius() {
    return convertToFahrenheit(false);
}

double TempSensorADT7410::getFahrenheit() {
    return convertToFahrenheit(true);
}

/******************************************************************************
 * Private
 */

int _sensorAddress;

/**
 * Initialization of the ADT7410 sets the configuration register based on input
 * from the Analog Devices datasheet page 14. 16-bit resolution selected.
 */
void TempSensorADT7410::initializeSensor() {
    Wire.beginTransmission(_sensorAddress);
    Wire.write(0x03);
    Wire.write(B01110001); 
    Wire.endTransmission();
}

/**
 * Sends request for temperature read and returns raw value
 */
int TempSensorADT7410::readSensor() {
  // Send request for temperature register.
  Wire.beginTransmission(_sensorAddress);
  Wire.write(0x00);
  Wire.endTransmission();
  
  // Listen for and acquire 16-bit register address.
  Wire.requestFrom(_sensorAddress,2);
  byte MSB = Wire.read();
  byte LSB = Wire.read();
  
  // return the 16-bit signed value.
  return ((MSB << 8) | LSB);
}

/**
 * Adjusts for negative values and 16-bit conversion
 */
double TempSensorADT7410::signAdjustedRawValue() {
    double sensorValue = readSensor();
    if(sensorValue < 0)
       return (sensorValue-65536);
    else
       return sensorValue;
}

/**
 * Converts to fahrenheit or raw conversion unchanged (celcius)
 */
double TempSensorADT7410::convertToFahrenheit(bool convert) {
    if(convert)
        return (signAdjustedRawValue() / 128.0 * 1.8) + 32;
    else
        return signAdjustedRawValue() / 128.0;
}