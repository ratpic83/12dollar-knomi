/*
 * Touch Driver Implementation
 *
 * Supports FT6236 capacitive touch controller
 * Provides touch detection, zones, and gesture recognition
 */

#include "TouchDriver.h"
#include <Wire.h>
#include <Adafruit_FT6236.h>

// Touch controller instance
Adafruit_FT6236* ft6236 = nullptr;

TouchDriver::TouchDriver() :
  touchController(nullptr),
  currentEvent(TOUCH_NONE),
  gesturesEnabled(true),
  swipeThreshold(50),
  tapThreshold(20),
  gestureInProgress(false),
  zoneCount(0)
{
  currentPoint.x = 0;
  currentPoint.y = 0;
  currentPoint.pressure = 0;
  currentPoint.valid = false;
  
  lastPoint = currentPoint;
  touchStartTime = 0;
  lastTouchTime = 0;
}

bool TouchDriver::begin(uint8_t sda, uint8_t scl, uint8_t rst, uint8_t intPin) {
  // Try FT6236 first (most common for round displays)
  if (initFT6236(sda, scl, rst, intPin)) {
    Serial.println("Touch: FT6236 initialized");
    return true;
  }
  
  Serial.println("Touch: No supported controller found");
  return false;
}

bool TouchDriver::initFT6236(uint8_t sda, uint8_t scl, uint8_t rst, uint8_t intPin) {
  // Initialize I2C if not already done
  if (!Wire.begin(sda, scl)) {
    Serial.println("Touch: Failed to initialize I2C");
    return false;
  }
  
  ft6236 = new Adafruit_FT6236();
  
  // Try different I2C addresses
  if (ft6236->begin(0x38)) {
    Serial.println("Touch: FT6236 found at 0x38");
    return true;
  }
  
  if (ft6236->begin(0x39)) {
    Serial.println("Touch: FT6236 found at 0x39");
    return true;
  }
  
  delete ft6236;
  ft6236 = nullptr;
  return false;
}

void TouchDriver::update() {
  if (!ft6236) return;
  
  // Get touch points from FT6236
  TS_Point point = ft6236->getPoint();
  
  // Check if touch is detected
  if (point.x != 0 || point.y != 0) {
    // Touch detected
    currentPoint.x = point.x;
    currentPoint.y = point.y;
    currentPoint.pressure = point.z;
    currentPoint.valid = true;
    
    // Handle touch down
    if (!lastPoint.valid) {
      currentEvent = TOUCH_DOWN;
      touchStartTime = millis();
      gestureInProgress = true;
    } else {
      // Check if it's a significant move
      int16_t deltaX = abs(currentPoint.x - lastPoint.x);
      int16_t deltaY = abs(currentPoint.y - lastPoint.y);
      
      if (deltaX > 5 || deltaY > 5) {
        currentEvent = TOUCH_MOVE;
      } else {
        currentEvent = TOUCH_NONE; // Minor movement, no event
      }
    }
    
    lastPoint = currentPoint;
    lastTouchTime = millis();
  } else {
    // No touch detected
    if (lastPoint.valid) {
      // Touch was just released
      if (gesturesEnabled) {
        currentEvent = detectGesture();
      } else {
        currentEvent = TOUCH_UP;
      }
      
      gestureInProgress = false;
    } else {
      currentEvent = TOUCH_NONE;
    }
    
    currentPoint.valid = false;
  }
}

TouchEvent TouchDriver::getEvent() {
  TouchEvent event = currentEvent;
  // Reset event after reading (one-shot)
  if (event != TOUCH_NONE) {
    currentEvent = TOUCH_NONE;
  }
  return event;
}

TouchPoint TouchDriver::getPoint() {
  return currentPoint;
}

bool TouchDriver::isTouched() {
  return currentPoint.valid;
}

void TouchDriver::addZone(uint8_t id, int16_t x, int16_t y, int16_t width, int16_t height) {
  if (zoneCount >= MAX_ZONES) return;
  
  zones[zoneCount].id = id;
  zones[zoneCount].x = x;
  zones[zoneCount].y = y;
  zones[zoneCount].width = width;
  zones[zoneCount].height = height;
  zones[zoneCount].active = true;
  
  zoneCount++;
}

void TouchDriver::removeZone(uint8_t id) {
  for (uint8_t i = 0; i < zoneCount; i++) {
    if (zones[i].id == id) {
      // Shift remaining zones
      for (uint8_t j = i; j < zoneCount - 1; j++) {
        zones[j] = zones[j + 1];
      }
      zoneCount--;
      break;
    }
  }
}

void TouchDriver::clearZones() {
  zoneCount = 0;
}

uint8_t TouchDriver::getTouchedZone() {
  if (!currentPoint.valid) return 0;
  
  for (uint8_t i = 0; i < zoneCount; i++) {
    if (zones[i].active && isPointInZone(currentPoint, zones[i])) {
      return zones[i].id;
    }
  }
  
  return 0; // No zone touched
}

void TouchDriver::enableGestures(bool enable) {
  gesturesEnabled = enable;
}

void TouchDriver::setSwipeThreshold(uint16_t threshold) {
  swipeThreshold = threshold;
}

void TouchDriver::setTapThreshold(uint16_t threshold) {
  tapThreshold = threshold;
}

TouchEvent TouchDriver::detectGesture() {
  if (!gesturesEnabled || touchStartTime == 0) {
    return TOUCH_UP;
  }
  
  unsigned long touchDuration = lastTouchTime - touchStartTime;
  int16_t deltaX = currentPoint.x - lastPoint.x;
  int16_t deltaY = currentPoint.y - lastPoint.y;
  
  // Check for tap (short duration, small movement)
  if (touchDuration < 500 && abs(deltaX) < tapThreshold && abs(deltaY) < tapThreshold) {
    // Could implement double-tap detection here
    return TOUCH_GESTURE_TAP;
  }
  
  // Check for swipe (significant movement)
  if (abs(deltaX) > swipeThreshold || abs(deltaY) > swipeThreshold) {
    if (abs(deltaX) > abs(deltaY)) {
      // Horizontal swipe
      return (deltaX > 0) ? TOUCH_GESTURE_SWIPE_RIGHT : TOUCH_GESTURE_SWIPE_LEFT;
    } else {
      // Vertical swipe
      return (deltaY > 0) ? TOUCH_GESTURE_SWIPE_DOWN : TOUCH_GESTURE_SWIPE_UP;
    }
  }
  
  return TOUCH_UP;
}

bool TouchDriver::isPointInZone(TouchPoint point, TouchZone zone) {
  return (point.x >= zone.x && point.x < zone.x + zone.width &&
          point.y >= zone.y && point.y < zone.y + zone.height);
}

void TouchDriver::printTouchInfo() {
  if (!ft6236) {
    Serial.println("Touch: No controller initialized");
    return;
  }
  
  Serial.printf("Touch: Event=%d, Point=(%d,%d), Valid=%d\n",
                currentEvent, currentPoint.x, currentPoint.y, currentPoint.valid);
}
