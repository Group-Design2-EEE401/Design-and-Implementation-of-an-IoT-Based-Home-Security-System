#include <SoftwareSerial.h>
//Create software serial object to communicate with mySerial
SoftwareSerial mySerial(17, 16); //mySerial Tx & Rx is connected to Arduino #3 & #2
#define LED_PIN 13
String phoneNumber = "+2348105053591";
void setup(){
  Serial.begin(9600);
   pinMode(LED_PIN,OUTPUT);
  //Begin serial communication with Arduino and mySerial
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);
}

void loop(){
  bool state = receive_sms();
  if(state)
  {
    Serial.println("Received the message");
    digitalWrite(LED_PIN,HIGH);
  }
  else
  {
    digitalWrite(LED_PIN,LOW);
    Serial.println("Wrong message");
  }
}
bool receive_sms(){
  String sms_message;
  mySerial.println("AT");
  delay(1000);
  mySerial.println("AT+CMGF=1");
  delay(1000);
  mySerial.println("AT+CNMI = 1,2,0,0,0");
  delay(10);
  if (mySerial.available()>0)
  {
    sms_message = mySerial.readString();
    sms_message.trim();
    Serial.println(sms_message);
    delay(10);
  }

  if((sms_message.indexOf(phoneNumber) > 0) && (sms_message.indexOf("Grant Access")  > 0)){
      return true;
  }
  else
  {
    return false;
  }
  }