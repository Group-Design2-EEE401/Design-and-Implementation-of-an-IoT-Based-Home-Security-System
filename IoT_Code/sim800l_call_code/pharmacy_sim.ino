#include <SoftwareSerial.h>





// Configure the SIM800L TX/RX pins
#define SIM800_TX_PIN 17
#define SIM800_RX_PIN 16

SoftwareSerial sim800l(SIM800_TX_PIN, SIM800_RX_PIN);

void setup() {
  Serial.begin(9600); // Serial monitor
  sim800l.begin(9600); // SIM800L module
  delay(1000); // Give SIM800L some time to initialize
}

void loop() {
  // Make a call
  makeCall("+2349065637151"); // Replace "+1234567890" with the number you want to call
  delay(30000); // Wait for 30 seconds before making another call
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
