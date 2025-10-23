/*
 * Touch Driver for Capacitive Touch Displays
 *
 * Supports FT6236 and other capacitive touch controllers
 * Provides touch event detection, coordinate mapping, and gesture recognition
 */

#ifndef TOUCH_DRIVER_H
#define TOUCH_DRIVER_H

#include <Arduino.h>

// Touch event types
enum TouchEvent {
  TOUCH_NONE,
  TOUCH_DOWN,
  TOUCH_UP,
  TOUCH_MOVE,
  TOUCH_GESTURE_TAP,
  TOUCH_GESTURE_DOUBLE_TAP,
  TOUCH_GESTURE_SWIPE_UP,
  TOUCH_GESTURE_SWIPE_DOWN,
  TOUCH_GESTURE_SWIPE_LEFT,
  TOUCH_GESTURE_SWIPE_RIGHT
};

// Touch point structure
struct TouchPoint {
  int16_t x;
  int16_t y;
  uint16_t pressure;
  bool valid;
};

// Touch zone for UI elements
struct TouchZone {
  int16_t x, y;           // Top-left corner
  int16_t width, height;  // Size
  uint8_t id;             // Zone identifier
  bool active;            // Zone is active
};

class TouchDriver {
public:
  TouchDriver();
  
  // Initialize touch controller
  bool begin(uint8_t sda = 18, uint8_t scl = 19, uint8_t rst = -1, uint8_t intPin = -1);
  
  // Update touch state (call in main loop)
  void update();
  
  // Get current touch event
  TouchEvent getEvent();
  
  // Get current touch point
  TouchPoint getPoint();
  
  // Check if touch is detected
  bool isTouched();
  
  // Define touch zones for UI
  void addZone(uint8_t id, int16_t x, int16_t y, int16_t width, int16_t height);
  void removeZone(uint8_t id);
  void clearZones();
  
  // Check if touch is within a zone
  uint8_t getTouchedZone();
  
  // Enable/disable gesture recognition
  void enableGestures(bool enable);
  
  // Get gesture threshold settings
  void setSwipeThreshold(uint16_t threshold);
  void setTapThreshold(uint16_t threshold);
  
  // Debug functions
  void printTouchInfo();

private:
  // Touch controller instance
  void* touchController;  // Generic pointer for different controllers
  
  // Touch state
  TouchEvent currentEvent;
  TouchPoint currentPoint;
  TouchPoint lastPoint;
  unsigned long touchStartTime;
  unsigned long lastTouchTime;
  
  // Gesture recognition
  bool gesturesEnabled;
  uint16_t swipeThreshold;
  uint16_t tapThreshold;
  bool gestureInProgress;
  
  // Touch zones
  static const uint8_t MAX_ZONES = 10;
  TouchZone zones[MAX_ZONES];
  uint8_t zoneCount;
  
  // Private methods
  bool initFT6236(uint8_t sda, uint8_t scl, uint8_t rst, uint8_t intPin);
  void processGestures();
  TouchEvent detectGesture();
  bool isPointInZone(TouchPoint point, TouchZone zone);
};

#endif // TOUCH_DRIVER_H
