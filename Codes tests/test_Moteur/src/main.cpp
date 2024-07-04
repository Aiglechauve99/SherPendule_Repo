#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>

ArduinoX AX_;
MotorControl MC_;

void setup() {
  Serial.begin(115200);
  AX_.init();
  MC_.init(6,31);
  MC_.setSpeed(-0.5);
  
}

void loop() {
  
}