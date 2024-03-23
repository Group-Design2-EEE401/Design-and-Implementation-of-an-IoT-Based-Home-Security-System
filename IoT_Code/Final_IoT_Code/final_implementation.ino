
#include "security.h"

void setup() {
  
  Serial.begin(BAUD_RATE); //initialize serial communication
  activate_LCD();
  pinMode(LED_PIN, OUTPUT);
  connect_wifi();
  digitalWrite(LED_PIN,HIGH);
  activate_SIM();
  for (byte i = 0; i < ROWS; i++) {
      pinMode(rowPins[i], INPUT_PULLUP);
    }     
  for (byte i = 0; i < COLS; i++)
  {
    pinMode(colPins[i], OUTPUT);
  }
}

void loop() {
  client.loop();
  
  if(message == "OFFLINE")
  {
    if (key) 
    {
      Serial.println(key);
      delay(500);

      if (key == '*') 
      {
        input_password = " "; // clear input password
      } 

      else if (key == '#') 
      {
        if (password == input_password) {
          scrollMessage("CORRECT PASSWORD INPUT,GRANT ACCESS");
          // delay(3000);
          process_SMS_correct();
        }
        else 
        {
          scrollMessage("The password is wrong, ACCESS DENIED"); 
          process_SMS_incorrect(wrong_status);
        } 
        input_password = " "; // clear input password
        digitalWrite(LED_PIN,HIGH);
      } 
    }
    else 
    {
      input_password += key; // append new character to input password string
    }
  }


  else
  {
    Serial.println(key);
    delay(500);

    if (key == '*') 
    {
      input_password = " "; // clear input password
    } 

    else if (key == '#') 
    {
      if (password == input_password) {
        scrollMessage("CORRECT PASSWORD INPUT,GRANT ACCESS");
        send_IOT();

      else 
      {
        scrollMessage("The password is wrong, ACCESS DENIED");
        client.publish(topic, wrong_status);  
      }
      input_password = " "; // clear input password
      digitalWrite(LED_PIN,HIGH);
    } 

    else 
    {
      input_password += key; // append new character to input password string
    }
  }
}
