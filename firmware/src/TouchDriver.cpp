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

// CST816D I2C registers (used in ESP32-2424S012C)
#define CST816D_ADDR 0x15
#define CST816D_REG_GESTURE 0x01
#define CST816D_REG_POINTS 0x02
#define CST816D_REG_XH 0x03
#define CST816D_REG_XL 0x04
#define CST816D_REG_YH 0x05
#define CST816D_REG_YL 0x06

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
  touchStartTime(0),
  lastTouchTime(0),
  gesturesEnabled(true),
  swipeThreshold(50),
  tapThreshold(20),
  gestureInProgress(false),
  circleQuadrants(0)
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
  // Initialize I2C
  Wire.begin(sda, scl);
  delay(50);
  
  // Try CST816D first (used in ESP32-2424S012C)
  Wire.beginTransmission(CST816D_ADDR);
  if (Wire.endTransmission() == 0) {
    Serial.println("Touch: CST816D found at 0x15");
    touchController = (void*)CST816D_ADDR;
    return true;
  }
  
  // Try FT6236 as fallback
  Wire.beginTransmission(FT6236_ADDR);
  if (Wire.endTransmission() == 0) {
    Serial.println("Touch: FT6236 found at 0x38");
    touchController = (void*)FT6236_ADDR;
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
  if (!touchController) return;
  
  uint8_t addr = (uint8_t)(uintptr_t)touchController;
  uint8_t reg = (addr == CST816D_ADDR) ? CST816D_REG_POINTS : FT6236_REG_TD_STATUS;
  
  // Read touch data via I2C
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) {
    return; // I2C error
  }
  
  if (Wire.requestFrom(addr, (uint8_t)6) != 6) {
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
        touchStartPoint = currentPoint;  // Store start position for swipe detection
        circleQuadrants = 0;  // Reset circle tracking
        gestureInProgress = true;
      } else {
        // Check if it's a significant move
        int16_t deltaX = abs(currentPoint.x - lastPoint.x);
        int16_t deltaY = abs(currentPoint.y - lastPoint.y);
        
        if (deltaX > 5 || deltaY > 5) {
          currentEvent = TOUCH_MOVE;
          
          // Track quadrants for circle gesture (relative to screen center)
          int16_t centerX = 120;  // Screen center
          int16_t centerY = 120;
          int16_t relX = currentPoint.x - centerX;
          int16_t relY = currentPoint.y - centerY;
          
          // Determine quadrant (0=top-right, 1=top-left, 2=bottom-left, 3=bottom-right)
          uint8_t quadrant = 0;
          if (relX >= 0 && relY < 0) quadrant = 0;  // Top-right
          else if (relX < 0 && relY < 0) quadrant = 1;  // Top-left
          else if (relX < 0 && relY >= 0) quadrant = 2;  // Bottom-left
          else if (relX >= 0 && relY >= 0) quadrant = 3;  // Bottom-right
          
          circleQuadrants |= (1 << quadrant);  // Mark this quadrant as visited
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
      Serial.println("Touch released!");
      if (gesturesEnabled) {
        currentEvent = detectGesture();
        Serial.printf("Gesture event: %d\n", currentEvent);
      } else {
        currentEvent = TOUCH_UP;
      }
      
      gestureInProgress = false;
    } else {
      currentEvent = TOUCH_NONE;
    }
    
    currentPoint.valid = false;
    lastPoint.valid = false;
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
  
  unsigned long touchDuration = millis() - touchStartTime;
  
  // Calculate total movement from start to current position
  int16_t deltaX = currentPoint.x - touchStartPoint.x;
  int16_t deltaY = currentPoint.y - touchStartPoint.y;
  
  Serial.printf("Gesture: duration=%lu, deltaX=%d, deltaY=%d, quadrants=0x%X\n", touchDuration, deltaX, deltaY, circleQuadrants);
  
  // Check for circle gesture (all 4 quadrants visited)
  if (circleQuadrants == 0x0F) {  // 0x0F = 0b1111 = all 4 quadrants
    Serial.println("Circle gesture detected!");
    return TOUCH_GESTURE_CIRCLE;
  }
  
  // Check for swipe (significant movement)
  if (abs(deltaX) > swipeThreshold || abs(deltaY) > swipeThreshold) {
    if (abs(deltaY) > abs(deltaX)) {
      // Vertical swipe
      TouchEvent result = (deltaY > 0) ? TOUCH_GESTURE_SWIPE_DOWN : TOUCH_GESTURE_SWIPE_UP;
      Serial.printf("Swipe detected: %s\n", (result == TOUCH_GESTURE_SWIPE_UP) ? "UP" : "DOWN");
      return result;
    } else {
      // Horizontal swipe
      TouchEvent result = (deltaX > 0) ? TOUCH_GESTURE_SWIPE_RIGHT : TOUCH_GESTURE_SWIPE_LEFT;
      Serial.printf("Swipe detected: %s\n", (result == TOUCH_GESTURE_SWIPE_LEFT) ? "LEFT" : "RIGHT");
      return result;
    }
  }
  
  // Check for tap (short duration, small movement)
  if (touchDuration < 500) {
    Serial.printf("Tap detected! Duration: %lu ms\n", touchDuration);
    return TOUCH_GESTURE_TAP;
  }
  
  return TOUCH_UP;
}

void TouchDriver::printTouchInfo() {
  Serial.printf("Touch: Event=%d, Point=(%d,%d), Valid=%d\n",
                currentEvent, currentPoint.x, currentPoint.y, currentPoint.valid);
}
