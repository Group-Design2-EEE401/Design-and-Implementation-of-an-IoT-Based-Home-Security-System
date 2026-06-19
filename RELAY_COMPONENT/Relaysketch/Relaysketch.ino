#define Relay_Pin 13


void setup(){
  pinMode(Relay_Pin, OUTPUT);
}


void loop(){
  digitalWrite(Relay_Pin, HIGH);
  delay(3000);
  digitalWrite(Relay_Pin, LOW);
  delay(3000);
}