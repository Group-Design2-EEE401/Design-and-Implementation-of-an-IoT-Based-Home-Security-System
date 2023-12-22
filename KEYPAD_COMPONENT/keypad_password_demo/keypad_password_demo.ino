/*
 * This ESP32 code is created by esp32io.com
 *
 * This ESP32 code is released in the public domain
 *
 * For more detail (instruction and wiring diagram), visit https://esp32io.com/tutorials/esp32-keypad
 */

#include <Keypad.h>

#define ROW_NUM     4 // four rows
#define COLUMN_NUM  4 // three columns
#define LED_PIN 13

char hexaKeys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte pin_rows[ROW_NUM] = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins


Keypad keypad = Keypad( makeKeymap(hexaKeys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

const String password = "7814"; // change your password here
String input_password;

void setup() {
  Serial.begin(9600);
  input_password.reserve(32); // maximum input characters is 33, change if needed
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  char key = keypad.getKey();

  if (key) {
    Serial.println(key);

    if (key == '*') 
    {
      input_password = ""; // clear input password
    } 

    else if (key == '0') 
    {
      if (password == input_password) {
        Serial.println("The password is correct, ACCESS GRANTED!");
        digitalWrite(LED_PIN,HIGH);
        delay(3000);
      } 
      else 
      {
        Serial.println("The password is incorrect, ACCESS DENIED!");
        digitalWrite(LED_PIN,LOW);
        Serial.print("The Pinmode");
        Serial.println(LED_PIN);
      }
      input_password = ""; // clear input password
      digitalWrite(LED_PIN,LOW);
    } 

    else 
    {
      input_password += key; // append new character to input password string
    }
  }
}
