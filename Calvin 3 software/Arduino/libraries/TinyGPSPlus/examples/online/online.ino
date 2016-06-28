#include <TinyGPS++.h>
#include <SoftwareSerial.h>


#define GPS_RX_PIN 2
#define GPS_TX_PIN 3

TinyGPSPlus gps;
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);

void setup()
{
  Serial.begin(9600);
  ss.begin(4800); 
}

char clat[11];
char clng[11];

void loop()
{

        bool isGpsLocationValid = false;
        do
        {  
            while (ss.available()>0)
            {
                char c = byte(ss.read());
                if (gps.encode(c)) //i previously used if(gps.encode(ss.read())); but no luck then
                {    
                    if (gps.location.isValid())
                    {
                        dtostrf(gps.location.lat(), 11, 6, clat);
                        dtostrf(gps.location.lng(), 11, 6, clng);                        
                        isGpsLocationValid = true;
                    }
                }   
             }
        } while (isGpsLocationValid == false);
        Serial.write(clat);
        Serial.println();
        Serial.write(clng);
}
