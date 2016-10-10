// -------------------------------------------------------------------------
// NextBus transit display using ESP8266 WiFi microcontroller and Adafruit
// 7-segment displays. Uses the following parts:
//   - Adafruit Feather HUZZAH with ESP8266 WiFi (Adafruit #2821) -OR-
//   - Adafruit HUZZAH ESP8266 Breakout (#2471) (requires FTDI cable)
//   - Adafruit 0.56" 4-Digit 7-Segment Display (878, 879, 880, 881 or 1002)
//     1 to 8 displays can be used, one per transit stop & line.
//
// Requires TinyXML library: https://github.com/adafruit/TinyXML
//
// This shows the next tracked & predicted arrival time for each transit
// line and stop listed below, one per display (or next 2 arrivals if both
// can fit on the 4-digit display with a space in-between).  As written,
// this discards arrivals below a certain time threshold (can't safely be
// reached in time) and shows only whole minutes; it does not count down
// seconds. Intended to be a convenience and not a stress-inducing thing.
// -------------------------------------------------------------------------

#include <ESP8266WiFi.h>
#include <TinyXML.h>
 
// CONFIGURABLE GLOBAL STUFF -----------------------------------------------

char ssid[] = "",     // WiFi network name
     pass[] = "", // WiFi network password
     host[] = "gtbuses.herokuapp.com";

#define POLL_INTERVAL 2 // Time between searches (minutes)
#define MIN_TIME      5 // Skip arrivals sooner than this (minutes)
#define READ_TIMEOUT 15 // Cancel query if no data received (seconds)
 
// For basic use as-is, shouldn't need to edit below this line -------------

 

WiFiClient client;
TinyXML    xml;
uint8_t    buffer[150]; // For XML decoding


void setup(void) {
  Serial.begin(115200);
  Serial.println("GTBus Tracker");
  xml.init((uint8_t *)buffer, sizeof(buffer), &XML_callback);
 
}

void loop()
{  
  int c;
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/predictions/trolley";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(5000);
       
  while(client.connected()) {
     if((c = client.read()) >= 0) {
      
        char c = client.read();
        xml.processChar(c);
        Serial.print(c);
      
     }
  }
    client.stop();
    Serial.println();
  
}

void XML_callback( uint8_t statusflags, char* tagName,  uint16_t tagNameLen,  char* data,  uint16_t dataLen )
{
 
  if((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "seconds")) {
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


