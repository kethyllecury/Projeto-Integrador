#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <HTTPClient.h>   
#include <ArduinoJson.h>  

const char* ssid = "Status";
const char* password = "casadosparasempre";

const char* backendURL = "https://projeto-integrador-f86b.onrender.com/api/rfid";  
const char* apiKey     = apiKey    

#define SS_PIN 10   // SDA
#define RST_PIN 9   // RST
#define MOSI 6
#define MISO 5
#define SCK  4

MFRC522 mfrc522(SS_PIN, RST_PIN);

String lastUid = "";
unsigned long lastTrigger = 0;
const unsigned long triggerDebounceMs = 3000;

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


  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected!");

  SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(APP_KEY, APP_SECRET);

  SPI.begin(SCK, MISO, MOSI, SS_PIN);
  mfrc522.PCD_Init();
  Serial.println("Approach your RFID tag...");
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
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  delay(50);
}
