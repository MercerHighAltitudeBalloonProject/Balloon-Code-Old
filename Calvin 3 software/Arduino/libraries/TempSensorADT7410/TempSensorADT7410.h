/**
 * Version 1.0
 *
 * Brandon Bearden
 * July 08, 2014
 *
 * Possible configurations
 * Configuration 1 : A0 -> GND, A1 -> GND
 * Configuration 2 : A0 -> GND, A1 -> Vcc
 * Configuration 3 : A0 -> Vcc, A1 -> GND
 * Configuration 4 : A0 -> Vcc, A1 -> Vcc
 */
#ifndef TempSensorADT7410_h
#define TempSensorADT7410_h

#include "Arduino.h"
#include "Wire.h"

class TempSensorADT7410
{
// Very simple... declare global constructor, set config in setup, get temp C/F
public:
    // Default null constructor
    TempSensorADT7410();
    
    // User defined configuration (1-4)
    bool setSensorConfiguration(int sensorConfiguration);
    
    // Returns either celcius or fahrenheit
    double getCelcius();
    double getFahrenheit();

// Nothing to see here!
private:
    int _sensorAddress;
    void initializeSensor();
    int readSensor();
    double signAdjustedRawValue();
    double convertToFahrenheit(bool convert);
};
#endif