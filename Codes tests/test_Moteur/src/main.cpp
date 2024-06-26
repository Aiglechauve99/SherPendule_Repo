#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>

ArduinoX AX_;
MotorControl MC_;

void setup() {
  Serial.begin(115200);
  AX_.init();
  MC_.init(8,7);
  
  
}

void loop() {
  MC_.setSpeed(1.0);
}