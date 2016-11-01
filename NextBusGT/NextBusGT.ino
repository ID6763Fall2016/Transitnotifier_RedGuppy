/*
 *  HTTP over TLS (HTTPS) example sketch
 *  This program uses the Nextbus api at gtbuses.herokuapp.com to map the bus arrival times
 *  to a clock.  The idea is to present a countdown to the next bus for specific stops and show
 *  when other busses might arrive.  Each route is given a color and the next busses that are
 *  estimated to arrive within an hour are displayed.  More information can be found at...
 *  http://ipdl.gatech.edu/doie2016/projects/transit-notifier/buzz-clock-by-red-guppy/
 *  authors: colin freeman and Edith Li
 */

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// we use the TinyXML library to parse the xml returned from the nextbus api
#include <TinyXML.h>

// we use Adafruit NeoPixels to display the busses on the clock and change their colors
#include <Adafruit_NeoPixel.h>

// control pin for NeoPixels is set to 14 and there are 60 neopixels, one for each second

#define PIN            14    // Node-MCU Input Pin
#define NUMPIXELS      60    // Number of Pixels on the Ring
#define BRIGHTNESS     30    // Brightness between 0 - 255

// setup placeholders for the RGB colors for each neopixel and the maximum number of routes that can be displayed at once
byte pixelColorRed, pixelColorGreen, pixelColorBlue;
byte onePixRed, onePixGreen, onePixBlue, twoPixRed, twoPixGreen, twoPixBlue, threePixRed, threePixGreen, threePixBlue;

// initialise the neopixels
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

//  IMPORTANT ................................
// make sure you input your ssid and password for the wifi where the clock will be displayed
//
const char* ssid = "YOUR_WIFI_SSID_HERE";
const char* password = "YOUR_WIFI_PASSWORD_HERE";

// setup a variable to contain the site string for the api you want to use.
const char* host = "gtbuses.herokuapp.com";

// setup variables to contain the stops and routes you want to get predictions for.
// see http://gtbuses.herokuapp.com for more information on the api

const char* trolleyString = "/multiPredictions?stops=trolley|techsqua&&stops=tech|techsqua";
//const char* techString = "/multiPredictions?stops=trolley|techsqua&&stops=tech|techsqua";
//const char* blueString = "/multiPredictions?stops=blue|studcentr&&stops=red|centrstud&&stops=green|studcent";
const char* redString = "/multiPredictions?stops=blue|studcentr&&stops=red|centrstud&&stops=green|studcent";
//const char* greenString = "/multiPredictions?stops=blue|studcentr&&stops=red|centrstud&&stops=green|studcent";
const char* nightString = "/multiPredictions?stops=night|cloucomm";

// variable for the port to use
const int httpsPort = 443;

// setup a variable to contain the xml parser
TinyXML    xml;
uint8_t    buffer[150]; // For XML decoding

// creat an int to use for a delay on the server parsing call.  This is a workaround to avoid setting a long
// delay in the loop, which would slow down other functions

int serverDelay = 10;

// setup a count variable for parsing the xml into different slots in the myPredictions array
int myCount = 0;

// create an array to hold the time predictions.  We setup 15 slots 5 for each route.  This could omit the longest
// time for each route if there are more than 6 busses set to arrive for that route in less than an hour, but in testing
// this proved not to be the case.
int myPredictions[15];

// setup button pin to enable toggling between stops and routes. Sort of a mode switch
const int  buttonPin = 15; 

// Variables for counting the button clicks
int buttonPushCounter = 0;   // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

// Setup a secure wifi client
  WiFiClientSecure client;
  
void setup() {
  // initialize the button pin as a input:
  pinMode(buttonPin, INPUT);
  
  int c;

  // setup the serial connection and init the XML parser
  Serial.begin(115200);
  Serial.println("GTBus Tracker");
  xml.init((uint8_t *)buffer, sizeof(buffer), &XML_callback);

  // connect to the wifi
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

  // initialize the neopixels
  pixels.begin();
  pixels.show();
  
  pixels.setBrightness(BRIGHTNESS);

  // setup the default color values for the neopixels.  This is changed later based on the routes we parse in the xml
  onePixRed = 170;
  onePixGreen = 100;
  onePixBlue = 0;
  twoPixRed = 170;
  twoPixGreen = 100;
  twoPixBlue = 0;
  threePixRed = 170;
  threePixGreen = 100;
  threePixBlue = 0;

  // parse the server one time in order to get the ball rolling.  We could wait and do this later, but this should provide some
  // information to the clock quicker.  Otherwise we have to wait for a second delay loop, which adds a few seconds.
  parseServer(serverDelay);
}

