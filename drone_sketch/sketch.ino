#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 17  
const char* ssid = "Wokwi-GUEST";
const char* password = "";
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

const char* scriptURL = "https://script.google.com/macros/s/AKfycbz6_ToPya23bwhmR1-lMcv702KDXT3bDSWpWOsJ8pK5He3VTB_Noezrat0tvb_fexIOHA/exec";
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  dht.begin();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi Connected");
}

void loop() {
    float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(scriptURL);

    // WAJIB pakai form encoding
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Simulasi data sensor (0–100)
    int kualitas_air   = f;
    int kualitas_udara = h;
    int kualitas_tanah = t;
    int daya_listrik   = 0;  // 0–220V

    // Buat JSON sesuai Apps Script
    String json = "{";
    json += "\"kualitas_air\":" + String(kualitas_air) + ",";
    json += "\"kualitas_udara\":" + String(kualitas_udara) + ",";
    json += "\"kualitas_tanah\":" + String(kualitas_tanah) + ",";
    json += "\"daya_listrik\":" + String(daya_listrik);
    json += "}";

    // Bungkus ke parameter data=
    String postData = "data=" + json;

    Serial.println("Sending:");
    Serial.println(postData);

    int httpResponseCode = http.POST(postData);

    Serial.print("HTTP Response: ");
    Serial.println(httpResponseCode);

    String response = http.getString();
    Serial.println("Server says: " + response);

    http.end();
  }

  delay(10000); // 10 detik
}