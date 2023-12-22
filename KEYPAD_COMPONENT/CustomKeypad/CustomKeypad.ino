

/* @file CustomKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates changing the keypad size and key values.
|| #
*/
// Include Arduino Wire library for I2C
#include <Wire.h>
// Include LCD display library for I2C
#include <LiquidCrystal_I2C.h>
// Include Keypad library
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the cymbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

#define Password_Length 7
// Character to hold password input
char Data[Password_Length];
// Password
char Master[Password_Length] = "123456";
 
// Pin connected to lock relay input
int lockOutput = 10;
// Character to hold key input
char customKey;
// Counter for character entries
byte data_count = 0;

byte pin_rows[ROW_NUM]      = {19, 18, 5, 17}; // GPIO19, GPIO18, GPIO5, GPIO17 connect to the row pins
byte pin_column[COLUMN_NUM] = {16, 4, 0, 2};   // GPIO16, GPIO4, GPIO0, GPIO2 connect to the column pins

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
// Create LCD object
LiquidCrystal_I2C lcd(0x20, 16, 2); 

void setup(){
  // Setup LCD with backlight and initialize
  lcd.init();
  lcd.backlight();
  
  lcd.setCursor(1,0);
  lcd.print("HelloWorld");
  delay(1000);
  // Set lockOutput as an OUTPUT pin
  pinMode(lockOutput, OUTPUT);
}
  
void loop(){
  // Initialize LCD and print
  lcd.setCursor(0, 0);
  lcd.print("Enter Password:");
  delay(4000);
  // Look for keypress
  customKey = customKeypad.getKey();
  if (customKey) {
    // Enter keypress into array and increment counter
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    //lcd.print(Data[data_count]);
    lcd.print("Entered");
    data_count++;
  }
 
  // See if we have reached the password length
  if (data_count == Password_Length - 1) {
    lcd.clear();
 
    if (!strcmp(Data, Master)) {
      // Password is correct
      lcd.print("Correct");
      // Turn on relay for 5 seconds
      digitalWrite(lockOutput, HIGH);
      delay(5000);
      digitalWrite(lockOutput, LOW);
    }
    else {
      // Password is incorrect
      digitalWrite(lockOutput, LOW);
      lcd.print("Incorrect");
      delay(1000);
      
    }
 
    // Clear data and LCD display
    lcd.clear();
    clearData();
  }
}

void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}
