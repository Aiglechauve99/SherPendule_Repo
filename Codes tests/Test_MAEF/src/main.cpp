#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>

ArduinoX AX_;

enum State {READY,PICK,APPROACH,SWING,OBSTACLE,STABILISE,DROP,RETURN};

/*
enum State next_state(enum State current)
{
  switch(current){
    case READY: return PICK;
    case PICK: return APPROACH;
    case APPROACH: return SWING;
    case SWING: return OBSTACLE;
    case OBSTACLE: return STABILISE;

  }
}*/

State transition;

switch(transition){
  case READY:
    //initialize the board and pins
    break;
  case PICK:
    //Activate magnet
    break;
  case APPROACH:
    //move to position defined
   break;
  case SWING:
    //oscilolate pendulum until desired anclge and frequency 
  case OBSTACLE:
   //pass the obstacle and get to the end place
   break;
  case STABILISE:
    //stabilise the pendulum for drop off
    break;
  case DROP:
    //deactivate the electromagnet to drop into basket
    break;
  case RETURN:
  //full speed back to the beginning
    break;
}






void setup() {
  Serial.begin(115200);
  AX_.init();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Ça fait flasher la led sur le Arduino
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}