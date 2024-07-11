#include <Arduino.h>

#include "NotreLibP2.h"
#include <ArduinoJson.h>

//ArduinoX AX_;
NotreLibP2 myLib_;
enum State {READY,PICK,APPROACH,SWING,OBSTACLE,STABILISE,DROP,RETURN} state;

void setup() {
  myLib_.initRobot();
  state = READY;
}

void loop() {
  // Lectures des entrées

  // Message disponible sur le buffer
  if(Serial.available() > 0){
    myLib_.readMsg();
  }

  switch(state){
    case READY:
      Serial.println("State: READY");
      

      if(millis()>5000){
        state=PICK;
      }
      break;
      
    case PICK:
      Serial.println("State: PICK");
      //Activate magnet
      if(millis()>10000){
        state=APPROACH;
      }
      break;
    case APPROACH:
      Serial.println("State: APPROACH");
      //move to position defined
      if(millis()>15000){
        *state=SWING;
      }
    break;
    case SWING:
      Serial.println("State: SWING");
      //oscilolate pendulum until desired anclge and frequency
      if(millis()>20000){
        *state=OBSTACLE;
      } 
      break;
    case OBSTACLE:
      Serial.println("State: OBSTACLE");
    //pass the obstacle and get to the end place
      if(millis()>25000){
        *state=STABILISE;
      } 
    break;
    case STABILISE:
      Serial.println("State: STABILISE");
      //stabilise the pendulum for drop off
      if(millis()>30000){
        *state=DROP;
      } 
      break;
    case DROP:
      Serial.println("State: DROP");
      //deactivate the electromagnet to drop into basket
      if(millis()>35000){
        *state=RETURN;
      } 
      break;
    case RETURN:
      Serial.println("State: RETURN");
    //full speed back to the beginning
    if(millis()>40000){
        *state=PICK;
      } 
      break;
  }

}