#include "BackendManager.h"
#include "Config.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <WiFi.h>

void BackendManager::sendUID(String uid) {
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
        Serial.printf("[BACKEND] (%d) %s\n", httpResponseCode, http.getString().c_str());
    } else {
        Serial.printf("[BACKEND ERRO] %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();
}
