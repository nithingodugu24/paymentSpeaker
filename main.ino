#include <SoftwareSerial.h>
#include <DFMiniMp3.h>  //See readme.md to get this library link
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include <FS.h>

// --------------- Configuration ---------------
const char* ssid = "YOUWIFISSID";
const char* password = "YOURWIFIPASS";
const char* host = "api.yourhost.com";
const int port = 80;
const size_t bufferSize = 1024;
String TRANSACTIONS_CSV_PATH = "/transactions.csv";
int failedTimes = 0;
int VOLUME = 30; // 30 IS MAX
int REQUEST_FREQUENCY = 5000;  //How often the request should send 5000=5seconds

int HUNDRED = 100;
int THOUSAND = 1000;
int RUPEES = 25;
int THANKYOU = 26;
int AND = 27;
int POWERON = 21;
int CONNECTING = 22;
int CONNECTED = 23;
int READYPAYMENT = 24;
int IHAVERECIEVED = 28;
int NOTIFICATION = 29;
int ERROROCCURED = 31;
int ERRORREQUEST = 32;
int ERRORINTERNET = 33;

bool audioFinished = true;
bool previousFinished = true;
class Mp3Notify;
typedef DFMiniMp3<SoftwareSerial, Mp3Notify> DfMp3;
SoftwareSerial secondarySerial(5, 4); // RX, TX
DfMp3 dfmp3(secondarySerial);

