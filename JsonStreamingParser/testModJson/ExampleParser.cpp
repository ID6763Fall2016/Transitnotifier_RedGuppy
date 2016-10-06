#include "ExampleParser.h"
#include "JsonListener.h"

extern String keyValue;
extern String idValue;
extern String routeValue;
extern String busLat;
extern String busLon;
extern String busSpeed;
extern bool endRead = false;

void ExampleListener::whitespace(char c) {
  //Serial.println("whitespace");
}

void ExampleListener::startDocument() {
  //Serial.println("start document");
  endRead = false;
}

void ExampleListener::key(String key) {
  //Serial.println("Changed Key: " + key);
  keyValue = key;
  endRead = false;
}

void ExampleListener::value(String value) {
  //Serial.println("value: " + value);
  if(keyValue == "id"){
      idValue = value;
  }else if(keyValue == "route"){
      routeValue = value;
  }else if(keyValue == "lat"){
      busLat = value;
  }else if(keyValue == "lng"){
      busLon = value;
  }else if(keyValue == "speed"){
      busSpeed = value;
  }
  endRead = false;
}

void ExampleListener::endArray() {
  //Serial.println("end array. ");
  endRead = false;
}

void ExampleListener::endObject() {
  //Serial.println("end object. ");
  endRead = true;
}

void ExampleListener::endDocument() {
  //Serial.println("end document. ");
  endRead = false;
}

void ExampleListener::startArray() {
   //Serial.println("start array. ");
   endRead = false;
}

void ExampleListener::startObject() {
   //Serial.println("start object. ");
   endRead = false;
}

