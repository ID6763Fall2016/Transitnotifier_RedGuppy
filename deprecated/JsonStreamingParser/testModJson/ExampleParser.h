#pragma once

#include "JsonListener.h"

class ExampleListener: public JsonListener {

  public:

    String keyValue;
    String idValue;
    String routeValue;
    String busLat;
    String busLon;
    String busSpeed;
    bool endRead;
    
    virtual void whitespace(char c);
  
    virtual void startDocument();

    virtual void key(String key);

    virtual void value(String value);

    virtual void endArray();

    virtual void endObject();

    virtual void endDocument();

    virtual void startArray();

    virtual void startObject();
};