class Mp3Notify {
public:
  static void PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
    // Implementation remains the same
  }

  static void OnError([[maybe_unused]] DfMp3& mp3, uint16_t errorCode) {
    // Implementation remains the same
  }

  static void OnPlayFinished(DfMp3& mp3, DfMp3_PlaySources source, uint16_t track) {
    audioFinished = true;
  }

  static void OnPlaySourceOnline([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
  }

  static void OnPlaySourceInserted([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
  }

  static void OnPlaySourceRemoved([[maybe_unused]] DfMp3& mp3, DfMp3_PlaySources source) {
  }
};

// ----------------------------------------------

bool playAudioFile(int track) {
  unsigned long startTime = millis();

  audioFinished = false;
  dfmp3.playMp3FolderTrack(track);

  while (!audioFinished && (millis() - startTime) <= 10000) {
    dfmp3.loop();
    delay(100);
  }

  return true;
}





// -------------- DFPlayer Functions -------------
void startMusicPlayer() {
  secondarySerial.begin(115200);
  Serial.println("DFPlayer Starting...");
  Serial.println("Developed by Nirhin Godugu...");
  dfmp3.begin();
  dfmp3.reset();
  uint16_t version = dfmp3.getSoftwareVersion();
  Serial.print("Version: ");
  Serial.println(version);
  uint16_t volume = dfmp3.getVolume();
  Serial.print("Volume: ");
  Serial.println(volume);
  dfmp3.setVolume(VOLUME);
  uint16_t count = dfmp3.getTotalTrackCount(DfMp3_PlaySource_Sd);
  Serial.print("Total Tracks: ");
  Serial.println(count);
  Serial.println("DFPlayer Started...");
  if(playAudioFile(POWERON)){}
  if(playAudioFile(CONNECTING)){}
}

// ----------------------------------------------


// -------------- WiFi Setup Function ------------
void setWifiClient() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  if(playAudioFile(CONNECTED)){}
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// ----------------------------------------------

// -------------- CSV Functions --------------
void createCSVFileIfNotExists() {
  File file = SPIFFS.open("/transactions.csv", "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.println("Id,Amount");
  file.close();
  Serial.println("CSV file created");
}

void addRowToCSV(const char* value1, int value2) {
  File file = SPIFFS.open(TRANSACTIONS_CSV_PATH, "a");
  if (!file) {
    createCSVFileIfNotExists();
    Serial.println("Failed to open file for writing");
    return;
  }
  file.print(value1);
  file.print(",");
  file.println(value2);
  file.close();
}


bool checkCellValue(const char* targetValue) {
  File file = SPIFFS.open(TRANSACTIONS_CSV_PATH, "r");
  if (!file) {
    Serial.println("Failed to open file for reading");
    return false;
  }

  bool isTargetValueFound = false;
  String rowContent;

  while (file.available()) {
    rowContent = file.readStringUntil('\n');
    if (rowContent.indexOf(targetValue) != -1) {
      isTargetValueFound = true;
      break;
    }
  }

  file.close();
  return isTargetValueFound;
}



void emptyCSVFileIfNeeded(int maxRowCount) {
  File file = SPIFFS.open(TRANSACTIONS_CSV_PATH, "r+");
  if (!file) {
    Serial.println("Failed to open file for reading and writing");
    return;
  }

  int rowCount = 0;
  while (file.available() && rowCount <= maxRowCount) {
    if (file.readStringUntil('\n').length() > 0) {
      rowCount++;
    }
  }

  
  file.close();

  if (rowCount > maxRowCount) {
  if (SPIFFS.exists(TRANSACTIONS_CSV_PATH)) {
    if (SPIFFS.remove(TRANSACTIONS_CSV_PATH)) {
      Serial.println("Previous CSV file deleted");
    } else {
      Serial.println("Failed to delete previous CSV file");
      return;
    }
  }

  File file = SPIFFS.open(TRANSACTIONS_CSV_PATH, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  file.println("Id,Amount");
  file.close();
  Serial.println("New CSV file created");

  }
}

// ----------------------------------------------


// Function to speak the amount
bool speakAmount(int amount, unsigned long timeoutMillis = 30000) {
  unsigned long startTime = millis();

  speakThousands(amount);
  speakHundreds(amount);
  speakTens(amount);

  while (!audioFinished && (millis() - startTime) < timeoutMillis) {
    dfmp3.loop();
    delay(100);
  }

  return audioFinished;
}


void speakThousands(int amount){
  int four5Digits = amount % 100000;
  int thousands = four5Digits / 1000;

  if(thousands > 0 && thousands <= 99){
    speakTens(thousands);
    playAudioFile(THOUSAND);
  }
  

}

void speakHundreds(int amount){
  int threeDigits = amount % 1000;
  int hundreds = threeDigits / 100 ;

  if(hundreds > 0 && hundreds < 10){
    playAudioFile(hundreds);
    playAudioFile(HUNDRED);
  }

  if((threeDigits - (hundreds * 100) > 0) && amount > 100){
    playAudioFile(AND);
  }
}

void speakTens(int amount){
  int twoDigits = amount % 100;
  int tens = twoDigits / 10 * 10;
  int ones = twoDigits - tens;

  if(twoDigits > 0 && twoDigits < 20){
    playAudioFile(twoDigits);
    return;
  }

  if(tens > 0 & tens < 99 && tens % 10 == 0){
    playAudioFile(tens);
  }

  if(ones > 0 & ones < 10){
    playAudioFile(ones);
  }
}


// -------------- Payments Functions ------------
void checkTransactions() {
  if(previousFinished){
  previousFinished = false;
  WiFiClient client;
  if (!client.connect(host, port)) {
    Serial.println("Connection failed");
    return;
  }
  client.print("GET /alerts/newMessage.php HTTP/1.1\r\n");
  client.print("Host: ");
  client.println(host);
  client.println("Connection: close");
  client.println();
  Serial.println("Request sent");
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders)) {
    Serial.println(F("Invalid response"));
    return;
  }
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, client);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  String status = doc["status"];
  if(status != "success"){
    previousFinished = true;
    failedTimes = failedTimes + 1;
    if(failedTimes % 10 == 0){
      playAudioFile(ERRORREQUEST);
    }
    return;
  }
  //Serial.print("Status: ");
  //Serial.println(status);

  //Serial.println("Transactions:");
  JsonArray transactions = doc["transactions"];
  for (JsonObject transaction : transactions) {
    const char* merchantTransId = transaction["merchantTransId"];
    int payMoneyAmount = transaction["payMoneyAmount"].as<int>();
    
    if (!checkCellValue(merchantTransId)) {
      Serial.print("MerchantTransId: ");
      Serial.print(merchantTransId);
      Serial.print(", PayMoneyAmount: ");
      Serial.println(payMoneyAmount);
      addRowToCSV(merchantTransId, payMoneyAmount);
      playAudioFile(NOTIFICATION);
      delay(1000);
      playAudioFile(IHAVERECIEVED);
      if (!speakAmount(payMoneyAmount, 30000)) {
        continue;
      }
      delay(100);
      playAudioFile(RUPEES);
      delay(2000);

    }
  }

  client.stop();
  previousFinished = true;
  }
}
void setup() {
  Serial.begin(115200);
  // Initialize SPIFFS
  if (!SPIFFS.begin()) {
    Serial.println("Failed to mount file system");
    return;
  }
  Serial.begin(115200);
  createCSVFileIfNotExists();
  startMusicPlayer();
  setWifiClient();
  delay(1000);
  if(playAudioFile(READYPAYMENT)){}
  checkTransactions();
}
void waitMilliseconds(uint16_t msWait) {
  uint32_t start = millis();
  
  while ((millis() - start) < msWait) {
    dfmp3.loop();
    delay(1);
  }
}

enum State {
  WAITING,
  CHECKING_TRANSACTIONS,
  EMPTYING_CSV
};

State currentState = WAITING;
unsigned long lastStateChangeTime = 0;

void updateStateMachine() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - lastStateChangeTime;

  switch (currentState) {
    case WAITING:
      // Wait for 5 seconds before transitioning to the next state
      if (elapsedTime >= REQUEST_FREQUENCY) {
        currentState = CHECKING_TRANSACTIONS;
        lastStateChangeTime = currentTime;
      }
      break;

    case CHECKING_TRANSACTIONS:
      checkTransactions();
      currentState = EMPTYING_CSV;
      lastStateChangeTime = currentTime;
      break;

    case EMPTYING_CSV:
      emptyCSVFileIfNeeded(50);
      currentState = WAITING;
      lastStateChangeTime = currentTime;
      break;
  }
}



void loop() {
  dfmp3.loop();
  updateStateMachine();
}
