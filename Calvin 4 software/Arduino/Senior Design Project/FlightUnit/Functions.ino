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
