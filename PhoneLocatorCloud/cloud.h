#include <ArduinoIoTCloud.h>
#include <Arduino_ConnectionHandler.h>
#include "secrets.h"

void onPhoneLocationChange();

CloudLocation phoneLocation;
int buttonState;

void initProperties()
{
    ArduinoCloud.addProperty(phoneLocation, READWRITE, ON_CHANGE, onPhoneLocationChange);
    ArduinoCloud.addProperty(buttonState, READ, ON_CHANGE, NULL);
}

WiFiConnectionHandler ArduinoIoTPreferredConnection(SSID, PASS);