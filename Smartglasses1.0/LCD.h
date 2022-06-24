#ifndef LCD_H
#define LCD_H

#define PNG_FILENAME "/cloudy.png"

#include <Arduino_GFX_Library.h>

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

#include <FFat.h>
#include <LITTLEFS.h>
#include <SPIFFS.h>
#include <SD.h>

#include <PNGdec.h>

class LCD{
  private:
    
    /* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
    static Arduino_DataBus *bus;
    static Arduino_GFX *gfx;
    static PNG png;
    static int16_t w, h, xOffset, yOffset;
    // Functions to access a file on the SD card
    static File pngFile;
    static void *myOpen(const char *filename, int32_t *size);
    static void myClose(void *handle);
    static int32_t myRead(PNGFILE *handle, uint8_t *buffer, int32_t length);
    static int32_t mySeek(PNGFILE *handle, int32_t position);
    static void PNGDraw(PNGDRAW *pDraw);
  public:
    static void initLCD();
    static void drawWeather(int icon);
};

#endif
