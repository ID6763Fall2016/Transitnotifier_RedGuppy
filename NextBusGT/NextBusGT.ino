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

byte pixelColorRed, pixelColorGreen, pixelColorBlue, storePixRed, storePixGreen, storePixBlue;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

const char* ssid = "";
const char* password = "";

const char* host = "gtbuses.herokuapp.com";

const char* trolleyString = "/multiPredictions?stops=trolley|techsqua";
const char* techString = "/multiPredictions?stops=tech|techsqua";
const char* nightString = "/multiPredictions?stops=night|cloucomm";

const int httpsPort = 443;

TinyXML    xml;
uint8_t    buffer[150]; // For XML decoding

int serverDelay = 10;
int myCount = 0;

int myPredictions[10];

// setup button pin
const int  buttonPin = 15; 

// Variables will change:
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

// Use web browser to view and copy
     // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
  
void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  
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

  storePixRed = 170;
  storePixGreen = 100;
  storePixBlue = 0;

  parseServer(serverDelay);
}

uint8_t myCountdown = 0;

void loop() {
  // read the pushbutton input pin:
  buttonState = digitalRead(buttonPin);
 
  // compare the buttonState to its previous state
  if (buttonState != lastButtonState) {
    // if the state has changed, increment the counter
    if (buttonState == HIGH) {
      // if the current state is HIGH then the button
      // wend from off to on:
      if(buttonPushCounter <= 3) {
        buttonPushCounter++;
        Serial.println("on");
        Serial.print("number of button pushes:  ");
        Serial.println(buttonPushCounter);
        if(buttonPushCounter == 1) {
          storePixRed = 170;
          storePixGreen = 100;
          storePixBlue = 0;
        }else if(buttonPushCounter == 2) {
          storePixRed = 0;
          storePixGreen = 0;
          storePixBlue = 200;
        }else if(buttonPushCounter == 3) {
          storePixRed = 170;
          storePixGreen = 0;
          storePixBlue = 170;
        }else if(buttonPushCounter > 3) {
          storePixRed = 170;
          storePixGreen = 100;
          storePixBlue = 0;
          buttonPushCounter = 0;
        }
      }
      
    } else {
      // if the current state is LOW then the button
      // wend from on to off:
      Serial.println("off");
    }
    // Delay a little bit to avoid bouncing
    delay(50);
  }
  // save the current state as the last state,
  //for next time through the loop
  lastButtonState = buttonState;
  

  // get the estimates for the next bus and place them value in the myPredictions array
  parseServer(serverDelay);

  //for the number of pixels we will set a color based on the estimated time of the next bus or trolley
  for(uint8_t i=0; i<pixels.numPixels(); i++) {

       // as a default make the pixel dark
       pixelColorRed = 0;
       pixelColorBlue = 0; 
       pixelColorGreen = 0;

       // parse through the list of predictions to determine which pixels should be turned on
       for(uint8_t j=0; j<10; j++) {
       
          if(60-myPredictions[0] < (60-1)){
            myCountdown = 0;
            if (myPredictions[j] != 0 && i >= (60-myPredictions[j])-1 && i <= (60-myPredictions[j]) + 1) {
              pixelColorRed = storePixRed;
              pixelColorGreen = storePixGreen; 
              pixelColorBlue = storePixBlue; 
            }
          }else{
              if(i > myCountdown) {
                pixelColorRed = storePixRed;
                pixelColorGreen = storePixGreen; 
                pixelColorBlue = storePixBlue;
              }else {
                pixelColorRed = 0;
                pixelColorGreen = 0; 
                pixelColorBlue = 0;
              }
              if(myCountdown > 60){
                myCountdown = 0;
              }
          }
          
      }
     
     
       pixels.setPixelColor(i, pixels.Color(pixelColorRed, pixelColorGreen, pixelColorBlue)); 
  }
     myCountdown ++;
     pixels.show();

     delay(1000);
     
}


void parseServer(int myDelay){
  if(myCount < myDelay){
      myCount += 1;
  } else {
     Serial.print("connecting to ");
     Serial.println(host);
     if (!client.connect(host, httpsPort)) {
        Serial.println("connection failed");
        return;
     }
 

    //String url = "/multiPredictions?stops=night|cloucomm";
    Serial.print("requesting URL: ");
    if(buttonPushCounter <= 1 || buttonPushCounter > 3) {
      const char* url = trolleyString;
      Serial.println(url);
    
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "User-Agent: trolleyStopClockESP8266\r\n" +
                  "Connection: close\r\n\r\n");
                 
      Serial.println("request sent");
    }else if (buttonPushCounter == 2) {
      const char* url = techString;
      Serial.println(url);
    
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "User-Agent: trolleyStopClockESP8266\r\n" +
                  "Connection: close\r\n\r\n");
                 
      Serial.println("request sent");
    }else if (buttonPushCounter == 3) {
      const char* url = nightString;
      Serial.println(url);
    
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "User-Agent: trolleyStopClockESP8266\r\n" +
                  "Connection: close\r\n\r\n");
                 
      Serial.println("request sent");
    }
    
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
    myCount = 0;
  }
}

/*
void colorWipe(uint32_t c, uint8_t wait) {
      for(uint16_t i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, c);
      pixels.show();
      delay(wait);
      }
    }
*/
int myNum = 0;

void XML_callback( uint8_t statusflags, char* tagName,  uint16_t tagNameLen,  char* data,  uint16_t dataLen )
{
  /*
  for (int i = 0; i<sizeof(myPredictions); i++) {
    myPredictions[i] = 0;
  }*/

  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "copyright")) {
    myNum = 0;
  }
  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "minutes")) {
    int32_t t = atoi(data); // Prediction in seconds (0 if gibberish)
    //Serial.println(t);
    //Serial.println(tagName);
    
    myPredictions[myNum] = t;
    String myTestString = String(myPredictions[myNum]);
    Serial.println("here's stored value " + String(myNum) +" : " + myTestString);


    
    //Serial.print(t);
    //Serial.println(" minutes");

  }
  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "dirTag")) {
    myNum++;
    Serial.println(myNum);

  }
  

  if (statusflags & STATUS_START_TAG)
  {
    if ( tagNameLen )
    {
      //Serial.print("Start tag ");
      //Serial.println(tagName);
    }
  }
  else if  (statusflags & STATUS_END_TAG)
  {

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
