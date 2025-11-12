#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <HTTPClient.h>   
#include <ArduinoJson.h>  

const char* ssid = "SENAC-Mesh";
const char* password = "09080706";

#define APP_KEY    "3e98388a-4ffc-4f10-a0c1-246c9046d27b"
#define APP_SECRET "f6ea6c00-9a77-495b-bd70-9ddf67e60222-ba1cba85-958f-49c1-a506-ce767a469de5"
#define DEVICE_ID  "68df39b8359ccc32ce0a8c45"

const char* backendURL = "https://projeto-integrador-f86b.onrender.com/api/rfid";  
const char* apiKey     = "abc123meuTokenSuperSecreto!";        

#define SS_PIN 10   
#define RST_PIN 9   
#define MOSI 6
#define MISO 5
#define SCK  4
MFRC522 mfrc522(SS_PIN, RST_PIN);

#define LED_PIN 2  

String lastUid = "";
unsigned long lastTrigger = 0;
const unsigned long triggerDebounceMs = 3000;

// Callback Alexa
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("[SinricPro] Device %s -> %s\n", deviceId.c_str(), state ? "ON" : "OFF");
  return true;
}

void sendToBackend(String uid) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(backendURL);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("x-api-key", apiKey);

    StaticJsonDocument<256> doc;
    doc["uid"] = uid;
    doc["tag_type"] = "MIFARE";
    doc["rssi"] = -45; 
    String body;
    serializeJson(doc, body);

    int httpResponseCode = http.POST(body);

    if (httpResponseCode > 0) {
      Serial.printf("Backend response (%d): %s\n", httpResponseCode, http.getString().c_str());
    } else {
      Serial.printf("Error sending to backend: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}

void setup() {
  Serial.begin(115200);
  delay(100);

  Serial.println("Conectando ao WiFi...");
WiFi.begin(ssid, password);

unsigned long startAttemptTime = millis();

while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 20000) {
  delay(500);
  Serial.print(".");
}

if (WiFi.status() == WL_CONNECTED) {
  Serial.println("\n WiFi conectado com sucesso!");
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
} else {
  Serial.println("\n Falha ao conectar ao WiFi.");
  Serial.println("Verifique SSID e senha, ou teste com hotspot do celular.");
}


  SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(APP_KEY, APP_SECRET);

  SPI.begin(SCK, MISO, MOSI, SS_PIN);
  mfrc522.PCD_Init();
  Serial.println("Approach your RFID tag...");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
}

void loop() {
  SinricPro.handle();

  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String uidStr = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) uidStr += "0";
      uidStr += String(mfrc522.uid.uidByte[i], HEX);
    }
    uidStr.toUpperCase();

    unsigned long now = millis();
    if (uidStr != lastUid || (now - lastTrigger) > triggerDebounceMs) {
      Serial.print("Tag detected: ");
      Serial.println(uidStr);

      lastUid = uidStr;
      lastTrigger = now;

      SinricProSwitch &sw = SinricPro[DEVICE_ID];
      bool sentOn = sw.sendPowerStateEvent(true);
      if(sentOn) Serial.println("Event 'On' sent to SinricPro");

      sendToBackend(uidStr);

      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON (10s)");
      delay(10000);
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  delay(50);
}
