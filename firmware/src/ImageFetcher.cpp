/*
 * Image Fetcher Implementation
 */

#include "ImageFetcher.h"

// Note: For JPEG/PNG decoding, you'll need additional libraries:
// - TJpg_Decoder for JPEG
// - PNGdec for PNG
// Install via: pio lib install "bodmer/TJpg_Decoder" "bitbank2/PNGdec"

ImageFetcher::ImageFetcher() :
  display(nullptr),
  fetching(false)
{
}

void ImageFetcher::init(DisplayDriver* disp) {
  display = disp;
}

bool ImageFetcher::fetchAndDisplay(const char* url, int16_t x, int16_t y, uint16_t maxWidth, uint16_t maxHeight) {
  if (!display || fetching) return false;
  
  fetching = true;
  Serial.printf("Fetching image from: %s\n", url);
  
  http.begin(url);
  http.setTimeout(5000);  // 5 second timeout
  
  int httpCode = http.GET();
  
  if (httpCode == HTTP_CODE_OK) {
    int len = http.getSize();
    
    if (len > 0 && len < 100000) {  // Max 100KB
      uint8_t* buffer = (uint8_t*)malloc(len);
      
      if (buffer) {
        WiFiClient* stream = http.getStreamPtr();
        size_t bytesRead = stream->readBytes(buffer, len);
        
        if (bytesRead == len) {
          // Detect image type by magic bytes
          if (buffer[0] == 0xFF && buffer[1] == 0xD8) {
            // JPEG
            Serial.println("Detected JPEG");
            decodeJPEG(buffer, len, x, y, maxWidth, maxHeight);
          } else if (buffer[0] == 0x89 && buffer[1] == 0x50) {
            // PNG
            Serial.println("Detected PNG");
            decodePNG(buffer, len, x, y, maxWidth, maxHeight);
          } else {
            Serial.println("Unknown image format");
          }
        }
        
        free(buffer);
      }
    }
  } else {
    Serial.printf("HTTP error: %d\n", httpCode);
  }
  
  http.end();
  fetching = false;
  
  return true;
}

bool ImageFetcher::fetchWebcam(const char* webcamUrl, int16_t x, int16_t y) {
  // Fetch webcam snapshot (usually JPEG)
  return fetchAndDisplay(webcamUrl, x, y, 240, 240);
}

bool ImageFetcher::fetchPrintPreview(const char* klipperIP, const char* filename, int16_t x, int16_t y) {
  // Construct Klipper thumbnail URL
  // Format: http://IP/server/files/gcodes/filename?thumbnail=true
  char url[256];
  snprintf(url, sizeof(url), "http://%s:7125/server/files/gcodes/%s?thumbnail=true", klipperIP, filename);
  
  return fetchAndDisplay(url, x, y, 200, 200);
}

bool ImageFetcher::decodeJPEG(uint8_t* data, size_t len, int16_t x, int16_t y, uint16_t maxWidth, uint16_t maxHeight) {
  // TODO: Implement JPEG decoding with TJpg_Decoder
  // For now, just show a placeholder
  display->drawRect(x, y, maxWidth, maxHeight, display->getThemeColors().accent);
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText("JPEG", y + maxHeight/2, 1);
  
  Serial.println("JPEG decoding not yet implemented");
  Serial.println("Install: pio lib install 'bodmer/TJpg_Decoder'");
  
  return false;
}

bool ImageFetcher::decodePNG(uint8_t* data, size_t len, int16_t x, int16_t y, uint16_t maxWidth, uint16_t maxHeight) {
  // TODO: Implement PNG decoding with PNGdec
  // For now, just show a placeholder
  display->drawRect(x, y, maxWidth, maxHeight, display->getThemeColors().accent);
  display->setTextColor(display->getThemeColors().text);
  display->drawCenteredText("PNG", y + maxHeight/2, 1);
  
  Serial.println("PNG decoding not yet implemented");
  Serial.println("Install: pio lib install 'bitbank2/PNGdec'");
  
  return false;
}
