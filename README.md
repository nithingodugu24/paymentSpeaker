#DIY Speaker with ESP8266 NodeMCU
This repository contains the code for a Do-It-Yourself (DIY) speaker project using an ESP8266 NodeMCU, a DFPlayer Mini, and a speaker. The speaker is designed to play voice alerts when a specific message is received in a periodic HTTP request. The project utilizes the capabilities of the ESP8266, allowing for voice notifications based on incoming data.

##Components Used
ESP8266 NodeMCU: WiFi-enabled microcontroller.
DFPlayer Mini: MP3 player module for playing audio files.
Speaker: Used for playing voice alerts.
Project Description
The system periodically sends an HTTP request to a specified server (api.srisaionline.co.in) to check for new messages. Upon receiving a response, the program processes the data and plays corresponding voice alerts. The voice alerts are stored as MP3 files on the DFPlayer Mini.

##Configuration
The project is configured with the following parameters in the code:

WiFi Credentials: Set your WiFi SSID and password.
Server Configuration: Specify the server host, port, and other relevant details.
Audio Configuration: Adjust the volume, request frequency, and MP3 track IDs for various alerts.
##Wiring
Connect the RX and TX pins of the DFPlayer Mini to the specified GPIO pins on the ESP8266 NodeMCU.
Connect the speaker to the DFPlayer Mini.
##Usage
Upload the code to the ESP8266 NodeMCU.
Power up the circuit.
The system will connect to WiFi and start checking for messages periodically.
Voice alerts will be played based on the received messages.
##Audio Alerts
POWERON: Power-on sound.
CONNECTING: Connecting to WiFi sound.
CONNECTED: Connected to WiFi sound.
READYPAYMENT: Sound indicating the system is ready to process payments.
NOTIFICATION: Notification sound for new messages.
IHAVERECIEVED: Acknowledgment sound.
SRISAI: Custom sound identifier.
RUPEES: Sound indicating a currency unit.
THOUSAND, HUNDRED: Numeric sounds.
AND: Sound indicating the conjunction "and."
ERROROCCURED, ERRORREQUEST, ERRORINTERNET: Error sounds.
##Notes
The code includes error handling and attempts to reconnect if the server connection fails.
Ensure you have the required libraries (SoftwareSerial, DFMiniMp3, ESP8266WiFi, WiFiClientSecure, ArduinoJson, FS) installed.
