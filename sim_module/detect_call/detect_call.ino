#include <SoftwareSerial.h>

SoftwareSerial SIM800(17, 16); // RX, TX

void setup() {
  Serial.begin(9600);
  SIM800.begin(9600); // Set the baud rate of SIM800

  // Configure SIM800 to notify about incoming calls
  SIM800.println("AT+CLIP=1");

  delay(1000);
}

void loop() {
  if (SIM800.available()) {
    // Read the incoming data from SIM800
    String message = SIM800.readString();
    
    // Check if the message contains the incoming call notification
    if (message.indexOf("RING") != -1) {
      // Extract the caller's number from the message
      int startIdx = message.indexOf("\"") + 1;
      int endIdx = message.indexOf("\"", startIdx + 1);
      String callerNumber = message.substring(startIdx, endIdx);
      Serial.println(callerNumber);
      // Check if the call is from a particular number
      if (callerNumber.equals("+2349065637151")) {
        Serial.println("Call received from desired_number");
        // Your code to handle the call from the desired number
      } else {
        Serial.println("Call received from unknown number");
        // Your code to handle the call from an unknown number
      }
    }
  }
}
