/*
 * UI Manager Implementation
 */

#include "UIManager.h"
#include "WifiConfig.h"

UIManager::UIManager() : 
  display(nullptr),
  currentScreen(SCREEN_BOOT),
  lastAnimationUpdate(0),
  animationFrame(0) {
}

void UIManager::init(DisplayDriver* disp) {
  display = disp;
  currentScreen = SCREEN_BOOT;
  animationFrame = 0;
}

void UIManager::showBootScreen() {
  currentScreen = SCREEN_BOOT;
  display->clear();
  
  // Draw logo/title
  display->setTextColor(display->getThemeColors().accent);
  display->drawCenteredText("KNOMI", 80, 3);
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText("Clone", 110, 2);
  
  // Draw version
  display->setTextColor(display->getThemeColors().secondary);
  display->drawCenteredText("v1.1", 140, 1);
  
  // Draw loading indicator
  display->drawCircle(SCREEN_WIDTH/2, 180, 10, display->getThemeColors().accent);
}

void UIManager::showConnectingScreen() {
  currentScreen = SCREEN_CONNECTING;
  display->clear();
  
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText("Connecting", 100, 2);
  display->setTextColor(display->getThemeColors().secondary);
  display->drawCenteredText("to WiFi...", 130, 1);
  
  // Draw WiFi icon
  display->drawWiFiIcon(SCREEN_WIDTH/2, 170, display->getThemeColors().accent, 1);
}

void UIManager::showConnectedScreen() {
  currentScreen = SCREEN_CONNECTED;
  display->clear();
  
  display->setTextColor(display->getThemeColors().success);
  display->drawCenteredText("Connected!", 110, 2);
  
  // Draw checkmark
  display->drawCheckIcon(SCREEN_WIDTH/2, 160, display->getThemeColors().success);
}

void UIManager::showWiFiError() {
  if (currentScreen == SCREEN_WIFI_ERROR) return;
  
  currentScreen = SCREEN_WIFI_ERROR;
  display->clear();
  
  display->setTextColor(display->getThemeColors().error);
  display->drawCenteredText("WiFi Error", 100, 2);
  display->setTextColor(display->getThemeColors().secondary);
  display->drawCenteredText("Check config", 130, 1);
  
  // Draw error icon
  display->drawErrorIcon(SCREEN_WIDTH/2, 170, display->getThemeColors().error);
}

void UIManager::showKlipperError() {
  if (currentScreen == SCREEN_KLIPPER_ERROR) return;
  
  currentScreen = SCREEN_KLIPPER_ERROR;
  display->clear();
  
  display->setTextColor(display->getThemeColors().error);
  display->drawCenteredText("Klipper", 90, 2);
  display->drawCenteredText("Offline", 120, 2);
  display->setTextColor(display->getThemeColors().secondary);
  display->drawCenteredText("Reconnecting...", 150, 1);
  
  // Draw printer icon
  display->drawPrinterIcon(SCREEN_WIDTH/2 - 15, 175, display->getThemeColors().error);
}

void UIManager::updateStatus(PrinterStatus& status) {
  // Determine which screen to show based on state
  ScreenType newScreen = currentScreen;
  
  if (status.state == STATE_IDLE || status.state == STATE_STANDBY) {
    newScreen = SCREEN_IDLE;
  } else if (status.state == STATE_PRINTING) {
    newScreen = SCREEN_PRINTING;
  } else if (status.state == STATE_PAUSED) {
    newScreen = SCREEN_PAUSED;
  } else if (status.state == STATE_COMPLETE) {
    newScreen = SCREEN_COMPLETE;
  } else if (status.state == STATE_ERROR) {
    newScreen = SCREEN_ERROR;
  }
  
  // Redraw if screen changed or significant data changed
  if (newScreen != currentScreen || shouldRedraw(status)) {
    currentScreen = newScreen;
    
    switch (currentScreen) {
      case SCREEN_IDLE:
        drawIdleScreen(status);
        break;
      case SCREEN_PRINTING:
        drawPrintingScreen(status);
        break;
      case SCREEN_PAUSED:
        drawPausedScreen(status);
        break;
      case SCREEN_COMPLETE:
        drawCompleteScreen(status);
        break;
      case SCREEN_ERROR:
        drawErrorScreen();
        break;
      default:
        break;
    }
  }
  
  // Store last status for comparison
  lastStatus = status;
}

