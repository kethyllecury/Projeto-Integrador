#ifndef CONFIG_H
#define CONFIG_H

// =======================
// WIFI
// =======================
const char* ssid = "SSID";
const char* password = "SENHA";

// =======================
// BACKEND
// =======================
const char* backendURL = "https://projeto-integrador-f86b.onrender.com/api/rfid";
const char* apiKey = "API_KEY";

// =======================
// SINAIS SINA
// =======================
const char* APP_KEY = "APP_KEY";
const char* APP_SECRET = "APP_SECRET";
const char* DEVICE_ID = "DEVICE_ID";

// =======================
// RFID PINS
// =======================
#define SS_PIN 7
#define RST_PIN 10
#define MOSI 6
#define MISO 5
#define SCK 4

// =======================
// SINAIS
// =======================
#define LED_PIN 2
#define BUZZER_PIN 3

#endif
