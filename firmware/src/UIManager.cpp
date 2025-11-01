/*
 * UI Manager Implementation
 */

#include "UIManager.h"
#include "WifiConfig.h"

UIManager::UIManager() : 
  display(nullptr),
  currentScreen(SCREEN_IDLE),
  lastAnimationUpdate(0),
  animationFrame(0),
  lastScreenSwitch(0),
  showingAnimation(false),
  lastTouchFeedback(0),
  manualMode(false)
{
}

void UIManager::init(DisplayDriver* disp) {
  display = disp;
  currentScreen = SCREEN_BOOT;
  animationFrame = 0;
}

void UIManager::showBootScreen() {
  currentScreen = SCREEN_BOOT;
  display->clear();
  
  // Draw rainbow arc at top (Electric Callboy style)
  int16_t centerX = 120;
  int16_t centerY = 120;
  int16_t radius = 90;
  
  // Rainbow colors
  uint16_t rainbowColors[] = {
    display->color565(255, 0, 0),     // Red
    display->color565(255, 127, 0),   // Orange
    display->color565(255, 255, 0),   // Yellow
    display->color565(0, 255, 0),     // Green
    display->color565(0, 0, 255),     // Blue
    display->color565(75, 0, 130),    // Indigo
    display->color565(148, 0, 211)    // Violet
  };
  
  // Draw rainbow arcs (smaller for boot screen)
  for (int i = 0; i < 7; i++) {
    for (int angle = 200; angle <= 340; angle += 3) {
      float rad = angle * PI / 180.0;
      int16_t x1 = centerX + (radius - i * 3) * cos(rad);
      int16_t y1 = centerY + (radius - i * 3) * sin(rad);
      int16_t x2 = centerX + (radius - i * 3 - 2) * cos(rad);
      int16_t y2 = centerY + (radius - i * 3 - 2) * sin(rad);
      display->drawLine(x1, y1, x2, y2, rainbowColors[i]);
    }
  }
  
  // Draw title text
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText("PaWe", 130, 3);
  display->drawCenteredText("i-print", 155, 2);
  
  // Draw version
  display->setTextColor(display->getThemeColors().secondary);
  display->drawCenteredText("v1.1", 185, 1);
  
  // Draw loading indicator
  display->drawCircle(SCREEN_WIDTH/2, 210, 8, display->getThemeColors().accent);
}

