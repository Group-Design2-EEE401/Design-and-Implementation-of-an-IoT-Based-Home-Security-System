#include <SoftwareSerial.h>


#define SIM800_RX_PIN 16 // Connect SIM800L TX to this pin
#define SIM800_TX_PIN 17 // Connect SIM800L RX to this pin
#define PHONE_NUMBER "+23488242491" // Replace with the phone number you want to call

SoftwareSerial sim800Serial(SIM800_RX_PIN, SIM800_TX_PIN);

void setup() {
  // Start serial communication with SIM800L
  sim800Serial.begin(9600);
  // Wait for the SIM800L to initialize (adjust the delay if needed)
  delay(2000);
  // Enable echo (optional)
  sim800Serial.println("ATE1");
  delay(100);
}

void loop() {
  // Make a call
  makeCall();
  // Wait for a while (adjust the delay if needed)
  delay(5000);
  // Check for new messages
  checkMessages();
  // Wait for a while (adjust the delay if needed)
  delay(5000);
}

void makeCall() {
  // Dial the phone number
  sim800Serial.print("ATD");
  sim800Serial.print(PHONE_NUMBER);
  sim800Serial.println(";");
  delay(10000); // Adjust the delay according to the call duration
  // Hang up the call
  sim800Serial.println("ATH");
  delay(2000); // Wait a bit before next action
}

void checkMessages() {
  // Set message format to text mode
  sim800Serial.println("AT+CMGF=1");
  delay(100);
  // List all received messages
  sim800Serial.println("AT+CMGL=\"ALL\"");
  delay(500);
  // Check for new message indication
  if (sim800Serial.find("+CMTI:")) {
    // Read the message index
    int index = sim800Serial.parseInt();
    // Read the message content using index
    sim800Serial.println("AT+CMGR=" + String(index));
    delay(1000);
    // Read and print the message
    while (sim800Serial.available()) {
      char c = sim800Serial.read();
      Serial.write(c); // Print the message to the Serial Monitor
    }
  }
}