void UIManager::drawIdleScreen(PrinterStatus& status) {
  display->clear();
  
  // Draw rolling eyes animation
  display->drawRollingEyes(animationFrame);
  
  // Draw status text at bottom
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText("Ready", 210, 2);
  
  // Draw temperatures
  char tempStr[32];
  sprintf(tempStr, "%.0f/%.0f", status.hotendTemp, status.hotendTarget);
  display->setTextColor(display->getThemeColors().highlight);
  display->drawCenteredText(tempStr, 230, 1);
  
  // Draw environmental data if available
  if (status.chamberTemp > 0 || status.chamberHumidity > 0) {
    display->setTextColor(display->getThemeColors().secondary);
    char envStr[32];
    if (status.chamberHumidity > 0) {
      sprintf(envStr, "%.1f°C %.0f%%", status.chamberTemp, status.chamberHumidity);
    } else {
      sprintf(envStr, "%.1f°C", status.chamberTemp);
    }
    display->drawCenteredText(envStr, 190, 1);
  }
}

void UIManager::drawPrintingScreen(PrinterStatus& status) {
  display->clear();
  
  // Draw progress ring with enhanced visuals
  drawProgressCircle(status.printProgress);
  
  // Draw progress percentage in center
  char progressStr[8];
  sprintf(progressStr, "%d%%", status.printProgress);
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText(progressStr, 110, 3);
  
  // Draw temperature gauges in corners
  drawTemperatureGauges(status);
  
  // Draw time remaining at top
  if (status.printTimeLeft > 0) {
    display->setTextColor(display->getThemeColors().secondary);
    String timeStr = formatTime(status.printTimeLeft);
    display->drawCenteredText("ETA: " + timeStr, 30, 1);
  }
  
  // Draw filename (truncated)
  if (status.fileName.length() > 0) {
    display->setTextColor(display->getThemeColors().accent);
    String shortName = status.fileName;
    if (shortName.length() > 20) {
      shortName = shortName.substring(0, 17) + "...";
    }
    display->drawCenteredText(shortName, 200, 1);
  }
  
  // Draw Z height at bottom
  display->setTextColor(display->getThemeColors().secondary);
  char zStr[16];
  sprintf(zStr, "Z:%.2f", status.posZ);
  display->drawCenteredText(zStr, 225, 1);
}

void UIManager::drawPausedScreen(PrinterStatus& status) {
  display->clear();
  
  // Draw progress ring (dimmed)
  drawProgressCircle(status.printProgress);
  
  // Draw PAUSED text
  display->setTextColor(display->getThemeColors().warning);
  display->drawCenteredText("PAUSED", 110, 2);
  
  // Draw progress percentage
  char progressStr[8];
  sprintf(progressStr, "%d%%", status.printProgress);
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText(progressStr, 140, 2);
  
  // Draw temperatures
  char tempStr[32];
  sprintf(tempStr, "E:%.0f B:%.0f", status.hotendTemp, status.bedTemp);
  display->setTextColor(display->getThemeColors().highlight);
  display->drawCenteredText(tempStr, 170, 1);
}

void UIManager::drawCompleteScreen(PrinterStatus& status) {
  display->clear();
  
  // Draw checkmark
  display->drawCheckIcon(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, display->getThemeColors().success);
  
  // Draw COMPLETE text
  display->setTextColor(display->getThemeColors().success);
  display->drawCenteredText("COMPLETE", 150, 2);
  
  // Draw print time
  display->setTextColor(display->getThemeColors().secondary);
  String timeStr = formatTime(status.printTime);
  display->drawCenteredText(timeStr, 180, 1);
}

void UIManager::drawErrorScreen() {
  display->clear();
  
  // Draw error icon
  display->drawErrorIcon(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, display->getThemeColors().error);
  
  // Draw ERROR text
  display->setTextColor(display->getThemeColors().error);
  display->drawCenteredText("ERROR", 150, 2);
  display->setTextColor(display->getThemeColors().secondary);
  display->drawCenteredText("Check printer", 180, 1);
}

void UIManager::drawTemperatureGauges(PrinterStatus& status) {
  // Draw hotend gauge in top-left
  display->drawTemperatureGauge(45, 45, 25, status.hotendTemp, status.hotendTarget, display->getThemeColors().highlight);
  display->setTextColor(display->getThemeColors().highlight);
  display->setTextSize(1);
  char hotendStr[8];
  sprintf(hotendStr, "%.0f", status.hotendTemp);
  display->drawCenteredText(hotendStr, 45 + 40, 1);
  
  // Draw bed gauge in top-right
  display->drawTemperatureGauge(SCREEN_WIDTH - 45, 45, 25, status.bedTemp, status.bedTarget, display->getThemeColors().text);
  display->setTextColor(display->getThemeColors().text);
  char bedStr[8];
  sprintf(bedStr, "%.0f", status.bedTemp);
  display->drawCenteredText(bedStr, SCREEN_WIDTH - 45 + 40, 1);
  
  // Draw target indicators if available
  if (status.hotendTarget > 0) {
    display->setTextColor(display->getThemeColors().secondary);
    char targetStr[8];
    sprintf(targetStr, "/%.0f", status.hotendTarget);
    display->drawCenteredText(targetStr, 45 + 50, 1);
  }
  
  if (status.bedTarget > 0) {
    display->setTextColor(display->getThemeColors().secondary);
    char targetStr[8];
    sprintf(targetStr, "/%.0f", status.bedTarget);
    display->drawCenteredText(targetStr, SCREEN_WIDTH - 45 + 50, 1);
  }
}

