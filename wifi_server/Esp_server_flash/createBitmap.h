#include <LittleFS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

class BitMap{

  private:
      static constexpr uint8_t _a = 128;
      static constexpr uint8_t _b = 64;
      static constexpr int _reset = -1;
      // 128 / 8 columns * 64 rows = 1024 bytes
      Adafruit_SSD1306 display;
      #define _SIZE 1024
      uint8_t _dynamicBitmap[_SIZE];

      void _drawMyUploadedBitmap() {
      // 1. Load the file from LittleFS into our dynamicBitmap array
        // 2. Clear the screen buffer
        display.clearDisplay();
        
        // 3. Draw the bitmap from our RAM array
        // (x, y, bitmap_array, width, height, color)
        display.drawBitmap(0, 0, _dynamicBitmap, 128, 64, SSD1306_WHITE);
        
        // 4. Push the buffer to the screen hardware
        display.display();
      }


  public:
    BitMap() : display(_a, _b, &Wire, _reset) {}
    void init(){//this method help initializes oled display
      
      if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        for (;;)
      ;  // Don't proceed, loop forever
      }
      display.display();
      delay(1000);
      display.clearDisplay();
      delay(200);
   }

    bool loadBitmapFromFS(String filename) {
      if (!LittleFS.exists(filename)) {
        Serial.println("Bitmap file not found!");
        return false;
      }

      File file = LittleFS.open(filename, "r");
      if (!file) {
        Serial.println("Failed to open bitmap file for reading.");
        return false;
      }

      int index = 0;
      
      // Read through the file until the end or until our array is full
      while (file.available() && index < _SIZE) {
        String token = "";
        char c = file.read();

        // Read until we hit a comma or newline
        while (file.available() && c != ',' && c != '\n' && c != '\r') {
          // Ignore spaces
          if (c != ' ') {
            token += c;
          }
          c = file.read();
        }

        // Process the token (e.g., "0x3F" or "0x00")
        if (token.length() > 0) {
          // strtol parses strings like "0xFF" or "FF" into a number base 16 (hex)
          _dynamicBitmap[index] = (uint8_t)strtol(token.c_str(), NULL, 16);
          index++;
        }
      }

      file.close();
      bool result = (index == _SIZE);
      if (result) {
        _drawMyUploadedBitmap();
        Serial.printf("Successfully loaded %d bytes into RAM bitmap.\n", index);
      }
      return result;
    }

};