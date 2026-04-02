#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
#define LED_PIN 13

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {19, 18, 5, 17}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {16, 4, 2, 15}; //connect to the column pinouts of the keypad

const String password = "7814";

String input_password;

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();      // Make sure backlight is on

  for (byte i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  for (byte i = 0; i < COLS; i++) {
    pinMode(colPins[i], OUTPUT);
  }
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600); //initialize serial communication
}

void loop() {
  char key = getKey();
  if (key) 
  {
    Serial.println(key);
    delay(500);

    if (key == '*') 
    {
      input_password = ""; // clear input password
    } 

    else if (key == '#') 
    {
      if (password == input_password) {
        scrollMessage("the password is correct, ACCESS GRANTED");
        digitalWrite(LED_PIN,HIGH);
        delay(3000);
      } 
      else 
      {
        scrollMessage("the password is wrong, ACCESS DENIED");
        digitalWrite(LED_PIN,LOW);
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

char getKey() {
  for (byte c = 0; c < COLS; c++)
  {
    digitalWrite(colPins[c], LOW);
    for (byte r = 0; r < ROWS; r++) 
    {
      if (digitalRead(rowPins[r]) == LOW) {
        digitalWrite(colPins[c], HIGH);
        delay(50);
        return keys[r][c];
      }
    }
    digitalWrite(colPins[c], HIGH);
  }
  return 0;
}



void scrollMessage(const char* message) {
  // Clear the LCD
  lcd.clear();

  // Print the message to the LCD
  lcd.setCursor(0,0);
  lcd.print(message);

  // Calculate the length of the message
  int messageLength = strlen(message);

  // Calculate the scroll amount (LCD width + extra characters)
  int scrollAmount = max(0, messageLength + 1 - 16);

  // Scroll the message to the left
  for (int positionCounter = 0; positionCounter < scrollAmount; positionCounter++) {
    lcd.scrollDisplayLeft();
    delay(300); // Adjust delay for scroll speed
  }

  // Pause at the end of the message
  delay(1000);

  // Scroll the message back to the right
  for (int positionCounter = 0; positionCounter < scrollAmount; positionCounter++) {
    lcd.scrollDisplayRight();
    delay(300); // Adjust delay for scroll speed
  }

  // Pause at the end of the scroll
  delay(1000);
}