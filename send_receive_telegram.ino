
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// Wifi network station credentials
#define WIFI_SSID "EMMY"
#define WIFI_PASSWORD "emmanuel"
// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "6934864422:AAGUWTsITGqqCDCSFoOd8pQuAfbQsGawblY"

const unsigned long BOT_MTBS = 1000; // mean time between scan messages
const int led = 13;

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // last time messages' scan has been done

void handleNewMessages(int numNewMessages)
{
  for (int i = 0; i < numNewMessages; i++)
  {
    // bot.sendMessage(bot.messages[i].chat_id, bot.messages[i].text, "");
    String text = bot.messages[i].text;
    Serial.println(text);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(led,OUTPUT);
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
  bool status = !status;
  digitalWrite(led,status);
  if(digitalRead(led))
  {
    bot.sendMessage("975869805","LED Pin is High", "");
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
