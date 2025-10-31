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
  TOUCH_GESTURE_SWIPE_RIGHT,
  TOUCH_GESTURE_CIRCLE
};

// Touch point structure
struct TouchPoint {
  int16_t x;
  int16_t y;
  uint16_t pressure;
  bool valid;
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
  TouchPoint touchStartPoint;  // Store where touch began for swipe detection
  unsigned long touchStartTime;
  unsigned long lastTouchTime;
  
  // Gesture recognition
  bool gesturesEnabled;
  uint16_t swipeThreshold;
  uint16_t tapThreshold;
  bool gestureInProgress;
  
  // Circle gesture tracking
  uint8_t circleQuadrants;  // Bitmask of visited quadrants (bit 0-3 for quadrants 1-4)
  
  // Private methods
  bool initFT6236(uint8_t sda, uint8_t scl, uint8_t rst, uint8_t intPin);
  void processGestures();
  TouchEvent detectGesture();
};

#endif // TOUCH_DRIVER_H