void UIManager::update() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastAnimationUpdate > 100) { // 10 FPS animation
    lastAnimationUpdate = currentTime;
    
    // Update animations based on current screen
    switch (currentScreen) {
      case SCREEN_IDLE:
        updateRollingEyes();
        break;
      case SCREEN_PRINTING:
      case SCREEN_PAUSED:
        updatePrintingAnimation();
        break;
      default:
        // No animation for other screens
        break;
    }
    
    animationFrame++;
  }
  
  // Handle touch feedback animation
  if (currentTime - lastTouchFeedback < TOUCH_FEEDBACK_DURATION) {
    // Show touch feedback ring
    int alpha = 255 - ((currentTime - lastTouchFeedback) * 255 / TOUCH_FEEDBACK_DURATION);
    display->drawTouchFeedbackRing(alpha);
  }
}

void UIManager::updateRollingEyes() {
  // Redraw eyes with new frame
  if (animationFrame % 5 == 0) {  // Update every 5 frames to reduce flicker
    drawIdleScreen(lastStatus);
  }
}

void UIManager::updatePrintingAnimation() {
  // Could add subtle animations during printing if desired
  // For now, keep it static to avoid distractions
}

bool UIManager::shouldRedraw(PrinterStatus& status) {
  // Check if significant values changed
  if (abs(status.hotendTemp - lastStatus.hotendTemp) > 1.0) return true;
  if (abs(status.bedTemp - lastStatus.bedTemp) > 1.0) return true;
  if (status.printProgress != lastStatus.printProgress) return true;
  if (status.state != lastStatus.state) return true;
  
  return false;
}

String UIManager::formatTime(uint32_t seconds) {
  uint32_t hours = seconds / 3600;
  uint32_t minutes = (seconds % 3600) / 60;
  uint32_t secs = seconds % 60;
  
  char timeStr[16];
  if (hours > 0) {
    sprintf(timeStr, "%dh %dm", hours, minutes);
  } else if (minutes > 0) {
    sprintf(timeStr, "%dm %ds", minutes, secs);
  } else {
    sprintf(timeStr, "%ds", secs);
  }
  
  return String(timeStr);
}

String UIManager::formatTemperature(float temp, float target) {
  char tempStr[16];
  if (target > 0) {
    sprintf(tempStr, "%.0f/%.0f", temp, target);
  } else {
    sprintf(tempStr, "%.0f", temp);
  }
  return String(tempStr);
}

// Touch feedback animation
static unsigned long lastTouchFeedback = 0;
static const unsigned long TOUCH_FEEDBACK_DURATION = 200; // 200ms feedback

void UIManager::handleTouchEvent(TouchEvent event, TouchPoint point) {
  // Handle different touch events
  switch (event) {
    case TOUCH_GESTURE_TAP:
      // Handle tap gesture - theme switching
      if (currentScreen == SCREEN_IDLE || currentScreen == SCREEN_PRINTING) {
        display->nextTheme();
        // Redraw current screen to show new theme
        if (currentScreen == SCREEN_IDLE) {
          drawIdleScreen(lastStatus);
        } else if (currentScreen == SCREEN_PRINTING) {
          drawPrintingScreen(lastStatus);
        }
        Serial.println("Touch: Theme switched via tap");
      }
      break;
      
    case TOUCH_GESTURE_DOUBLE_TAP:
      // Handle double tap - could be used for other functions
      Serial.println("Touch: Double tap detected");
      break;
      
    case TOUCH_GESTURE_SWIPE_LEFT:
    case TOUCH_GESTURE_SWIPE_RIGHT:
      // Handle swipe gestures - could navigate between screens
      Serial.println("Touch: Swipe gesture detected");
      break;
      
    case TOUCH_GESTURE_SWIPE_UP:
      // Swipe up - show more details or next screen
      Serial.println("Touch: Swipe up detected");
      break;
      
    case TOUCH_GESTURE_SWIPE_DOWN:
      // Swipe down - show less details or previous screen
      Serial.println("Touch: Swipe down detected");
      break;
      
    case TOUCH_DOWN:
      // Touch pressed down - show visual feedback
      lastTouchFeedback = millis();
      // Could add visual touch feedback here
      break;
      
    case TOUCH_UP:
      // Touch released
      break;
      
    case TOUCH_MOVE:
      // Touch moving (dragging)
      break;
      
    default:
      break;
  }
}
