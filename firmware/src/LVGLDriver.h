/*
 * LVGL Display Driver for ESP32-C3 + GC9A01
 */

#ifndef LVGL_DRIVER_H
#define LVGL_DRIVER_H

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

class LVGLDriver {
public:
  LVGLDriver();
  void init();
  void tick();
  
private:
  TFT_eSPI tft;
  static void displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);
  static LVGLDriver* instance;
};

#endif