// setup a countdown variable to control the timers
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
        // increment the counter
        buttonPushCounter++;
        Serial.println("on");
        Serial.print("number of button pushes:  ");
        Serial.println(buttonPushCounter);
        
        // if the button = 1 then the route and colors should be set for the trolley and the tech express at tech square
        if(buttonPushCounter == 1) {
          onePixRed = 170;
          onePixGreen = 100;
          onePixBlue = 0;
          twoPixRed = 100;
          twoPixGreen = 200;
          twoPixBlue = 0;
          threePixRed = 0;
          threePixGreen = 0;
          threePixBlue = 0;
          
          // if the route = 2 then the colors are setup for red,green, and blue routes to the student center
        }else if(buttonPushCounter == 2) {
          onePixRed = 200;
          onePixGreen = 0;
          onePixBlue = 0;
          twoPixRed = 0;
          twoPixGreen = 200;
          twoPixBlue = 0;
          threePixRed = 0;
          threePixGreen = 0;
          threePixBlue = 200;
          
          // if the route = 3 then the route is setup for night bus at clough commons
        }else if(buttonPushCounter == 3) {
          onePixRed = 170;
          onePixGreen = 0;
          onePixBlue = 170;
          twoPixRed = 170;
          twoPixGreen = 0;
          twoPixBlue = 170;
          threePixRed = 170;
          threePixGreen = 0;
          threePixBlue = 170;
          
          // if route is greater than 3 then we reset the values to the trolley at tech square.  Other routes could be added
          // here but this is enough to show how the process works
        }else if(buttonPushCounter > 3) {
          onePixRed = 170;
          onePixGreen = 100;
          onePixBlue = 0;
          twoPixRed = 100;
          twoPixGreen = 200;
          twoPixBlue = 0;
          threePixRed = 170;
          threePixGreen = 100;
          threePixBlue = 0;
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
       for(uint8_t j=0; j<15; j++) {

          // there are 60 pixels and 60 minutes to consider in an hour so we use that as a way to parse through the pixels and times
          // we also add one minute to the estimate to give people time to get to the stop.  this is done in the 60-1 value
          if(60-myPredictions[0] < (60-1)){

            // assuming the bus isn't arriving then we set my countdown to 0 and set the color pixel values
            // we use the j value of 0-4 5-9 and 10-14 to check which predictions we are parsing so we can apply the correct
            // color to the correct pixel
            myCountdown = 0;
            if (myPredictions[j] != 0 && j<5 && i >= (60-myPredictions[j])-1 && i <= (60-myPredictions[j]) + 1) {
              pixelColorRed = onePixRed;
              pixelColorGreen = onePixGreen; 
              pixelColorBlue = onePixBlue; 
            }else if (myPredictions[j] != 0 && j>4 && j<10 && i >= (60-myPredictions[j])-1 && i <= (60-myPredictions[j]) + 1) {
              pixelColorRed = twoPixRed;
              pixelColorGreen = twoPixGreen; 
              pixelColorBlue = twoPixBlue; 
            }else if (myPredictions[j] != 0 && j>9 && j<15 && i >= (60-myPredictions[j])-1 && i <= (60-myPredictions[j]) + 1) {
              pixelColorRed = threePixRed;
              pixelColorGreen = threePixGreen; 
              pixelColorBlue = threePixBlue; 
            }

            // if a bus is arriving we start a 2 minute countdown animation that will alert people to which bus is arriving at the stop
          }else{
              if(i > myCountdown && j<5) {
                pixelColorRed = onePixRed;
                pixelColorGreen = onePixGreen; 
                pixelColorBlue = onePixBlue;
              }else if(i > myCountdown && j>4 && j<10) {
                pixelColorRed = twoPixRed;
                pixelColorGreen = twoPixGreen; 
                pixelColorBlue = twoPixBlue;
              }else if(i > myCountdown && j>9 && j<15) {
                pixelColorRed = threePixRed;
                pixelColorGreen = threePixGreen; 
                pixelColorBlue = threePixBlue;
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
     
       // set the colors for all pixels on the neopixels
       pixels.setPixelColor(i, pixels.Color(pixelColorRed, pixelColorGreen, pixelColorBlue)); 
  }
    // increment the countdown
     myCountdown ++;
     pixels.show();

     // delay for 1 second.  This is mainly for the bus arriving countdown.
     delay(1000);
     
}


// function to parse the server with a small delay added to the 1 second delay in the loop.  This comes out to 10 seconds per server
// parse call
void parseServer(int myDelay){
  
  // if the delay threshold isn't met then keep delaying
  if(myCount < myDelay){
      myCount += 1;

  // else start connecting
  } else {
     Serial.print("connecting to ");
     Serial.println(host);
     if (!client.connect(host, httpsPort)) {
        Serial.println("connection failed");
        return;
     }
 

    //String url is set to the appropriate string for the current button mode
    Serial.print("requesting URL: ");
    if(buttonPushCounter <= 1 || buttonPushCounter > 3) {
      const char* url = trolleyString;
      Serial.println(url);

    // call to the server with the request
      client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "User-Agent: trolleyStopClockESP8266\r\n" +
                  "Connection: close\r\n\r\n");
                 
      Serial.println("request sent");
    }else if (buttonPushCounter == 2) {
      const char* redurl = redString;
      Serial.println(redurl);
    
      client.print(String("GET ") + redurl + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "User-Agent: trolleyStopClockESP8266\r\n" +
                  "Connection: close\r\n\r\n");
                 
      Serial.println("request sent");
    }else if (buttonPushCounter == 3) {
      const char* nighturl = nightString;
      Serial.println(nighturl);
    
      client.print(String("GET ") + nighturl + " HTTP/1.1\r\n" +
                  "Host: " + host + "\r\n" +
                  "User-Agent: trolleyStopClockESP8266\r\n" +
                  "Connection: close\r\n\r\n");
                 
      Serial.println("request sent");
    }

   // process the characters returned in the xml while the client is connected
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

// placeholder variables for parsing the xml to the predictions array
int myNum = 0;
String myRoute = "";


// when xml is recieved parse it to determine the tags and the values
void XML_callback( uint8_t statusflags, char* tagName,  uint16_t tagNameLen,  char* data,  uint16_t dataLen )
{

// parsed agencyTitle to determine if we are on a new route
  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "agencyTitle")) {
    myNum = 0;
  }
  // parsed routeTag to get the route information
  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "routeTag")) {
    myRoute = data;
  }

