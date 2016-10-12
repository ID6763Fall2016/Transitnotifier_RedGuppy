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

const char* ssid = "";
const char* password = "";

const char* host = "gtbuses.herokuapp.com";
const int httpsPort = 443;

TinyXML    xml;
uint8_t    buffer[150]; // For XML decoding

// Use web browser to view and copy
 
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

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpsPort)) {
    Serial.println("connection failed");
    return;
  }
 

  String url = "/predictions/trolley";
  Serial.print("requesting URL: ");
  Serial.println(url);
  
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");
               
  Serial.println("request sent");
  while (client.connected()) {
    if (client.available() > 0) {
      char c = client.read();
      xml.processChar(c);
      Serial.print(c);
    }
  }
  
 
  Serial.println("==========");
  Serial.println("closing connection");
}

void loop() {
}

void XML_callback( uint8_t statusflags, char* tagName,  uint16_t tagNameLen,  char* data,  uint16_t dataLen )
{

  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "seconds")) {
    uint32_t t = atoi(data); // Prediction in seconds (0 if gibberish)
    Serial.print(t);
    Serial.println(" seconds");
  }

  if (statusflags & STATUS_START_TAG)
  {
    if ( tagNameLen )
    {
      Serial.print("Start tag ");
      Serial.println(tagName);
    }
  }
  else if  (statusflags & STATUS_END_TAG)
  {
    Serial.print("End tag ");
    Serial.println(tagName);
  }
  else if  (statusflags & STATUS_TAG_TEXT)
  {
    Serial.print("Tag:");
    Serial.print(tagName);
    Serial.print(" text:");
    Serial.println(data);
  }
  else if  (statusflags & STATUS_ATTR_TEXT)
  {
    Serial.print("Attribute:");
    Serial.print(tagName);
    Serial.print(" text:");
    Serial.println(data);
  }
  else if  (statusflags & STATUS_ERROR)
  {
    Serial.print("XML Parsing error  Tag:");
    Serial.print(tagName);
    Serial.print(" text:");
    Serial.println(data);
  }


}
