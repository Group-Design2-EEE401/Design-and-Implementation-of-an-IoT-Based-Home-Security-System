#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <esp_system.h>


#define SIM800_TX_PIN 17
#define SIM800_RX_PIN 16

// WiFi
const char *ssid = "Umidigi A3_Pro"; // Enter your Wi-Fi name
const char *password_ = "maxkkott2018";  // Enter Wi-Fi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "esp32/group_design_send";
const char *topic_1 = "esp32/group_design_receive";
const char *topic_2 = "esp32/group_design_status";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int mqtt_port = 1883;
// String message;
String message_1; // Variable to store message from topic 1
String message_2; // Variable to store message from topic 2

String client_id = "esp32-client-";
String phoneNumber = "+2348105053591";
String wrong_status = "WRONG PASSWORD INPUT,LIKELY BREAKIN";
String correct_status = "CORRECT PASSWORD INPUT, GRANT ACCESS";
bool sms = 0;
WiFiClient espClient;
PubSubClient client(espClient);

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
SoftwareSerial mySerial(SIM800_TX_PIN, SIM800_RX_PIN);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
#define LED_PIN 13
#define light 33
 
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {19, 18, 5, 26}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {27, 4, 2, 15}; //connect to the column pinouts of the keypad\

const String password = "714";

String input_password;

void setup() {
  activate_LCD();
  pinMode(LED_PIN, OUTPUT);
  pinMode(light, OUTPUT);
  Serial.begin(9600); //initialize serial communication
  digitalWrite(LED_PIN,HIGH);
  digitalWrite(light,HIGH);
  // Make sure backlight is on         
  mySerial.begin(9600); // mySerial module
  delay(1000); // Give mySerial some time to initialize

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
  client.subscribe(topic_1);
  client.subscribe(topic_2);

}

void activate_LCD(){
    lcd.init();
    lcd.clear();
    lcd.backlight(); // Make sure backlight is on 
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message: ");

  // Convert payload to string
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char) payload[i];
  }
  message.trim();
  // Store message in the corresponding variable based on the topic
  if (strcmp(topic, topic_1) == 0) {
    message_1 = message;
  } else if (strcmp(topic, topic_2) == 0) {
    message_2 = message;
  }

  Serial.println(message);
  Serial.println("-----------------------");
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

    else if (key == 'D') 
    {
      if(message_2 == "ONLINE"){

      if (password == input_password) {
        scrollMessage("Password Correct, GRANT ACCESS");
        // delay(3000);
        client.publish(topic, "GRANT ACCESS");
        delay(20000);
        // if (client.connected() && client.available()) {
        // Check for new MQTT messages and process them accordingly
        client.loop();
        // }
        
        Serial.print("here before:");
        Serial.println(message_1);
        if (message_1 == "Access granted"){
          Serial.println("it entered access");
          // scrollMessage("ACCESS GRANTED");
          digitalWrite(LED_PIN,LOW);
          message_1 = ' ';
        } 
         delay(2000);
      } 
      else 
      {
        scrollMessage("the password is wrong, ACCESS DENIED");
        client.publish(topic, "SOMEONE TRYING TO BREAK IN");
        digitalWrite(LED_PIN,HIGH);
      }
    }

    else{
      if (password == input_password) {
          
        scrollMessage("Password Correct,GRANT ACCESS");
        Serial.println("Entering call and send: ");
        delay(1000);
        mySerial.println("AT"); // Check if the module is ready
        delay(1000);
        mySerial.println("ATD+2349159868774;"); // ATD command for dialing
        delay(20000); // Wait for 10 seconds
        mySerial.println("ATH"); // ATH command for hanging up
        delay(2000);


        //send message
        mySerial.println("AT"); // Check if the module is ready
        delay(1000);
        mySerial.println("AT+CMGF=1");
        delay(1000);
        mySerial.println("AT+CMGS=\"+2348105053591\"");
        delay(1000);
        mySerial.print(correct_status);
        delay(1000);
        mySerial.write(26);
        delay(10000);


        //receive message
        Serial.println(" ");
        Serial.println("Entering receive message: ");
        delay(20000);
        bool state;
        String sms_message;
        mySerial.println("AT");
        delay(1000);
        mySerial.println("AT+CMGF=1");
        delay(1000);
        mySerial.println("AT+CNMI = 1,2,0,0,0");
        delay(1000);
        if (mySerial.available()>0)
          {
            delay(10000);
            sms_message = mySerial.readString();
            sms_message.trim();
            Serial.println(sms_message);
            delay(10000);
          
          
          if((sms_message.indexOf(phoneNumber) >= 0) && (sms_message.indexOf("Grant Access")  >= 0)){
            state=true;
            digitalWrite(LED_PIN,LOW);
            Serial.println("it entered sms access");
            // scrollMessage("ACCESS GRANTED");
            state= false;
            delay(2000);
          }
          else
          {
            state=false;
            Serial.println("it didn't enter sms access");
          }
        }
      } 
      else 
      {
        scrollMessage("Password Wrong, ACCESS DENIED");
        // lcd.print("wrong password");
        Serial.println("Entering call and send wrong password: ");
        delay(1000);
        mySerial.println("AT"); // Check if the module is ready
        mySerial.println("ATD+2349159868774;"); // ATD command for dialing
        delay(20000); // Wait for 10 seconds
        mySerial.println("ATH"); // ATH command for hanging up
        delay(3000);

        // //send message
        mySerial.println("AT"); // Check if the module is ready
        delay(1000);
        mySerial.println("AT+CMGF=1");
        delay(1000);
        mySerial.println("AT+CMGS=\"+2348105053591\"");
        delay(1000);
        mySerial.print(wrong_status);
        delay(1000);
        mySerial.write(26);
        delay(5000);
      }
      esp_restart();
    }
      input_password = ""; // clear input password
      digitalWrite(LED_PIN,HIGH);
      // reconnectMQTT(); // Reconnect MQTT client if not connected
      
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





void reconnectMQTT() {
  // Attempt to reconnect to the MQTT broker
  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s reconnects to the public MQTT broker\n", client_id.c_str());
    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public EMQX MQTT broker reconnected");
      client.subscribe(topic_1);
      client.subscribe(topic_2);
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}

