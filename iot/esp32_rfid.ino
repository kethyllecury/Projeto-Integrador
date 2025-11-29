#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>


// =======================
// WIFI
// =======================
const char* ssid = ssid;
const char* password = password;

// =======================
// BACKEND
// =======================
const char* backendURL = "https://projeto-integrador-f86b.onrender.com/api/rfid";
const char* apiKey = apiKey;

// =======================
// RFID PINS
// =======================
#define SS_PIN 7
#define RST_PIN 10
#define MOSI 6
#define MISO 5
#define SCK 4

MFRC522 mfrc522(SS_PIN, RST_PIN);

// =======================
// SINAIS
// =======================
#define LED_PIN 2
#define BUZZER_PIN 3

String lastUid = "";
unsigned long lastTrigger = 0;
const unsigned long triggerDebounceMs = 3000;

// =======================
// BUZZER
// =======================
unsigned long buzzerLast = 0;
const unsigned long buzzerOnTime = 1000;
const unsigned long buzzerOffTime = 1000;
bool buzzerState = false;

// ============================================================
// QA BÁSICO (só para evitar enviar lixo ao backend)
// ============================================================
bool isHex(String s) {
  for (int i = 0; i < s.length(); i++) {
    if (!isxdigit(s.charAt(i))) return false;
  }
  return true;
}

bool validateUid(String uid) {
  if (uid.length() < 8 || uid.length() > 16) return false;
  if (!isHex(uid)) return false;
  return true;
}

// ============================================================
// CALLBACK DO SINRIC
// ============================================================
bool onPowerState(const String &deviceId, bool &state) {
  Serial.printf("[SinricPro] Dispositivo %s -> %s\n",
                deviceId.c_str(), state ? "ON" : "OFF");
  return true;
}

// ============================================================
// ENVIO AO BACKEND
// ============================================================
void sendToBackend(String uid) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[BACKEND] WiFi não conectado!");
    return;
  }

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
    Serial.printf("[BACKEND] (%d) %s\n",
                  httpResponseCode,
                  http.getString().c_str());
  } else {
    Serial.printf("[BACKEND ERRO] %s\n",
                  http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}

// ============================================================
// SETUP
// ============================================================
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
    Serial.println("\nWiFi conectado!");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("\nFalha ao conectar ao WiFi!");
  }

  SinricProSwitch &mySwitch = SinricPro[DEVICE_ID];
  mySwitch.onPowerState(onPowerState);
  SinricPro.begin(APP_KEY, APP_SECRET);

  SPI.begin(SCK, MISO, MOSI, SS_PIN);
  mfrc522.PCD_Init();

  Serial.println("Aproxime o cartão RFID...");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
}

// ============================================================
// LOOP PRINCIPAL
// ============================================================
void loop() {
  SinricPro.handle();

  unsigned long now = millis();

  // Buzzer alternando
  if (!buzzerState && now - buzzerLast >= buzzerOffTime) {
    digitalWrite(BUZZER_PIN, HIGH);
    buzzerState = true;
    buzzerLast = now;
  } else if (buzzerState && now - buzzerLast >= buzzerOnTime) {
    digitalWrite(BUZZER_PIN, LOW);
    buzzerState = false;
    buzzerLast = now;
  }

  // RFID
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {

    String uidStr = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      if (mfrc522.uid.uidByte[i] < 0x10) uidStr += "0";
      uidStr += String(mfrc522.uid.uidByte[i], HEX);
    }
    uidStr.toUpperCase();

    if (!validateUid(uidStr)) {
      Serial.println("[ERRO] UID inválido detectado! Ignorando...");
      return;
    }

    if (uidStr != lastUid || (now - lastTrigger) > triggerDebounceMs) {
      Serial.print("[RFID] TAG: ");
      Serial.println(uidStr);

      lastUid = uidStr;
      lastTrigger = now;

      // Envia evento para SinricPro
      SinricProSwitch &sw = SinricPro[DEVICE_ID];
      if (sw.sendPowerStateEvent(true))
        Serial.println("[SINRIC] Evento enviado!");

      // Envia ao backend
      sendToBackend(uidStr);

      // LED por 10s
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ON (10s)");
      delay(10000);
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED OFF");
    }

    mfrc522.PICC_HaltA();
    mfrc522.PCD_StopCrypto1();
  }

  delay(10);
}
