#include <Arduino.h>

#include "NotreLibP2.h"
#include <ArduinoJson.h>

//ArduinoX AX_;
NotreLibP2 myLib_;

void setup() {
  myLib_.initRobot();
}

void loop() {
  // Lectures des entrées

  // Message disponible sur le buffer
  if(Serial.available() > 0){
    myLib_.readMsg();
  }

  

}