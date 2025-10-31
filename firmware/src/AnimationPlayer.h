/*
 * Animation Player
 * Plays frame-by-frame animations on the display
 */

#ifndef ANIMATION_PLAYER_H
#define ANIMATION_PLAYER_H

#include <Arduino.h>
#include "DisplayDriver.h"

// Animation frame structure
struct AnimationFrame {
  const uint16_t* data;  // RGB565 pixel data
  uint16_t width;
  uint16_t height;
  uint16_t delay_ms;     // Delay before next frame
};

// Animation structure
struct Animation {
  const AnimationFrame* frames;
  uint8_t frameCount;
  bool loop;
};

class AnimationPlayer {
public:
  AnimationPlayer();
  
  void init(DisplayDriver* disp);
  
  // Play animation
  void play(const Animation* anim, int16_t x, int16_t y);
  void stop();
  
  // Update (call in loop)
  void update();
  
  // Check if playing
  bool isPlaying() const { return playing; }
  
private:
  DisplayDriver* display;
  const Animation* currentAnimation;
  uint8_t currentFrame;
  unsigned long lastFrameTime;
  int16_t posX, posY;
  bool playing;
};

#endif // ANIMATION_PLAYER_H
