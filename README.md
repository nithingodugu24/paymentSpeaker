# DIY Speaker with ESP8266 NodeMCU

This repository contains the code for a (DIY) speaker project using an ESP8266 NodeMCU,basically it is like a Paytm,Bhartpe,GooglePay Payment Notification speaker.This is made up with a DFPlayer Mini, and a speaker. The speaker is designed to play voice alerts when a specific message is received in a periodic HTTP request (You can use Paytm Business / Phonepe API to fetch the transactions and expose the data to an endpoint so the ESP8266 can connect to that endpoint and fetch latest transactions. And also it stores the Transaction IDs in a csv file to prevent repetative announcement of earlier transactions. You will need a SD card in that there should a folder (mp3) and inside it a b bunch of mp3 files used for the announcement of Transaction. You can simply download the mp3 folder from this repository and copy that folder into SD Card insert it in DFPlayer mini.

## Components Used
- **ESP8266 NodeMCU**: WiFi-enabled microcontroller.
- **DFPlayer Mini**: MP3 player module for playing audio files.
- **Speaker**: Used for playing voice alerts.

## Project Description
The system periodically sends an HTTP request to a specified server `api.testing.com`(Replace it with your own web address ) to check for new messages. Upon receiving a response, the program processes the data and plays corresponding voice alerts. The voice alerts are stored as MP3 files on the DFPlayer Mini.

## Configuration
The project is configured with the following parameters in the code:

- **WiFi Credentials:** Set your WiFi SSID and password.
- **Server Configuration:** Specify the server host, port, and other relevant details.
- **Audio Configuration:** Adjust the volume, request frequency, and MP3 track IDs for various alerts.

## Wiring
- Connect the RX and TX pins of the DFPlayer Mini to the specified GPIO pins on the ESP8266 NodeMCU.
- Connect the speaker to the DFPlayer Mini.

## Usage
1. Upload the code to the ESP8266 NodeMCU.
2. Power up the circuit.
3. The system will connect to WiFi and start checking for messages periodically.
4. Voice alerts will be played based on the received messages.

## Audio Alerts
- `POWERON`: Power-on sound.
- `CONNECTING`: Connecting to WiFi sound.
- `CONNECTED`: Connected to WiFi sound.
- `READYPAYMENT`: Sound indicating the system is ready to process payments.
- `NOTIFICATION`: Notification sound for new messages.
- `IHAVERECIEVED`: Acknowledgment sound.
- `RUPEES`: Sound indicating a currency unit.
- `THOUSAND`, `HUNDRED`: Numeric sounds.
- `AND`: Sound indicating the conjunction "and."
- `ERROROCCURED`, `ERRORREQUEST`, `ERRORINTERNET`: Error sounds.

## Notes
- The code includes error handling and attempts to reconnect if the server connection fails.
- Ensure you have the required libraries (`SoftwareSerial`, [DFMiniMp3](https://github.com/Makuna/DFMiniMp3), `ESP8266WiFi`, `WiFiClientSecure`, `ArduinoJson`, `FS`) installed.
