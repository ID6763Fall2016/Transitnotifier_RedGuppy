/*
 *  Simple HTTP get webclient test
 */
 
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
 
const char* ssid     = "1412";
const char* password = "chacha12";
 
const char* host = "m.gatech.edu";
 
void setup() {
  Serial.begin(115200);
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
  delay(5000);
  ++value;
 
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
  String url = "/api/buses/stop";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(5000);
  
  // Read all the lines of the reply from server and print them to Serial
 // char json [300] ="";
  while(client.available()){
    //char const* line = client.readStringUntil('\r').c_str();
    char charBuf[1000];
    String line = client.readStringUntil('\n');
    
    line.toCharArray(charBuf, 1000); 
   // strcat(json, line);
    //char* s = client.readStringUntil('\r').c_str();
  //  char* token = strtok(s, "},");
   // while (token) {
    //  printf("token: %s\n", token);
   //   token = strtok(NULL, "},");
  //  }
      

 
  char *p = charBuf;
  p++; /* 'N' is not in `p` */
  p[strlen(p)-1] = 0; 
 //Serial.print(p);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(p);
  //Serial.println(root);
  const char* id = root["route_id"];
  Serial.println(id);
  }
   
  Serial.println("closing connection");
}
