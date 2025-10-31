/*
 * Image Fetcher
 * Downloads and displays images from URLs (webcam, print previews, etc.)
 */

#ifndef IMAGE_FETCHER_H
#define IMAGE_FETCHER_H

#include <Arduino.h>
#include <HTTPClient.h>
#include "DisplayDriver.h"

class ImageFetcher {
public:
  ImageFetcher();
  
  void init(DisplayDriver* disp);
  
  // Fetch and display image from URL
  bool fetchAndDisplay(const char* url, int16_t x, int16_t y, uint16_t maxWidth, uint16_t maxHeight);
  
  // Fetch webcam snapshot
  bool fetchWebcam(const char* webcamUrl, int16_t x, int16_t y);
  
  // Fetch print preview thumbnail
  bool fetchPrintPreview(const char* klipperIP, const char* filename, int16_t x, int16_t y);
  
  // Check if currently fetching
  bool isFetching() const { return fetching; }
  
private:
  DisplayDriver* display;
  HTTPClient http;
  bool fetching;
  
  // Decode and display JPEG
  bool decodeJPEG(uint8_t* data, size_t len, int16_t x, int16_t y, uint16_t maxWidth, uint16_t maxHeight);
  
  // Decode and display PNG
  bool decodePNG(uint8_t* data, size_t len, int16_t x, int16_t y, uint16_t maxWidth, uint16_t maxHeight);
};

#endif // IMAGE_FETCHER_H
