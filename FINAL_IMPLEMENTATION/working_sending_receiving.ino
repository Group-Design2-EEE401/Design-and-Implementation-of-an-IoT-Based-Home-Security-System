#include "security.h"

void setup()
{
  Serial.begin(115200);
  Serial.println();


  for (byte i = 0; i < ROWS; i++) 
  {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
  for (byte i = 0; i < COLS; i++) 
  {
    pinMode(colPins[i], OUTPUT);
  }
  pinMode(LED_PIN, OUTPUT);
  // attempt to connect to Wifi network:
  Serial.print("Connecting to Wifi SSID ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi connected. IP address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Retrieving time: ");
  configTime(0, 0, "pool.ntp.org"); // get UTC time via NTP
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  char key = getKey();
  if (key) 
  {
    Serial.println(key);
    delay(500);
    if (key == '*') {
        input_password = ""; // clear input password
      } 
    else if (key == '#') {
      if (key_password == input_password) {
          bot.sendMessage(chat_id,"The Correct Password was input");
          Serial.println("The Correct password was input");
       }                                
      else{
          bot.sendMessage(chat_id,"The Incorrect Password was input");
          Serial.println("The Password is wrong, ACCESS DENIED");
        }

        input_password = ""; // clear input password
        digitalWrite(LED_PIN,LOW);
      }
    else
      {
        input_password += key; // append new character to input password string
      } 
  }

    
  

    if (millis() - bot_lasttime > BOT_MTBS)
    {
      int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

      while (numNewMessages)
      {
        // Serial.println("got response");
        handleNewMessages(numNewMessages);
        numNewMessages = bot.getUpdates(bot.last_message_received + 1);
      }

      bot_lasttime = millis();
    }
}
