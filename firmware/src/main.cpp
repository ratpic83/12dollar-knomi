#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>
#include "splash_gif.h"

// WiFi Config
const char* WIFI_SSID = "azazel";
const char* WIFI_PASSWORD = "fritzipupa";
const char* KLIPPER_IP = "192.168.68.91";
const int KLIPPER_PORT = 7125;

// Klipper data
float hotendTemp = 0;
float hotendTarget = 0;
float bedTemp = 0;
float bedTarget = 0;
String printerState = "standby";

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  // Init display
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  // Animate splash GIF
  for (int loop = 0; loop < 5; loop++) {  // Play 5 times
    for (int frame = 0; frame < SPLASH_FRAME_COUNT; frame++) {
      tft.pushImage(0, 0, 240, 240, splash_frames[frame]);
      delay(300);  // 300ms per frame = smoother
    }
  }
  delay(500);

  // Connect WiFi
  Serial.println("Connecting WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi connected!");
    Serial.println(WiFi.localIP());
  }
}

unsigned long lastUpdate = 0;
int animFrame = 0;

void loop() {
  // Update Klipper data every 2 seconds
  if (millis() - lastUpdate > 2000) {
    lastUpdate = millis();

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://" + String(KLIPPER_IP) + ":" + String(KLIPPER_PORT) + "/printer/objects/query?extruder&heater_bed";

      http.begin(url);
      http.setTimeout(10000);

      int code = http.GET();
      if (code == 200) {
        String payload = http.getString();
        StaticJsonDocument<1024> doc;

        if (deserializeJson(doc, payload) == DeserializationError::Ok) {
          hotendTemp = doc["result"]["status"]["extruder"]["temperature"] | 0.0;
          hotendTarget = doc["result"]["status"]["extruder"]["target"] | 0.0;
          bedTemp = doc["result"]["status"]["heater_bed"]["temperature"] | 0.0;
          bedTarget = doc["result"]["status"]["heater_bed"]["target"] | 0.0;
        }
      }
      http.end();
    }
    
    // Update display
    tft.fillScreen(TFT_BLACK);
    
    // Title
    tft.setTextColor(TFT_CYAN);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("KNOMI CLONE", 120, 30, 4);
    
    // Hotend
    tft.setTextColor(TFT_ORANGE);
    tft.setTextDatum(TL_DATUM);
    tft.drawString("Hotend", 30, 80, 2);
    String hotendStr = String(hotendTemp, 1) + "/" + String((int)hotendTarget) + "C";
    tft.setTextColor(TFT_WHITE);
    tft.drawString(hotendStr, 30, 100, 4);
    
    // Bed
    tft.setTextColor(TFT_RED);
    tft.drawString("Bed", 30, 150, 2);
    String bedStr = String(bedTemp, 1) + "/" + String((int)bedTarget) + "C";
    tft.setTextColor(TFT_WHITE);
    tft.drawString(bedStr, 30, 170, 4);
    
    // Status
    tft.setTextColor(TFT_GREEN);
    tft.setTextDatum(BC_DATUM);
    tft.drawString("READY", 120, 230, 2);
  }
  
  delay(10);
}
