/*
 * Touch Driver Implementation
 *
 * Supports FT6236 capacitive touch controller
 * Provides touch detection, zones, and gesture recognition
 */

#include "TouchDriver.h"
#include <Wire.h>

// FT6236 I2C registers
#define FT6236_ADDR 0x38
#define FT6236_REG_NUMTOUCHES 0x02
#define FT6236_REG_TD_STATUS 0x02
#define FT6236_REG_P1_XH 0x03
#define FT6236_REG_P1_XL 0x04
#define FT6236_REG_P1_YH 0x05
#define FT6236_REG_P1_YL 0x06
#define FT6236_REG_P1_WEIGHT 0x07
#define FT6236_REG_P1_MISC 0x08

// FT6236 touch point structure
struct FT6236_TouchPoint {
  uint16_t x;
  uint16_t y;
  uint8_t weight;
  uint8_t misc;
};

static FT6236_TouchPoint ft6236_touchPoint;

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
  
  // Test I2C communication with FT6236
  Wire.beginTransmission(FT6236_ADDR);
  if (Wire.endTransmission() != 0) {
    Serial.println("Touch: FT6236 not found at 0x38");
    return false;
  }
  
  Serial.println("Touch: FT6236 found at 0x38");
  return true;
}

void TouchDriver::update() {
  // Read touch data from FT6236 via I2C
  Wire.beginTransmission(FT6236_ADDR);
  Wire.write(FT6236_REG_TD_STATUS);
  if (Wire.endTransmission(false) != 0) {
    return; // I2C error
  }
  
  if (Wire.requestFrom(FT6236_ADDR, 6) != 6) {
    return; // Not enough data
  }
  
  uint8_t touchStatus = Wire.read();
  uint8_t numTouches = touchStatus & 0x0F; // Lower 4 bits
  
  if (numTouches > 0) {
    // Read touch point data
    uint8_t xh = Wire.read();
    uint8_t xl = Wire.read();
    uint8_t yh = Wire.read();
    uint8_t yl = Wire.read();
    uint8_t weight = Wire.read();
    uint8_t misc = Wire.read();
    
    // Extract coordinates (12-bit values)
    uint16_t x = ((xh & 0x0F) << 8) | xl;
    uint16_t y = ((yh & 0x0F) << 8) | yl;
    
    // Check if touch is detected (coordinates not zero)
    if (x != 0 || y != 0) {
      // Touch detected
      currentPoint.x = x;
      currentPoint.y = y;
      currentPoint.pressure = weight; // Use weight as pressure
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
  } else {
    // No touches detected
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
  Serial.printf("Touch: Event=%d, Point=(%d,%d), Valid=%d\n",
                currentEvent, currentPoint.x, currentPoint.y, currentPoint.valid);
}
