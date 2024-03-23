#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#define LED_PIN 13
#define BAUD_RATE 115200
// Configure the SIM800L TX/RX pins
#define SIM800_TX_PIN 17
#define SIM800_RX_PIN 16

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

String phoneNumber = "+2348105053591";
String message = '\0';
String wrong_status = "WRONG PASSWORD INPUT,LIKELY BREAKIN";
String correct_status = "CORRECT PASSWORD INPUT, GRANT ACCESS";
bool sms = 0;
String client_id = "esp32-client-";
String input_password = '\0';

WiFiClient espClient;

SoftwareSerial sim800l(SIM800_TX_PIN, SIM800_RX_PIN);

PubSubClient client(espClient);
//Create software serial object to communicate with SIM800L

LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display

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

const String password = "7914";

// functions used 
void callback(char *topic, byte *payload, unsigned int length);
bool receive_sms();
void scrollMessage(const char* message);
char getKey();
void makeCall(String phoneNumber);
void activate_SIM();
void connect_wifi();
void activate_LCD();
void send_IOT();
void process_SMS_correct();
void process_SMS_incorrect();
void grant_access();
void send_SMS(String message_status);

void send_SMS(String send_message){
  // sim800l.println("AT");
  sim800l.println("AT+CMGF=1");
  sim800l.println("AT+CMGS=\"+2348105053591\"");
  sim800l.print(send_message);
  sim800l.write(26);
}

void grant_access()
{
  if (message == "Access granted" || sms == TRUE){
    Serial.println("it entered access");
    scrollMessage("ACCESS GRANTED");
    digitalWrite(LED_PIN,LOW);
    delay(5000);
  } 

  else{
    scrollMessage("ACCESS DENIED, NOT EXPECTING A VISITOR");
    digitalWrite(LED_PIN,HIGH);
  }
    message = ' ';
    sms = FALSE;
}

void send_IOT(){
  client.publish(topic,correct_status);
  delay(20000);
        // if (client.connected() && client.available()) {
        // Check for new MQTT messages and process them accordingly
  client.loop();
        // }
  message.trim();
  // Serial.print("here before:");
  // Serial.println(message);
  grant_access();
  delay(2000); 
}

void activate_LCD(){
    lcd.init();
    lcd.clear();
    lcd.backlight(); // Make sure backlight is on 
}

void sim_activate(){
    sim800l.begin(BAUD_RATE); // SIM800L module
    delay(1000); // Give SIM800L some time to initialize
}

void connect_wifi(){
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
    while (!client.connected()) 
    {
      String client_id = "esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) 
        {
          Serial.println("Public EMQX MQTT broker connected");
        } 
        else 
        {
          Serial.print("failed with state ");
          Serial.print(client.state());
          delay(2000);
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

void makeCall(String phoneNumber) {
  sim800l.println("AT"); // Check if the module is ready
  delay(1000);
  if (sim800l.find("OK")) {
    Serial.println("Module is ready.");
  } else {
    Serial.println("Module is not responding.");
    return;
  }

  // Set the phone number
  sim800l.print("ATD"); // ATD command for dialing
  sim800l.print(phoneNumber);
  sim800l.println(";"); // Dial the number
  delay(10000); // Wait for 10 seconds

  // End the call
  sim800l.println("ATH"); // ATH command for hanging up
}

bool receive_sms(){
  sim800l.println("AT");
  sim800l.println("AT+CMGF=1");
  sim800l.println("AT+CNMI = 1,2,0,0,0");
  delay(10);
  if (sim800l.available()>0)
  {
    String sms_message = sim800l.readString();
    sms_message.trim();
    delay(10)
  }

  if((sms_message.indexOf(phoneNumber) > 0) && (sms_message.indexOf("Grant Access")  > 0)){
      sms = TRUE;
  }
  else
  {
    sms = FALSE;
  }
  
}

void process_SMS_correct(){
  makeCall();
  send_SMS(correct_status);
  delay(10000);
  receive_sms();
  grant_access()
}

void process_SMS_incorrect(){
  makeCall();
  send_SMS(wrong_status);
}