#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>


// WiFi
const char *ssid = "Okoli's Reno"; // Enter your Wi-Fi name
const char *password_ = "d4seibck";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp32/group_design_send";
const char *topic_ = "esp32/group_design_receive";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;
String message;

String client_id = "esp32-client-";
WiFiClient espClient;
PubSubClient client(espClient);
//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(17, 16); //SIM800L Tx & Rx is connected to Arduino #3 & #2

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

const String password = "7914";

String input_password;

void setup() {
  lcd.init();
  lcd.clear();
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200); //initialize serial communication
  mySerial.begin(115200);
  digitalWrite(LED_PIN,HIGH);
  // Make sure backlight is on         
  lcd.backlight();      

  // Connecting to a WiFi network
  WiFi.begin(ssid, password_);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the Wi-Fi network");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);
  while (!client.connected()) {
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public EMQX MQTT broker connected");
      } else {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
      }

  for (byte i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLUP);
  }
    for (byte i = 0; i < COLS; i++) {
      pinMode(colPins[i], OUTPUT);
    }

  }

    // Publish and subscribe
  client.publish(topic, "Hi, I'm ESP32 ^^");
  client.subscribe(topic_);

}

void callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("Message arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");

    // Check if the message is from the same client
     message = ' ';

    for (int i = 0; i < length; i++) {
        message += (char) payload[i];
    }
    Serial.println();
    Serial.println("-----------------------");
    Serial.println(message);
}

void loop() {
  client.loop();
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
        // scrollMessage("the password is correct, ACCESS NOT GRANTED");
        // delay(3000);
        client.publish(topic, "GRANT ACCESS");
        delay(20000);
        // if (client.connected() && client.available()) {
        // Check for new MQTT messages and process them accordingly
        client.loop();
        // }
        message.trim();
        Serial.print("here before:");
        Serial.println(message);
        if (message == "Access granted"){
          Serial.println("it entered access");
          // scrollMessage("ACCESS GRANTED");
          digitalWrite(LED_PIN,LOW);
          message = ' ';
        } 
         delay(2000);
      } 
      else 
      {
        // scrollMessage("the password is wrong, ACCESS DENIED");
        client.publish(topic, "SOMEONE TRYING TO BREAK IN");
        digitalWrite(LED_PIN,HIGH);
      }
      input_password = ""; // clear input password
      digitalWrite(LED_PIN,HIGH);
    } 

    else 
    {
      input_password += key; // append new character to input password string
    }
  }
  // message = " ";
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

