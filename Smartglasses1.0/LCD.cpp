#include "LCD.h"

void LCD::initLCD(){
  *bus = create_default_Arduino_DataBus();
  *gfx = new Arduino_GC9A01(bus, 33 /* RST */, 0 /* rotation */, true /* IPS */);
  gfx->begin();
  gfx->fillScreen(BLACK);

  w = gfx->width(), h = gfx->height();
  gfx->fillScreen(BLACK);
  
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
}

void *LCD::myOpen(const char *filename, int32_t *size)
{
  /* Wio Terminal */
#if defined(ARDUINO_ARCH_SAMD) && defined(SEEED_GROVE_UI_WIRELESS)
  pngFile = SD.open(filename, "r");
#elif defined(ARDUINO_RASPBERRY_PI_PICO)
  pngFile = LITTLEFS.open(filename, "r");
  // pngFile = SD.open(filename, "r");
#elif defined(ESP32)
  // pngFile = FFat.open(filename, "r");
  pngFile = LITTLEFS.open(filename, "r");
  // pngFile = SPIFFS.open(filename, "r");
  // pngFile = SD.open(filename, "r");
#elif defined(ESP8266)
  pngFile = LITTLEFS.open(filename, "r");
  // pngFile = SD.open(filename, "r");
#else
  pngFile = SD.open(filename, FILE_READ);
#endif

  if (!pngFile || pngFile.isDirectory())
  {
    Serial.println(F("ERROR: Failed to open " PNG_FILENAME " file for reading"));
    gfx->println(F("ERROR: Failed to open " PNG_FILENAME " file for reading"));
  }
  else
  {
    *size = pngFile.size();
    Serial.printf("Opened '%s', size: %d\n", filename, *size);
  }

  return &pngFile;
}

void LCD::myClose(void *handle)
{
  if (pngFile)
    pngFile.close();
}

int32_t LCD::myRead(PNGFILE *handle, uint8_t *buffer, int32_t length)
{
  if (!pngFile)
    return 0;
  return pngFile.read(buffer, length);
}

int32_t LCD::mySeek(PNGFILE *handle, int32_t position)
{
  if (!pngFile)
    return 0;
  return pngFile.seek(position);
}

// Function to draw pixels to the display
void LCD::PNGDraw(PNGDRAW *pDraw)
{
  uint16_t usPixels[320];
  uint8_t usMask[320];

  // Serial.printf("Draw pos = 0,%d. size = %d x 1\n", pDraw->y, pDraw->iWidth);
  png.getLineAsRGB565(pDraw, usPixels, PNG_RGB565_LITTLE_ENDIAN, 0x00000000);
  png.getAlphaMask(pDraw, usMask, 1);
  gfx->draw16bitRGBBitmap(xOffset, yOffset + pDraw->y, usPixels, usMask, pDraw->iWidth, 1);
}

void LCD::drawWeather(int icon){
  if (!LITTLEFS.begin())
  {
    Serial.println(F("ERROR: File System Mount Failed!"));
    gfx->println(F("ERROR: File System Mount Failed!"));
  }
  else
  {
    unsigned long start = millis();
    int rc;
    rc = png.open(PNG_FILENAME, myOpen, myClose, myRead, mySeek, PNGDraw);
    if (rc == PNG_SUCCESS)
    {
      int16_t pw = png.getWidth();
      int16_t ph = png.getHeight();

      xOffset = (w - pw) / 2;
      yOffset = (h - ph) / 2;

      rc = png.decode(NULL, 0);

      Serial.printf("Draw offset: (%d, %d), time used: %lu\n", xOffset, yOffset, millis() - start);
      Serial.printf("image specs: (%d x %d), %d bpp, pixel type: %d\n", png.getWidth(), png.getHeight(), png.getBpp(), png.getPixelType());
      png.close();
    }
    else
    {
      Serial.println("png.open() failed!");
    }
  }
}

String getIconFile(String weatherID){
  
}
