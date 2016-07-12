#define aboveShootPin 26
#define belowShootPin 28


void setup() {
  // put your setup code here, to run once:
  pinMode(aboveShootPin, OUTPUT);
  pinMode(belowShootPin, OUTPUT);
  Serial.begin(9600);
  
  digitalWrite(aboveShootPin, HIGH);
  digitalWrite(belowShootPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly: 
  
  digitalWrite(aboveShootPin, LOW);
  digitalWrite(belowShootPin, LOW);
  Serial.println("Low");
  delay(3000);
  digitalWrite(aboveShootPin, HIGH);
  digitalWrite(belowShootPin, HIGH);
  Serial.println("High");
  delay(3000);
  
}
