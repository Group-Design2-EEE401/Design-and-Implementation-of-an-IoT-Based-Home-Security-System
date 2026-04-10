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
        Serial.println("The password is correct, ACCESS GRANTED!");
        digitalWrite(LED_PIN,HIGH);
        delay(3000);
      } 
      else 
      {
        Serial.println("The password is incorrect, ACCESS DENIED!");
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