void cutDown(int val)
{
  if (val == 1)
  {
   digitalWrite(aboveShootPin, LOW);
   delay(8000);
   digitalWrite(aboveShootPin, HIGH);
  }
  else if (val == 2)
  {
   digitalWrite(belowShootPin,LOW);
   delay(8000);
   digitalWrite(belowShootPin, HIGH);
  }
}

void setHeader()
{
  dataFile = SD.open("data.txt", FILE_WRITE);
  dataFile.print("Time, Euler Orient X, Euler Orient Y, Euler Orient Z, Quat Orient X, Quat Orient Y, Quat Orient Z, Linear Accel X, Linear Accel Y, Linear Accel Z, Ang Velocity X, Ang Velocity Y, Ang Velocity Z, Accel X, Accel Y, Accel Z, Gravity X, Gravity Y, Gravity Z, Magnetic X, Magnetic Y, Magnetic Z,"); 
  dataFile.println("Baro 0 Temp, Baro 1 Temp, Baro 2 Temp, Baro 3 Temp, BNO Temp, DHT Temp Outside, DHT Temp Inside, Baro 0 Pressure, Baro 1 Pressure, Baro 2 Pressure, Baro 3 Pressure, Humidity Outside, Humidity Inside, Altitude, Latitude, Longitude, Velocity, Heading, Sat Count");
  dataFile.close();
}
