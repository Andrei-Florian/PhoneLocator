#define BLYNK_MAX_SENDBYTES 256

#include <stdio.h>
#include <Bridge.h>
#include <BlynkSimpleYun.h>

char auth[] = "L6M732Z7WKMNZEornpb7oHADwD3WY1hp"; // TODO - insert Blynk auth token here

double latitude;
double longitude;

const int buttonPin = 2;
const int ledPin = LED_BUILTIN;

// runs in the background to populate the geolocation fields
BLYNK_WRITE(V0)
{
	latitude = param[0].asDouble();
	longitude = param[1].asDouble();
}

// check if the button is pressed
bool getButtonState(int _buttonPin)
{
	if (digitalRead(_buttonPin) == HIGH)
	{
		delay(1000);
		while (digitalRead(_buttonPin) == HIGH)
			;
		return true;
	}

	return false;
}

// check if the location fields are populated
bool checkLocation(double _latitude, double _longitude)
{
	if (_latitude == 0 || _longitude == 0)
	{
		return false;
	}

	return true;
}

// format the message to be sent
String formatMessage(double _latitude, double _longitude)
{
	String message = "Phone Located! \n received co-ordinates \n LAT ";
	message += String(_latitude, 8); // add vaiables to string
	message += " LNG ";
	message += String(_longitude, 8);
	message += ". View location on Maps: ";
	message += "www.google.com/maps/?q=";
	message += String(_latitude, 8);
	message += ",";
	message += String(_longitude, 8);

	return message;
}

// interact with the API to send the email
void sendMessage(String _message)
{
	Serial.println("[Locate] Sent location");
	Serial.println(_message);
	Serial.println("");

	Blynk.email("PhoneLocator", _message); // send email
}

void setup()
{
	pinMode(buttonPin, INPUT); // set the button pin as an input
	pinMode(ledPin, OUTPUT);   // set the led as an output
	Blynk.begin(auth);		   // initialise Blynk

	Serial.begin(9600);
	Serial.println("[INFO] phoneLocator Online");

	Serial.println("[INFO] Connecting to Phone");
	while (!checkLocation(latitude, longitude)) // wait until the device streams its location
	{
		Blynk.run();

		digitalWrite(ledPin, HIGH);
		delay(1000);
		digitalWrite(ledPin, LOW);
		delay(1000);
	}

	Serial.println("[INFO] Connected to Phone");
	digitalWrite(ledPin, HIGH);
}

void loop()
{
	Blynk.run();

	if (getButtonState(buttonPin) && checkLocation(latitude, longitude))
	{
		sendMessage(formatMessage(latitude, longitude));
	}

	delay(10);
}