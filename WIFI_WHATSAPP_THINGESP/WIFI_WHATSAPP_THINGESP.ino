#include <WiFi.h>
#include <ThingESP.h>

ThingESP32 thing("humpheryufuoma7", "HomeSecurity", "Esp32WroomSecurity");

int LIGHT = 13;

unsigned long previousMillis = 0;
const long INTERVAL = 6000; 

void setup()
{
  Serial.begin(115200);
  pinMode(LIGHT, OUTPUT);
  thing.SetWiFi("avengers_mifi", "maxkkott2018");
  thing.initDevice();
}

String HandleResponse(String query)
{
  if (query == "lights on") {
    digitalWrite(LIGHT, 1);
    return "Done: Light Turned ON";
  }

  else if (query == "lights off") {
    digitalWrite(LIGHT, 0);
    return "Done: Light Turned OFF";
  }

  else if (query == "light status"){
    return digitalRead(LIGHT) ? "LIGHT is OFF" : "LIGHT is ON";}
  else {
    Serial.println(query);
    return "Your query was invalid..";}
}

void loop()
{
  thing.Handle();
}