void UIManager::updateStatus(PrinterStatus& status) {
  // Skip automatic screen switching if user is in manual mode
  if (manualMode) {
    // Just update the data on current screen without switching
    if (shouldRedraw(status)) {
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
      }
    }
    lastStatus = status;
    return;
  }
  
  // Automatic mode: Determine which screen to show based on state
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
  
  // Animation cycling logic
  unsigned long currentTime = millis();
  unsigned long timeSinceSwitch = currentTime - lastScreenSwitch;
  bool modeChanged = false;
  
  // Check if it's time to switch between data and animation
  if (showingAnimation && timeSinceSwitch > ANIMATION_DISPLAY_TIME) {
    showingAnimation = false;
    lastScreenSwitch = currentTime;
    display->clear();
    modeChanged = true;
  } else if (!showingAnimation && timeSinceSwitch > DATA_DISPLAY_TIME) {
    showingAnimation = true;
    lastScreenSwitch = currentTime;
    display->clear();
    modeChanged = true;
  }
  
  // Redraw if screen changed, mode changed, or significant data changed
  if (newScreen != currentScreen || modeChanged || shouldRedraw(status)) {
    // Clear screen only when switching screens
    if (newScreen != currentScreen) {
      display->clear();
      lastScreenSwitch = currentTime;  // Reset timer on screen change
      showingAnimation = false;  // Start with data view
    }
    currentScreen = newScreen;
    
    // Choose between data view and animation view
    switch (currentScreen) {
      case SCREEN_IDLE:
        if (showingAnimation) {
          drawIdleAnimation(status);
        } else {
          drawIdleScreen(status);
        }
        break;
      case SCREEN_PRINTING:
        if (showingAnimation) {
          drawPrintingAnimation(status);
        } else {
          drawPrintingScreen(status);
        }
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
  // Only clear on screen change (handled by updateStatus)
  // Don't clear here to prevent flicker during animation
  
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
  // Only clear on screen change (handled by updateStatus)
  
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
  // Only clear on screen change (handled by updateStatus)
  
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

void UIManager::drawProgressCircle(uint8_t progress) {
  // Draw progress ring in center of screen
  int16_t centerX = SCREEN_WIDTH / 2;
  int16_t centerY = SCREEN_HEIGHT / 2;
  int16_t radius = 75;
  int16_t thickness = 12;
  uint16_t color = display->getThemeColors().highlight;
  
  display->drawProgressRing(centerX, centerY, radius, thickness, progress, color);
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

void UIManager::handleTouchEvent(TouchEvent event, TouchPoint point) {
  // Handle different touch events
  switch (event) {
    case TOUCH_GESTURE_TAP:
      // Handle tap gesture - theme switching (also exits manual mode)
      manualMode = false;  // Exit manual mode on tap
      if (currentScreen == SCREEN_IDLE || currentScreen == SCREEN_PRINTING) {
        display->nextTheme();
        // Clear screen and redraw to show new theme
        display->clear();
        if (currentScreen == SCREEN_IDLE) {
          drawIdleScreen(lastStatus);
        } else if (currentScreen == SCREEN_PRINTING) {
          drawPrintingScreen(lastStatus);
        }
        Serial.println("Touch: Theme switched via tap");
      }
      break;
      
    case TOUCH_GESTURE_CIRCLE:
      // Handle circle gesture - show VIVA LA ELTON JOHN logo
      {
        Serial.println("Touch: Circle gesture - showing VIVA LA ELTON JOHN!");
        display->clear();
        
        // Draw rainbow arc at top
        int16_t centerX = 120;
        int16_t centerY = 120;
        int16_t radius = 100;
        
        // Rainbow colors (7 colors)
        uint16_t rainbowColors[] = {
          display->color565(255, 0, 0),     // Red
          display->color565(255, 127, 0),   // Orange
          display->color565(255, 255, 0),   // Yellow
          display->color565(0, 255, 0),     // Green
          display->color565(0, 0, 255),     // Blue
          display->color565(75, 0, 130),    // Indigo
          display->color565(148, 0, 211)    // Violet
        };
        
        // Draw rainbow arcs
        for (int i = 0; i < 7; i++) {
          for (int angle = 180; angle <= 360; angle += 2) {
            float rad = angle * PI / 180.0;
            int16_t x1 = centerX + (radius - i * 4) * cos(rad);
            int16_t y1 = centerY + (radius - i * 4) * sin(rad);
            int16_t x2 = centerX + (radius - i * 4 - 3) * cos(rad);
            int16_t y2 = centerY + (radius - i * 4 - 3) * sin(rad);
            display->drawLine(x1, y1, x2, y2, rainbowColors[i]);
          }
        }
        
        // Draw text
        display->setTextColor(display->getThemeColors().text);
        display->drawCenteredText("VIVA LA", 140, 3);
        display->drawCenteredText("ELTON JOHN", 170, 3);
        
        // Show for 3 seconds
        delay(3000);
        
        // Clear and redraw current screen
        display->clear();
        switch (currentScreen) {
          case SCREEN_IDLE:
            drawIdleScreen(lastStatus);
            break;
          case SCREEN_PRINTING:
            drawPrintingScreen(lastStatus);
            break;
          case SCREEN_PAUSED:
            drawPausedScreen(lastStatus);
            break;
          case SCREEN_COMPLETE:
            drawCompleteScreen(lastStatus);
            break;
          default:
            break;
        }
      }
      break;
      
    case TOUCH_GESTURE_SWIPE_LEFT:
      // Swipe left - next display mode
      {
        // Cycle through display modes: IDLE -> PRINTING -> PAUSED -> COMPLETE -> back to IDLE
        ScreenType nextScreen = currentScreen;
        switch (currentScreen) {
          case SCREEN_IDLE:
            nextScreen = SCREEN_PRINTING;
            break;
          case SCREEN_PRINTING:
            nextScreen = SCREEN_PAUSED;
            break;
          case SCREEN_PAUSED:
            nextScreen = SCREEN_COMPLETE;
            break;
          case SCREEN_COMPLETE:
          case SCREEN_ERROR:
          default:
            nextScreen = SCREEN_IDLE;
            break;
        }
        
        currentScreen = nextScreen;
        display->clear();
        
        // Draw the new screen
        switch (currentScreen) {
          case SCREEN_IDLE:
            drawIdleScreen(lastStatus);
            break;
          case SCREEN_PRINTING:
            drawPrintingScreen(lastStatus);
            break;
          case SCREEN_PAUSED:
            drawPausedScreen(lastStatus);
            break;
          case SCREEN_COMPLETE:
            drawCompleteScreen(lastStatus);
            break;
          default:
            break;
        }
        
        manualMode = true;  // Enter manual mode
        Serial.printf("Touch: Swipe left - Screen mode: %d (manual)\n", currentScreen);
      }
      break;
      
    case TOUCH_GESTURE_SWIPE_RIGHT:
      // Swipe right - previous display mode
      {
        ScreenType prevScreen = currentScreen;
        switch (currentScreen) {
          case SCREEN_IDLE:
            prevScreen = SCREEN_COMPLETE;
            break;
          case SCREEN_PRINTING:
            prevScreen = SCREEN_IDLE;
            break;
          case SCREEN_PAUSED:
            prevScreen = SCREEN_PRINTING;
            break;
          case SCREEN_COMPLETE:
          case SCREEN_ERROR:
          default:
            prevScreen = SCREEN_PAUSED;
            break;
        }
        
        currentScreen = prevScreen;
        display->clear();
        
        // Draw the new screen
        switch (currentScreen) {
          case SCREEN_IDLE:
            drawIdleScreen(lastStatus);
            break;
          case SCREEN_PRINTING:
            drawPrintingScreen(lastStatus);
            break;
          case SCREEN_PAUSED:
            drawPausedScreen(lastStatus);
            break;
          case SCREEN_COMPLETE:
            drawCompleteScreen(lastStatus);
            break;
          default:
            break;
        }
        
        manualMode = true;  // Enter manual mode
        Serial.printf("Touch: Swipe right - Screen mode: %d (manual)\n", currentScreen);
      }
      break;
      
    case TOUCH_GESTURE_SWIPE_UP:
      // Swipe up - increase brightness
      {
        int currentBrightness = display->getBrightness();
        int newBrightness = min(255, currentBrightness + 25);
        display->setBrightness(newBrightness);
        Serial.printf("Touch: Swipe up - Brightness: %d -> %d\n", currentBrightness, newBrightness);
        
        // Show brightness indicator briefly
        display->clear();
        display->setTextColor(display->getThemeColors().text);
        display->drawCenteredText("Brightness", 100, 2);
        char brightStr[16];
        sprintf(brightStr, "%d%%", (newBrightness * 100) / 255);
        display->drawCenteredText(brightStr, 130, 3);
        delay(500);
        
        // Redraw current screen
        if (currentScreen == SCREEN_IDLE) {
          drawIdleScreen(lastStatus);
        } else if (currentScreen == SCREEN_PRINTING) {
          drawPrintingScreen(lastStatus);
        }
      }
      break;
      
    case TOUCH_GESTURE_SWIPE_DOWN:
      // Swipe down - decrease brightness
      {
        int currentBrightness = display->getBrightness();
        int newBrightness = max(20, currentBrightness - 25);
        display->setBrightness(newBrightness);
        Serial.printf("Touch: Swipe down - Brightness: %d -> %d\n", currentBrightness, newBrightness);
        
        // Show brightness indicator briefly
        display->clear();
        display->setTextColor(display->getThemeColors().text);
        display->drawCenteredText("Brightness", 100, 2);
        char brightStr[16];
        sprintf(brightStr, "%d%%", (newBrightness * 100) / 255);
        display->drawCenteredText(brightStr, 130, 3);
        delay(500);
        
        // Redraw current screen
        if (currentScreen == SCREEN_IDLE) {
          drawIdleScreen(lastStatus);
        } else if (currentScreen == SCREEN_PRINTING) {
          drawPrintingScreen(lastStatus);
        }
      }
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
