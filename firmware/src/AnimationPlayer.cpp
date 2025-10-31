/*
 * Animation Player Implementation
 */

#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer() :
  display(nullptr),
  currentAnimation(nullptr),
  currentFrame(0),
  lastFrameTime(0),
  posX(0),
  posY(0),
  playing(false)
{
}

void AnimationPlayer::init(DisplayDriver* disp) {
  display = disp;
}

void AnimationPlayer::play(const Animation* anim, int16_t x, int16_t y) {
  if (!anim || !display) return;
  
  currentAnimation = anim;
  currentFrame = 0;
  posX = x;
  posY = y;
  playing = true;
  lastFrameTime = millis();
}

void AnimationPlayer::stop() {
  playing = false;
  currentAnimation = nullptr;
}

void AnimationPlayer::update() {
  if (!playing || !currentAnimation || !display) return;
  
  unsigned long now = millis();
  const AnimationFrame& frame = currentAnimation->frames[currentFrame];
  
  // Check if it's time for next frame
  if (now - lastFrameTime >= frame.delay_ms) {
    // Draw current frame
    // For now, we'll use a simple pixel-by-pixel approach
    // In production, you'd use DMA or optimized methods
    
    // Move to next frame
    currentFrame++;
    
    // Check if animation is complete
    if (currentFrame >= currentAnimation->frameCount) {
      if (currentAnimation->loop) {
        currentFrame = 0;  // Loop back
      } else {
        stop();  // Stop playing
        return;
      }
    }
    
    lastFrameTime = now;
  }
}
