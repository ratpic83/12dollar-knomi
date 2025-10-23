/*
 * Simple LVGL UI Implementation
 */

#include "SimpleUI.h"

SimpleUI::SimpleUI() {
  screen = nullptr;
  statusLabel = nullptr;
  tempLabel = nullptr;
  progressLabel = nullptr;
}

void SimpleUI::init() {
  // Create main screen
  screen = lv_obj_create(NULL);
  lv_scr_load(screen);
  
  // Create status label (top)
  statusLabel = lv_label_create(screen);
  lv_obj_set_style_text_color(statusLabel, lv_color_hex(0xFFFFFF), 0);
  lv_obj_set_style_text_align(statusLabel, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(statusLabel, LV_ALIGN_TOP_MID, 0, 40);
  lv_label_set_text(statusLabel, "Knomi Clone");
  
  // Create temperature label (middle)
  tempLabel = lv_label_create(screen);
  lv_obj_set_style_text_color(tempLabel, lv_color_hex(0x00FF00), 0);
  lv_obj_set_style_text_align(tempLabel, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(tempLabel, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(tempLabel, "---");
  
  // Create progress label (bottom)
  progressLabel = lv_label_create(screen);
  lv_obj_set_style_text_color(progressLabel, lv_color_hex(0xFFFF00), 0);
  lv_obj_set_style_text_align(progressLabel, LV_TEXT_ALIGN_CENTER, 0);
  lv_obj_align(progressLabel, LV_ALIGN_BOTTOM_MID, 0, -40);
  lv_label_set_text(progressLabel, "");
  
  Serial.println("Simple UI initialized");
}

void SimpleUI::showBootScreen() {
  lv_label_set_text(statusLabel, "Knomi Clone");
  lv_label_set_text(tempLabel, "Booting...");
  lv_label_set_text(progressLabel, "v1.0");
}

void SimpleUI::showConnecting() {
  lv_label_set_text(statusLabel, "WiFi");
  lv_label_set_text(tempLabel, "Connecting...");
  lv_label_set_text(progressLabel, "");
}

void SimpleUI::showConnected() {
  lv_label_set_text(statusLabel, "WiFi");
  lv_label_set_text(tempLabel, "Connected!");
  lv_label_set_text(progressLabel, "");
}

void SimpleUI::showKlipperData(float hotendTemp, float hotendTarget, float bedTemp, float bedTarget, int progress) {
  // Update status
  lv_label_set_text(statusLabel, "Klipper");
  
  // Update temperatures
  char tempStr[64];
  snprintf(tempStr, sizeof(tempStr), "E: %.1f/%.1f\nB: %.1f/%.1f", 
           hotendTemp, hotendTarget, bedTemp, bedTarget);
  lv_label_set_text(tempLabel, tempStr);
  
  // Update progress
  if (progress > 0) {
    char progStr[16];
    snprintf(progStr, sizeof(progStr), "%d%%", progress);
    lv_label_set_text(progressLabel, progStr);
  } else {
    lv_label_set_text(progressLabel, "Ready");
  }
}

void SimpleUI::showOffline() {
  lv_label_set_text(statusLabel, "Klipper");
  lv_label_set_text(tempLabel, "Offline");
  lv_label_set_text(progressLabel, "");
}
