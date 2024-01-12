#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <LiquidCrystal_I2C.h>
// WiFi
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
// Wifi network station credentials
#define WIFI_SSID "EMMY"
#define WIFI_PASSWORD "emmanuel"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6934864422:AAGUWTsITGqqCDCSFoOd8pQuAfbQsGawblY"
#define LED_PIN 13

const unsigned long BOT_MTBS = 1000; // mean time between scan messages

bool ledStatus;
unsigned long bot_lasttime; // last time messages' scan has been done
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte rowPins[ROWS] = {19, 18, 5, 17}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {16, 4, 2, 15}; //connect to the column pinouts of the keypad

const String key_password = "147";

String input_password;
String chat_id = "975869805";

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);



char getKey();
void scrollMessage(const char*);

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

void handleNewMessages(int numNewMessages)
{
  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    Serial.println(text);
    String from_name = bot.messages[i].from_name;
     Serial.println(text);
    
    if (from_name == "")
      from_name = "Guest";

    if (text == "/open")
    {
      digitalWrite(LED_PIN, HIGH); // turn the LED on (HIGH is the voltage level)
      scrollMessage("The Password is correct, ACCESS GRANTED");
      ledStatus = 1;
      bot.sendMessage(chat_id, "DOOR IS OPEN", "");
      delay(3000);
    }

    if (text == "/close")
    {
      ledStatus = 0;
      digitalWrite(LED_PIN, LOW); // turn the LED off (LOW is the voltage level)
      scrollMessage("ACCESS DENIED, SECURITY BREACH DETECTED!!!!");
      bot.sendMessage(chat_id, "DOOR is CLOSED", "");
      delay(3000);
    }

    if (text == "/status")
    {
      if (ledStatus)
      {
        bot.sendMessage(chat_id, "DOOR is OPEN", "");
      }
      else
      {
        bot.sendMessage(chat_id, "DOOR is CLOSED", "");
      }
    }

    if (text == "/start")
    {
      String welcome = "Welcome to IoT Based Home Security System, " + from_name + ".\n";
      welcome += "This is a quick guide on how to use the system.\n\n";
      welcome += "/open : to open the Door\n";
      welcome += "/close : to close the Door\n";
      welcome += "/status : Returns current status of your Door\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


