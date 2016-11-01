#include "JsonStreamingParser.h"
#include "JsonListener.h"
#include "ExampleParser.h"

/*
 *  Simple HTTP get webclient test
 */
 
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

 
const char* ssid     = "gtwifi";
const char* password = "******";
 
const char* host = "m.gatech.edu";

JsonStreamingParser parser;
ExampleListener listener;

void setup() {
 
  Serial.begin(115200);
  //Serial.println(String(ESP.getFreeHeap()));
  parser.setListener(&listener);
  // put your setup code here, to run once:

  delay(100);
 
  // We start by connecting to a WiFi network
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
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
   
}

int value = 0;

void loop() {
  // put your main code here, to run repeatedly:
  delay(5000);
  ++value;
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
  String url = "/api/buses/position";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  
  delay(5000);

  while(client.available()) {
  // Read all the lines of the reply from server and print them to Serial
 // char json [300] ="";
    char line[2000];
    client.readBytesUntil(']', line, 2000);
    //char charBuf[4000];
    //String line = client.readStringUntil('\n');
    //Serial.println(line);
    
    //line.toCharArray(charBuf, 4000); 

    delay(1000);

     for (int i = 0; i < 2000; i++) {
     
       //Serial.println("Started parsing json from API... looking for Trolley");
       parser.parse(line[i]);
       if(listener.endRead == true){
         if(listener.routeValue == "green"){
            Serial.print("Trolley " + listener.idValue + ": ");
            listener.idValue = "";
            Serial.print("latitude = " + listener.busLat + " , ");
            listener.busLat = "";
            Serial.print("longitude = " + listener.busLon + " , ");
            listener.busLon = "";
            Serial.print("speed = " + listener.busSpeed);
            listener.busSpeed = "";
            Serial.println("");
            listener.endRead = false;
            listener.routeValue = " ";
        }
        if(listener.routeValue == "red"){
            Serial.print("Trolley " + listener.idValue + ": ");
            listener.idValue = "";
            Serial.print("latitude = " + listener.busLat + " , ");
            listener.busLat = "";
            Serial.print("longitude = " + listener.busLon + " , ");
            listener.busLon = "";
            Serial.print("speed = " + listener.busSpeed);
            listener.busSpeed = "";
            Serial.println("");
            listener.endRead = false;
            listener.routeValue = " ";
        }
        if(listener.routeValue == "trolley"){
            Serial.print("Trolley " + listener.idValue + ": ");
            listener.idValue = "";
            Serial.print("latitude = " + listener.busLat + " , ");
            listener.busLat = "";
            Serial.print("longitude = " + listener.busLon + " , ");
            listener.busLon = "";
            Serial.print("speed = " + listener.busSpeed);
            listener.busSpeed = "";
            Serial.println("");
            listener.endRead = false;
            listener.routeValue = " ";
        }
        if(listener.routeValue == "night"){
            Serial.print("Trolley " + listener.idValue + ": ");
            listener.idValue = "";
            Serial.print("latitude = " + listener.busLat + " , ");
            listener.busLat = "";
            Serial.print("longitude = " + listener.busLon + " , ");
            listener.busLon = "";
            Serial.print("speed = " + listener.busSpeed);
            listener.busSpeed = "";
            Serial.println("");
            listener.endRead = false;
            listener.routeValue = " ";
        }
        if(listener.routeValue == "emory"){
            Serial.print("Trolley " + listener.idValue + ": ");
            listener.idValue = "";
            Serial.print("latitude = " + listener.busLat + " , ");
            listener.busLat = "";
            Serial.print("longitude = " + listener.busLon + " , ");
            listener.busLon = "";
            Serial.print("speed = " + listener.busSpeed);
            listener.busSpeed = "";
            Serial.println("");
            listener.endRead = false;
            listener.routeValue = " ";
        }
      }
    }
  }
  
    //for (int i = 0; i < sizeof(charBuf); i++) {
       //parser.parse(charBuf[i]);
       // should be blank since parsing the json erases it
    //}
  //}
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
  }
}





