#ifndef NTP_H
#define NTP_H

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

class WifiFuncs{
  private:
    const char* ssid     = "Jonnys iPhone 12";
    const char* password = "donaldsBunker";
    
    // Define NTP Client to get time
    WiFiUDP ntpUDP();
    NTPClient timeClient(ntpUDP);
    
    // Variables to save date and time
    String formattedDate;
    String dayStamp;
    String timeStamp;

  public:
    void WifiInit();
    void NTPInit();
    void printTime();
};

#endif
