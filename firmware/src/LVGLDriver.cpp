/*
 * LVGL Display Driver Implementation
 */

#include "LVGLDriver.h"

LVGLDriver* LVGLDriver::instance = nullptr;

LVGLDriver::LVGLDriver() {
  instance = this;
}

void LVGLDriver::init() {
  // Initialize TFT
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);  // Backlight on
  
  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  
  // Initialize LVGL
  lv_init();
  
  // Create display buffer
  static lv_disp_draw_buf_t draw_buf;
  static lv_color_t buf[240 * 10];  // 10 lines buffer
  lv_disp_draw_buf_init(&draw_buf, buf, NULL, 240 * 10);
  
  // Initialize display driver
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);
  disp_drv.hor_res = 240;
  disp_drv.ver_res = 240;
  disp_drv.flush_cb = displayFlush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);
  
  Serial.println("LVGL initialized!");
}

void LVGLDriver::displayFlush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p) {
  if (instance) {
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);
    
    instance->tft.startWrite();
    instance->tft.setAddrWindow(area->x1, area->y1, w, h);
    instance->tft.pushColors((uint16_t *)&color_p->full, w * h, true);
    instance->tft.endWrite();
  }
  
  lv_disp_flush_ready(disp);
}

void LVGLDriver::tick() {
  lv_timer_handler();  // Handle LVGL tasks
}
