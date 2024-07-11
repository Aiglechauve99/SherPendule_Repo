#include <Arduino.h>

#include "NotreLibP2.h"
#include <ArduinoJson.h>

//ArduinoX AX_;
NotreLibP2 myLib_;
//VexQuadEncoder vex;
enum State {READY,PICK,APPROACH,SWING,OBSTACLE,STABILISE,DROP,RETURN} state;
unsigned long tempsAvant = 0;
bool compte = false;


void setup() {
  myLib_.initRobot();

  myLib_.vex.init(2,3);
  attachInterrupt(myLib_.vex.getPinInt(), []{myLib_.vex.isr();}, FALLING);
  myLib_.vex.reset();
  //state = READY;
  state = STABILISE;
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
      
      if(myLib_.msgRecu.etape == 1){
        state=PICK;
      }
      break;
      
    case PICK:
      Serial.println("State: PICK");
      //Activate magnet
      myLib_.controlMagnet(HIGH);
      if(!compte){
        tempsAvant = millis();
        compte = true;
      }

      if(millis()-tempsAvant >= 5000){
        state = APPROACH;
      }
      break;

    case APPROACH:
      Serial.println("State: APPROACH");
      //move to position defined
      if(!myLib_.avanceDe(0.5)){
        state = STABILISE;
      }
    break;

    case SWING:
      Serial.println("State: SWING");
      //oscilolate pendulum until desired anclge and frequency
      myLib_.oscillation();
      state = OBSTACLE;
      
      break;
    case OBSTACLE:
      Serial.println("State: OBSTACLE");
    //pass the obstacle and get to the end place
      while(1);
      
    break;
    case STABILISE:
      Serial.println("State: STABILISE");
      //stabilise the pendulum for drop off
      if(!myLib_.stabilise(90)){
        //state = SWING;
        Serial.println("Stable");
      }
      
      break;
    case DROP:
      Serial.println("State: DROP");
      //deactivate the electromagnet to drop into basket
      
      break;
    case RETURN:
      Serial.println("State: RETURN");
    //full speed back to the beginning
    
      break;
  }

}

