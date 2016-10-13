/*
 *  HTTP over TLS (HTTPS) example sketch
 *
 *  This example demonstrates how to use
 *  WiFiClientSecure class to access HTTPS API.
 *  We fetch and display the status of
 *  esp8266/Arduino project continuous integration
 *  build.
 *
 *  Created by Ivan Grokhotkov, 2015.
 *  This example is in public domain.
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <TinyXML.h>
#include <Adafruit_NeoPixel.h>

#define PIN            14    // Node-MCU Input Pin
#define NUMPIXELS      60    // Number of Pixels on the Ring
#define BRIGHTNESS     30    // Brightness between 0 - 255

byte pixelColorRed, pixelColorGreen, pixelColorBlue;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "";
const char* password = "";

const char* host = "gtbuses.herokuapp.com";
const int httpsPort = 443;

TinyXML    xml;
uint8_t    buffer[150]; // For XML decoding

// Use web browser to view and copy
     // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  
void setup() {
  int c;
  
  Serial.begin(115200);
  Serial.println("GTBus Tracker");
  xml.init((uint8_t *)buffer, sizeof(buffer), &XML_callback);

  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  pixels.begin();
  pixels.show();
  
  pixels.setBrightness(BRIGHTNESS);
}

void loop() {

  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
 

  String url = "/multiPredictions?stops=night|cloucomm";
  Serial.print("requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: trolleyStopClockESP8266\r\n" +
               "Connection: close\r\n\r\n");
               
  Serial.println("request sent");
  while (client.connected()) {
    if (client.available() > 0) {
      char c = client.read();
      xml.processChar(c);
      //Serial.print(c);
    }
  }
  
 
  Serial.println("==========");
  Serial.println("closing connection");
  //client.stop();

  
  delay(100);
}

void colorWipe(uint32_t c, uint8_t wait) {
      for(uint16_t i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, c);
      pixels.show();
      delay(wait);
      }
    }
    
void XML_callback( uint8_t statusflags, char* tagName,  uint16_t tagNameLen,  char* data,  uint16_t dataLen )
{
  int myPredictions[6];

  int myNum = 0;
  
  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "minutes")) {
    uint32_t t = atoi(data); // Prediction in seconds (0 if gibberish)
    myPredictions[myNum] = t;
    Serial.print(t);
    Serial.println(" minutes");

    for(uint8_t i=0; i<pixels.numPixels(); i++) {
     
        if (i == 60-t) {
          pixelColorRed = 150;
          pixelColorGreen = 150; 
          pixelColorBlue = 0; 
        }
        else {
          pixelColorRed = 0;
          pixelColorBlue = 0; 
          pixelColorGreen = 0; 
        }
     
        pixels.setPixelColor(i, pixels.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
      }
     pixels.show();
  }

  if (statusflags & STATUS_START_TAG)
  {
    if ( tagNameLen )
    {
      myNum += 1;
      //Serial.print("Start tag ");
      //Serial.println(tagName);
    }
  }
  else if  (statusflags & STATUS_END_TAG)
  {
      myNum = 0;
      for(int i= 0; i < sizeof(myPredictions); i++) {
        if(myPredictions[i] > 1 && myPredictions[i] < 60) {
          pixelColorRed = 150;
          pixelColorGreen = 150;
          pixelColorBlue = 20;
          Serial.println(myPredictions[i]);
          pixels.setPixelColor((60 - myPredictions[i]), pixels.Color(pixelColorRed, pixelColorGreen, pixelColorBlue));
        }
      }
      
    //Serial.print("End tag ");
    //Serial.println(tagName);
  }
  else if  (statusflags & STATUS_TAG_TEXT)
  {
    //Serial.print("Tag:");
    //Serial.print(tagName);
    //Serial.print(" text:");
    //Serial.println(data);
  }
  else if  (statusflags & STATUS_ATTR_TEXT)
  {
    //Serial.print("Attribute:");
    //Serial.print(tagName);
    //Serial.print(" text:");
    //Serial.println(data);
  }
  else if  (statusflags & STATUS_ERROR)
  {
    Serial.print("XML Parsing error  Tag:");
    Serial.print(tagName);
    Serial.print(" text:");
    Serial.println(data);
  }


}
