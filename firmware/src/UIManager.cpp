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
  display->setTextColor(COLOR_ACCENT);
  display->drawCenteredText("KNOMI", 80, 3);
  display->setTextColor(COLOR_TEXT);
  display->drawCenteredText("Clone", 110, 2);
  
  // Draw version
  display->setTextColor(COLOR_GRAY);
  display->drawCenteredText("v1.0", 140, 1);
  
  // Draw loading indicator
  display->drawCircle(SCREEN_WIDTH/2, 180, 10, COLOR_ACCENT);
}

void UIManager::showConnectingScreen() {
  currentScreen = SCREEN_CONNECTING;
  display->clear();
  
  display->setTextColor(COLOR_TEXT);
  display->drawCenteredText("Connecting", 100, 2);
  display->setTextColor(COLOR_GRAY);
  display->drawCenteredText("to WiFi...", 130, 1);
  
  // Draw WiFi icon
  display->drawWiFiIcon(SCREEN_WIDTH/2, 170, COLOR_ACCENT, 1);
}

void UIManager::showConnectedScreen() {
  currentScreen = SCREEN_CONNECTED;
  display->clear();
  
  display->setTextColor(COLOR_SUCCESS);
  display->drawCenteredText("Connected!", 110, 2);
  
  // Draw checkmark
  display->drawCheckIcon(SCREEN_WIDTH/2, 160, COLOR_SUCCESS);
}

void UIManager::showWiFiError() {
  if (currentScreen == SCREEN_WIFI_ERROR) return;
  
  currentScreen = SCREEN_WIFI_ERROR;
  display->clear();
  
  display->setTextColor(COLOR_ERROR);
  display->drawCenteredText("WiFi Error", 100, 2);
  display->setTextColor(COLOR_GRAY);
  display->drawCenteredText("Check config", 130, 1);
  
  // Draw error icon
  display->drawErrorIcon(SCREEN_WIDTH/2, 170, COLOR_ERROR);
}

void UIManager::showKlipperError() {
  if (currentScreen == SCREEN_KLIPPER_ERROR) return;
  
  currentScreen = SCREEN_KLIPPER_ERROR;
  display->clear();
  
  display->setTextColor(COLOR_ERROR);
  display->drawCenteredText("Klipper", 90, 2);
  display->drawCenteredText("Offline", 120, 2);
  display->setTextColor(COLOR_GRAY);
  display->drawCenteredText("Reconnecting...", 150, 1);
  
  // Draw printer icon
  display->drawPrinterIcon(SCREEN_WIDTH/2 - 15, 175, COLOR_ERROR);
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
  display->setTextColor(COLOR_TEXT);
  display->drawCenteredText("Ready", 210, 2);
  
  // Draw temperatures
  char tempStr[32];
  sprintf(tempStr, "%.0f/%.0f", status.hotendTemp, status.hotendTarget);
  display->setTextColor(COLOR_ORANGE);
  display->drawCenteredText(tempStr, 230, 1);
}

void UIManager::drawPrintingScreen(PrinterStatus& status) {
  display->clear();
  
  // Draw progress ring
  drawProgressCircle(status.printProgress);
  
  // Draw progress percentage in center
  char progressStr[8];
  sprintf(progressStr, "%d%%", status.printProgress);
  display->setTextColor(COLOR_TEXT);
  display->drawCenteredText(progressStr, 110, 3);
  
  // Draw temperatures
  display->setTextColor(COLOR_ORANGE);
  char tempStr[32];
  sprintf(tempStr, "E:%.0f B:%.0f", status.hotendTemp, status.bedTemp);
  display->drawCenteredText(tempStr, 145, 1);
  
  // Draw time remaining
  if (status.printTimeLeft > 0) {
    display->setTextColor(COLOR_GRAY);
    String timeStr = formatTime(status.printTimeLeft);
    display->drawCenteredText(timeStr, 165, 1);
  }
  
  // Draw filename (truncated)
  if (status.fileName.length() > 0) {
    display->setTextColor(COLOR_ACCENT);
    String shortName = status.fileName;
    if (shortName.length() > 20) {
      shortName = shortName.substring(0, 17) + "...";
    }
    display->drawCenteredText(shortName, 190, 1);
  }
  
  // Draw Z height
  display->setTextColor(COLOR_GRAY);
  char zStr[16];
  sprintf(zStr, "Z:%.2f", status.posZ);
  display->drawCenteredText(zStr, 210, 1);
}

void UIManager::drawPausedScreen(PrinterStatus& status) {
  display->clear();
  
  // Draw progress ring (dimmed)
  drawProgressCircle(status.printProgress);
  
  // Draw PAUSED text
  display->setTextColor(COLOR_WARNING);
  display->drawCenteredText("PAUSED", 110, 2);
  
  // Draw progress percentage
  char progressStr[8];
  sprintf(progressStr, "%d%%", status.printProgress);
  display->setTextColor(COLOR_TEXT);
  display->drawCenteredText(progressStr, 140, 2);
  
  // Draw temperatures
  display->setTextColor(COLOR_ORANGE);
  char tempStr[32];
  sprintf(tempStr, "E:%.0f B:%.0f", status.hotendTemp, status.bedTemp);
  display->drawCenteredText(tempStr, 170, 1);
}

void UIManager::drawCompleteScreen(PrinterStatus& status) {
  display->clear();
  
  // Draw checkmark
  display->drawCheckIcon(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, COLOR_SUCCESS);
  
  // Draw COMPLETE text
  display->setTextColor(COLOR_SUCCESS);
  display->drawCenteredText("COMPLETE", 150, 2);
  
  // Draw print time
  display->setTextColor(COLOR_GRAY);
  String timeStr = formatTime(status.printTime);
  display->drawCenteredText(timeStr, 180, 1);
}

void UIManager::drawErrorScreen() {
  display->clear();
  
  // Draw error icon
  display->drawErrorIcon(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 - 20, COLOR_ERROR);
  
  // Draw ERROR text
  display->setTextColor(COLOR_ERROR);
  display->drawCenteredText("ERROR", 150, 2);
  display->setTextColor(COLOR_GRAY);
  display->drawCenteredText("Check printer", 180, 1);
}

void UIManager::drawProgressCircle(uint8_t progress) {
  display->drawProgressRing(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 80, 8, progress, COLOR_ACCENT);
}

void UIManager::update() {
  unsigned long currentMillis = millis();
  
  // Update animations
  if (currentMillis - lastAnimationUpdate >= ANIMATION_SPEED) {
    lastAnimationUpdate = currentMillis;
    animationFrame++;
    
    // Update screen-specific animations
    if (currentScreen == SCREEN_IDLE) {
      updateRollingEyes();
    } else if (currentScreen == SCREEN_PRINTING) {
      updatePrintingAnimation();
    }
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
