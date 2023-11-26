# Design-and-Implementation-of-an-IoT-Based-Home-Security-System
This repository contains work done on the project - Design and Implementation of an IoT Based Home Security System

# IoT-Based Home Security System

## Design and Implementation

### Overview

The IoT-Based Home Security System is designed to enhance the security of homes through a smart and accessible locking mechanism. The system employs a keypad for code input, allowing authorized users to unlock the door seamlessly. In the event of unauthorized attempts, the system sends a notification to the user via onboard Wi-Fi, ensuring an additional layer of security.

### Features

- **Keypad Input:** The system utilizes a keypad for users to input access codes securely.

- **Secure Locking Mechanism:** Upon entering the correct code, the Arduino, powered by an ESP32 microcontroller, controls a switch (relay or transistor) to enable voltage to the 12VDC solenoid door lock, facilitating secure access.

- **User Authentication:** The system allows authorized users to enter a predefined code for access. Multiple users can be configured with unique access codes.

- **Failed Access Notification:** In the case of five consecutive failed attempts, the system triggers a notification to the user through the onboard Wi-Fi, providing an alert about potential unauthorized access attempts.

### Implementation

The implementation involves integrating the following components:

- **ESP32 Microcontroller:** The ESP32 microcontroller is the core of the system, responsible for processing user input, controlling the locking mechanism, and managing Wi-Fi communication.

- **Keypad:** The keypad acts as the user interface, allowing individuals to input their access codes securely.

- **12VDC Solenoid Door Lock:** The solenoid door lock, powered by a 12VDC supply, is controlled by the microcontroller to grant access upon successful code entry.

- **Wi-Fi Connectivity:** The onboard Wi-Fi capability of the ESP32 enables communication for notifications and alerts.

- **Switching Element (Relay or Transistor):** The switching element facilitates the controlled release of voltage to the solenoid door lock.

### Usage

1. **Code Input:** Users enter their access code using the keypad.

2. **Authentication:** The system authenticates the code, allowing access if the code matches the predefined set.

3. **Secure Unlocking:** Upon successful authentication, the microcontroller controls the switch, allowing voltage to pass to the solenoid door lock, unlocking the door securely.

4. **Failed Attempts Notification:** In the case of five consecutive failed attempts, the system sends a notification to the user through the onboard Wi-Fi.

### Dependencies

- Arduino IDE
- ESP32 Board Support (installed in Arduino IDE)
- Appropriate libraries for keypad interfacing (if any)

### Configuration

- Define access codes for authorized users in the Arduino code.

- Configure Wi-Fi settings for notification functionality.

### License

This project is open-source

### Contributors
Otuoniyo Ufuoma-Oghene Humphery (https://www.linkedin.com/in/humpheryotu/), <br/>
Alimi RereOluwa Victor (alimirereloluwa@gmail.com),<br/>
Asepeoluwa Solutiontech (solutionasepekosile@gmail.com), <br/>
Peter Emmanuel Ameh, </b>
Temioluwa Akinbile

### Acknowledgments
- Dr T.K Yesufu 
- Special thanks to the Arduino and ESP32 communities for their valuable resources and support.