// parsed the minutes tag to get actual estimates and place them within the prediction array
  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "minutes")) {
        int32_t t = atoi(data); // Prediction in seconds (0 if gibberish)

        if(myRoute == "trolley"){
          myPredictions[myNum] = t;
          String myTestString = String(myPredictions[myNum]);
          Serial.println("here's stored value " + String(myNum) +" : " + myTestString);
        }else if(myRoute == "tech"){
          myPredictions[myNum+5] = t;
          String myTestString = String(myPredictions[myNum+5]);
          Serial.println("here's stored value " + String(myNum+5) +" : " + myTestString);
        }else if(myRoute == "red"){
          myPredictions[myNum] = t;
          String myTestString = String(myPredictions[myNum]);
          Serial.println("here's stored value " + String(myNum) +" : " + myTestString);
        }else if(myRoute == "green"){
          myPredictions[myNum+5] = t;
          String myTestString = String(myPredictions[myNum+5]);
          Serial.println("here's stored value " + String(myNum+5) +" : " + myTestString);
        }else if(myRoute == "blue"){
          myPredictions[myNum+10] = t;
          String myTestString = String(myPredictions[myNum+10]);
          Serial.println("here's stored value " + String(myNum+10) +" : " + myTestString);
        }else if(myRoute == "night"){
          myPredictions[myNum] = t;
          String myTestString = String(myPredictions[myNum]);
          Serial.println("here's stored value " + String(myNum) +" : " + myTestString);
        }
        
    
    }

// parsed the dirTag to determine when to increment to a new slot in the predictions array.  basically
// at the end of each prediction time
  if ((statusflags & STATUS_ATTR_TEXT) && !strcasecmp(tagName, "dirTag")) {
    myNum++;
    Serial.println(myNum);

  }
  

// the rest are other ways to parse which aren't actually used in this program
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
