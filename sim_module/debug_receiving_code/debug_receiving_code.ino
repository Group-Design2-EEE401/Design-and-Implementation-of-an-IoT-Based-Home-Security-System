#include <SoftwareSerial.h>
//Create software serial object to communicate with mySerial
SoftwareSerial mySerial(17, 16); //mySerial Tx & Rx is connected to Arduino #3 & #2
#define LED_PIN 13
String phoneNumber = "+2349065637151";
void setup(){
  Serial.begin(9600);
   pinMode(LED_PIN,OUTPUT);
  //Begin serial communication with Arduino and mySerial
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);
}

void loop() {
  // put your main code here, to run repeatedly:
        Serial.println(" ");
        Serial.println("Entering receive message: ");
        delay(20000);
        bool state;
        String sms_message;
        mySerial.println("AT");
        delay(1000);
        mySerial.println("AT+CMGF=1");
        delay(1000);
        mySerial.println("AT+CNMI = 1,2,0,0,0");
        delay(10000);
        // mySerial.println("AT+CMGDA=\"DEL ALL\"");
        if (mySerial.available()>0)
        {
          delay(10000);
          sms_message = mySerial.readString();
          sms_message.trim();
          Serial.println(sms_message);
          delay(10000);
        }
        
        if((sms_message.indexOf(phoneNumber) >= 0) && (sms_message.indexOf("Grant Access")  >= 0)){
           state=true;
        }
        else
        {
          state=false;
        }
        // sms = receive_sms();
        // delay(1000);
        // Serial.print("here before:");
        // Serial.println(sms);
        if (state){
          Serial.println("it entered sms access");
          // scrollMessage("ACCESS GRANTED");
          digitalWrite(LED_PIN,LOW);
          state= false;
        } 
         else{
          Serial.println("it didn't enter sms access");
         }
}
