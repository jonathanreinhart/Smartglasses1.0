/*******************************************************************************
 * Arduino Nano                   -> GC9A01
 * Vin                            -> Vcc
 * GND                            -> GND
 * GPIO 9        -> 3.3k resistor -> CS
 * GPIO 7        -> 3.3k resistor -> RESET
 * GPIO 8        -> 3.3k resistor -> D/C
 * GPIO 11(MOSI) -> 3.3k resistor -> SDI(MOSI)
 * GPIO 13(SCK)  -> 3.3k resistor -> SCK
 * GPIO 6                         -> LED

 * Start of Arduino_GFX setting
 * 
 * Arduino_GFX try to find the settings depends on selected board in Arduino IDE
 * Or you can define the display dev kit not in the board list
 * Defalult pin list for non display dev kit:
 * Arduino Nano, Micro and more: CS:  9, DC:  8, RST:  7, BL:  6
 * ESP32 various dev board     : CS:  5, DC: 27, RST: 33, BL: 22
 * ESP32-C3 various dev board  : CS:  7, DC:  2, RST:  1, BL:  3
 * ESP32-S2/3 various dev board: CS: 34, DC: 26, RST: 33, BL: 21
 * ESP8266 various dev board   : CS: 15, DC:  4, RST:  2, BL:  5
 * Raspberry Pi Pico dev board : CS: 17, DC: 27, RST: 26, BL: 28
 * RTL8720 BW16 old patch core : CS: 18, DC: 17, RST:  2, BL: 23
 * RTL8720_BW16 Official core  : CS:  9, DC:  8, RST:  6, BL:  3
 * RTL8722 dev board           : CS: 18, DC: 17, RST: 22, BL: 23
 * RTL8722_mini dev board      : CS: 12, DC: 14, RST: 15, BL: 13
 * Seeeduino XIAO dev board    : CS:  3, DC:  2, RST:  1, BL:  0
 * Teensy 4.1 dev board        : CS: 39, DC: 41, RST: 40, BL: 22
 ******************************************************************************/
#include <U8g2lib.h>
#include <Arduino_GFX_Library.h>
#include "WifiFuncs.h"
#include "BluetoothFuncs.h"

#define GFX_BL DF_GFX_BL // default backlight pin, you may replace DF_GFX_BL to actual backlight pin

/* More dev device declaration: https://github.com/moononournation/Arduino_GFX/wiki/Dev-Device-Declaration */
#if defined(DISPLAY_DEV_KIT)
Arduino_GFX *gfx = create_default_Arduino_GFX();
#else /* !defined(DISPLAY_DEV_KIT) */

/* More data bus class: https://github.com/moononournation/Arduino_GFX/wiki/Data-Bus-Class */
Arduino_DataBus *bus = create_default_Arduino_DataBus();

/* More display class: https://github.com/moononournation/Arduino_GFX/wiki/Display-Class */
Arduino_GFX *gfx = new Arduino_GC9A01(bus, 33 /* RST */, 0 /* rotation */, true /* IPS */);

#endif /* !defined(DISPLAY_DEV_KIT) */
/*******************************************************************************
 * End of Arduino_GFX setting
 ******************************************************************************/

WifiFuncs wifi_f;
BluetoothFuncs blue_f;

void setup(void)
{
  Serial.begin(115200);
  wifi_f.WifiInit();
  wifi_f.NTPInit();
  blue_f.ANCSInit();
  gfx->begin();
  gfx->fillScreen(BLACK);

#ifdef GFX_BL
  pinMode(GFX_BL, OUTPUT);
  digitalWrite(GFX_BL, HIGH);
#endif

  gfx->setCursor(70,140);
  //gfx->setUTF8Print(true);
  gfx->setFont(u8g2_font_fur30_tn);
  gfx->setTextColor(WHITE);
  gfx->println("21:32");
}

void loop()
{
  wifi_f.printTime();
  blue_f.printNotifications();
}
