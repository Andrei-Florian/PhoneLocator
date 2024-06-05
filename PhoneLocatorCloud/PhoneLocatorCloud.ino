#include "cloud.h"

const int buttonPin = 1;
const int ledRedPin = 3;
const int ledGreenPin = 4;
const int ledBluePin = 5;

const bool debugging = true;

bool localButtonState;
bool updatePending = false;

void setup()
{
    if (debugging)
    {
        Serial.begin(9600);
        while (!Serial)
            ;

        Serial.println("Initialising Arduino Cloud");
    }

    initProperties();
    ArduinoCloud.begin(ArduinoIoTPreferredConnection);
    setDebugMessageLevel(2);
    ArduinoCloud.printDebugInfo();

    if (debugging)
        Serial.println("Initialising components");

    pinMode(buttonPin, INPUT);
    pinMode(ledRedPin, OUTPUT);
    pinMode(ledGreenPin, OUTPUT);
    pinMode(ledBluePin, OUTPUT);

    if (debugging)
        Serial.println("Setup Complete! \n");
}

void loop()
{
    if (updatePending)
        blueLED();

    ArduinoCloud.update();
    localButtonState = digitalRead(buttonPin);

    if (localButtonState == HIGH)
    {
        if (debugging)
            Serial.println("Button pressed!");

        updatePending = true;
    }

    delay(200);
    if (updatePending)
        offLED();
    delay(200);
}

void onPhoneLocationChange()
{
    if (debugging)
        Serial.println("Phone Location Received: \n  Latitude: " + String(phoneLocation.getValue().lat, 8) + "\n  Longitude: " + String(phoneLocation.getValue().lon, 8));

    if (updatePending)
    {
        if (debugging)
            Serial.println("Updating cloud");

        buttonState++;
        updatePending = false;

        greenLED();
        delay(3000);
        offLED();
    }
}

void blueLED()
{
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledBluePin, HIGH);
}

void greenLED()
{
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledGreenPin, HIGH);
    digitalWrite(ledBluePin, LOW);
}

void offLED()
{
    digitalWrite(ledRedPin, LOW);
    digitalWrite(ledGreenPin, LOW);
    digitalWrite(ledBluePin, LOW);
}