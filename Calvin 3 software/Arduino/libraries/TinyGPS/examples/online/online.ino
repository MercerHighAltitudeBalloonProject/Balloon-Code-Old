#include <TinyGPS.h>
#include <SoftwareSerial.h>


#define GPS_RX_PIN 2
#define GPS_TX_PIN 3

TinyGPS gps;
SoftwareSerial ss(GPS_RX_PIN, GPS_TX_PIN);

void setup()
{
 Serial.begin(9600);
 ss.begin(4800); 
 }
 
void loop()
{
 while (ss.available())
 {
   char c = byte(ss.read());
   // Here i tried Serial.write(c); and i succesully saw the NMEA data in my serial monitor
   if (gps.encode(c))
   {
     long lat, lon;
    unsigned long fix_age;
   gps.get_position(&lat, &lon, &fix_age);
   if (fix_age == TinyGPS::GPS_INVALID_AGE )
     Serial.println("No fix ever detected!");
   else if (fix_age > 2000)
     Serial.println("Data is getting STALE!");
   else
     Serial.println("Latitude and longitude valid!");
     
     Serial.print("Lat: "); 
     Serial.print(lat);
     Serial.print(" Lon: "); 
     Serial.println(lon);
     
   }
   else Serial.println("Not encoded data!");
 }
}
