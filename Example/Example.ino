/*
 * Changes Based on:
 * https://github.com/mike-rankin/ESP8266_OLED_Display
 */
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>

//#define SSID "GTOther" // insert your SSID
//#define PASS "GeorgeP@1927" // insert your password

#define SSID "1412" // insert your SSID
#define PASS "chacah12" // insert your password
#define host "m.gatech.edu"

const int buffer_size = 300;                         // length of json buffer

SoftwareSerial lcdSerial(4, 5); // RX, TX for debugging

const int buffer=300;
 

// Array of desired weather conditions 
// These must be in the order received from nextBus


char* conds[]={
  "\"id\":",
  "\"route\":",
  "\"lat\":",
  "\"lng\":",
  "\"plat\":",
  "\"plng\":",
  "\"speed\":",
  "\"jobID\":",
  "\"ts\":"
};

int num_elements = 9;  // number of conditions you are retrieving, count of elements in conds

unsigned long WMillis = 0;                           // temporary millis() register

void setup()
{  
  Serial.begin(115200);                              // baudrate of monitor
  WiFi.begin(SSID,PASS);                             // your WiFi Network's SSID & Password
  while (WiFi.status() != WL_CONNECTED) {            // DO until connected
    delay(500);                                      
    Serial.print(".");                               // print a few dots
  }
  
  Serial.println("");                            
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
   
  lcdSerial.begin(9600); // Print to LCD (or similar display)
  nextBus();
}


void loop()
{
  // Only check bus status every 10 seconds 
  if (millis()-WMillis >= 10000) {                // 300 seconds interval
    nextBus();                                // get new data
    WMillis=millis();                              // 
   }
}

void nextBus() 
{
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  String url = "/api/buses/position";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(5000);

//test to print everything
  char jsonEx [300] ="";
  while(client.available()){
    char const* line = client.readStringUntil('\r').c_str();
    //String line = client.readStringUntil('\r');
    Serial.print(line);
    strcat(jsonEx, line);
  }
//end of test 
  
  unsigned int i = 0;                                           // timeout counter
  //char json[buffer_size]="{";                                   // first character for json-string is begin-bracket 
  char json[buffer_size]="[";   
  int n = 1;                                                    // character counter for json  
  
  for (int j=0;j<num_elements;j++){                             // do the loop for every element/condition
    boolean quote = false; int nn = false;                      // if quote=fals means no quotes so comma means break
    while (!client.find(conds[j])){}                            // find the part we are interested in.
  
    String Str1 = conds[j];                                     // Str1 gets the name of element/condition
  
    for (int l=0; l<(Str1.length());l++)                        // for as many character one by one
    {
      json[n] = Str1[l];                                     // fill the json string with the name
      n++;
     }                                                  // character count +1
    while (i<5000) {                                            // timer/counter
      if(client.available()) {                                  // if character found in receive-buffer
        char c = client.read();                                 // read that character
           Serial.print("char is ");
           Serial.print(c);                                   // 
// ************************ construction of json string converting comma's inside quotes to dots ********************        
               if ((c=='"') && (quote==false))                  // there is a " and quote=false, so start of new element
                  {quote = true;nn=n;}                          // make quote=true and notice place in string
               if ((c==',')&&(quote==true)) {c='.';}            // if there is a comma inside quotes, comma becomes a dot.
               if ((c=='"') && (quote=true)&&(nn!=n))           // if there is a " and quote=true and on different position
                  {quote = false;}                              // quote=false meaning end of element between ""
               if((c==',')&&(quote==false)) break;              // if comma delimiter outside "" then end of this element
 // ****************************** end of construction ******************************************************
          json[n]=c;                                            // fill json string with this character
          n++;                                                  // character count + 1
          i=0;                                                  // timer/counter + 1
        }
        i++;                                                    // add 1 to timer/counter
      }                    // end while i<5000
     if (j==num_elements-1)                                     // if last element
        {}                                          // add end bracket of json string
     else                                                       // else
        {json[n]=',';}                                          // add comma as element delimiter
     n++;                                                       // next place in json string
  }
  Serial.println("json file is ");
  Serial.println(json);                                       // debugging json string 
  parseJSON(json);                                              // extract the conditions
  WMillis=millis();
}


void parseJSON(char json[300])
{
  StaticJsonBuffer<buffer> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  const char* routeID = root["route_id"];
  const char* stopID = root["stop_id"];  
  Serial.println(routeID);
  Serial.println(stopID);
}
