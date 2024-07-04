#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>

ArduinoX AX_;
JsonDocument doc;

void setup() {
  Serial.begin(115200);
  AX_.init();

  doc["Action"]="Etape 1";
  doc["Consigne"] = 5;
  
}

void loop() {
  
}