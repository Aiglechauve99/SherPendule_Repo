#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>

ArduinoX AX_;
StaticJsonDocument<500> doc;
JsonVariant parse_msg;
int Action = 0;

void setup() {
  Serial.begin(115200);
  AX_.init();
  
  
}

void loop() {
  if(Serial.available() > 0){
    DeserializationError error = deserializeJson(doc, Serial);

    if (error){
      Serial.print("deserialize() failed: ");
      Serial.println(error.c_str());
    }

    parse_msg = doc["Action"];
    if(!parse_msg.isNull()){
      Action = doc["Action"].as<int>();
    }
  }
  
}