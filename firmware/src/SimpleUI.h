/*
 * Simple LVGL UI for Knomi Clone
 */

#ifndef SIMPLE_UI_H
#define SIMPLE_UI_H

#include <Arduino.h>
#include <lvgl.h>

class SimpleUI {
public:
  SimpleUI();
  void init();
  void showBootScreen();
  void showConnecting();
  void showConnected();
  void showKlipperData(float hotendTemp, float hotendTarget, float bedTemp, float bedTarget, int progress);
  void showOffline();
  
private:
  lv_obj_t* screen;
  lv_obj_t* statusLabel;
  lv_obj_t* tempLabel;
  lv_obj_t* progressLabel;
};

#endif
