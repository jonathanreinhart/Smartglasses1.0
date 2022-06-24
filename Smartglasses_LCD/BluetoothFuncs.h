#ifndef BluetoothFuncs_H
#define BluetoothFuncs_H

#include "esp32notifications.h"

class BluetoothFuncs{
  private:
    BLENotifications notifications;
    // Holds the incoming call's ID number, or zero if no notification
    uint32_t incomingCallNotificationUUID;
    void onBLEStateChanged(BLENotifications::State state);
    void onNotificationArrived(const ArduinoNotification * notification, const Notification * rawNotificationData);
    void onNotificationRemoved(const ArduinoNotification * notification, const Notification * rawNotificationData);

  public:
    void ANCSInit();
    void printNotifications();
};

#endif
