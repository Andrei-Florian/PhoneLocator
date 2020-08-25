// Universum | Universum Projects > phoneLocator

// Andrei Florian 10/FEB/2018

  // 1.1 Andrei Florian 10/SEP/2018
  
#include <stdio.h>
#include <Bridge.h>
#include <BlynkSimpleYun.h>

char auth[] = "5b15d89f12514c3e8eff04b6287e1335";   // TODO - insert Blynk auth token here
char userEmail[] = "lightpro64@gmail.com";            // TODO - insert your email here

float latitude;
float longitude;
double altitude;
double speed;

int i;
int proDebug = 1; // TODO - set to 0 if not debugging
int buttonPin = 2;
int online;

BLYNK_WRITE(V0) // read virtual pin 0 for co-ordinates
{
  latitude = param[0].asDouble();
  longitude = param[1].asDouble();
  altitude = param[2].asDouble();
  speed = param[3].asDouble();
}




void getLocation()
{
  if(digitalRead(buttonPin) == HIGH)
  {
    online = 1;
    if(proDebug == 1)
    {
      Serial.println("button pressed");
      Serial.println("Locating Phone");
      Serial.println("");
    }
  }
  
  if(i < 10) // if requested co-ordinates less than 10 times
  {
    if(online == 1)
    {
      if(latitude != 0.00 && longitude != 0.00) // if co-ordinates are valid
      {
        //String toSend is the string that will be sent as the emails body
        String toSend = "Phone Located! \n received co-ordinates \n LAT ";
        toSend += String(latitude, 8); // add vaiables to string
        toSend += " LNG ";
        toSend += String(longitude, 8);
        toSend += ". View location on Maps: ";
        toSend += "www.google.com/maps/?q=";
        toSend += String(latitude, 8);
        toSend += ",";
        toSend += String(longitude, 8);
        delay(500);

        if(proDebug == 1)
        {
          Serial.println("Phone geolocation found");
          Serial.println("Sending Email...");
          Serial.println(toSend);
        }
      
        Blynk.email("lightpro64@gmail.com", "PhoneLocator | Notification", toSend); // send email

        if(proDebug == 1)
        {
          Serial.println("Success");
          Serial.println("Email Sent");
          Serial.println("Restarting Protocol");
          Serial.println("");
        } 
      
        delay(5000);
        i = 0;
        online = 0;
      }
      else
      {
        if(i > 9)
        {
          if(proDebug == 1)
          {
            Serial.println("Error - Timeout Occured in Communication");
            delay(500);
            Serial.println("Restarting");
          }
    
          i = 0;
          online = 0;
          delay(1000);
        }
        else if(i < 9)
        {
          i++;
          delay(1000);
          if(proDebug == 1)
          {
            Serial.println("Geolocation not Received");
            Serial.println("Blynk responded with");
            Serial.println(latitude, 8);
            Serial.println(longitude, 8);
            Serial.println("");
          }
        }
      }
    }
  }
}





void setup()
{
  pinMode(buttonPin, INPUT);
  Blynk.begin(auth); // initialise Blynk
  delay(1000);
  Blynk.email("lightpro64@gmail.com", "PhoneLocator | Notification", "Project Online"); // send email

  if(proDebug == 1)
  {
    Serial.begin(9600);
    while(!Serial) {};
    delay(1000);
    Serial.println("phoneLocator Online");
    Serial.println("");
    Serial.println("For this project to work, ensure that");
    Serial.println("  Phone is connected to WiFi or Mobile Data");
    Serial.println("");
    Serial.println("");
    delay(500);
  }
}

void loop()
{
  Blynk.run();
  getLocation();
}
