#include <Arduino.h>

#include "NotreLibP2.h"
#include <ArduinoJson.h>

//ArduinoX AX_;
NotreLibP2 myLib_;
//VexQuadEncoder vex;
enum State {READY,PICK,APPROACH,SWING,STOP,STABILISE,DROP, GOHOME, MESURE} state;
unsigned long tempsAvant = 0;
bool compte = false;
float position = 0;


void setup() {
  myLib_.initRobot();

  myLib_.vex.init(2,3);
  attachInterrupt(myLib_.vex.getPinInt(), []{myLib_.vex.isr();}, FALLING);
  myLib_.vex.reset();
  state = PICK;
  //state = MESURE;
  //state = STABILISE;
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
        compte = false;
        state = APPROACH;
      }
      break;

    case APPROACH:
      Serial.println("State: APPROACH");
      // Position pour oscillation 1
      /*
      if(!myLib_.avanceDe(0.3, 0.4)){
        state = SWING;
      }
      */
     // Position pour oscillation 2
     if(!myLib_.avanceDe(0.6, 0.4)){
        myLib_.AX_.setMotorPWM(0,0);
        delay(100);
        state = SWING;
      }

    break;

    case SWING:
      Serial.println("State: SWING");
      //oscilolate pendulum until desired anclge and frequency
      //myLib_.oscillation();
      myLib_.oscillation2();
      state = STABILISE;
      //state = STOP;
      
      break;

    case STOP:
      Serial.println("State: OBSTACLE");
      myLib_.AX_.setMotorPWM(0,0);
    //pass the obstacle and get to the end place
      while(1);
      
    break;

    case STABILISE:
      Serial.println("State: STABILISE");
      /*
      while(1){
        myLib_.stabilise(90);
      }*/

      //stabilise the pendulum for drop off
      if(!myLib_.stabilise(90)){
        state = DROP;
        Serial.println("Stable");
      }
      
      break;

    case DROP:
      Serial.println("State: DROP");
      position = myLib_.EncodeurOptiPos();
      Serial.println(position);
      myLib_.avanceDe((1.25), 0.4);
      delay(500);
      myLib_.controlMagnet(LOW);
      state = GOHOME;
      break;

    case GOHOME:
      Serial.println("State: GOHOME");
      //Serial.println(myLib_.getAngle());
      if(!myLib_.avanceDe(0, 0.7)){
        state = PICK;
      }
      break;

     case MESURE:
      Serial.println("State: MESURE");
      //Serial.println(myLib_.getAngle());
      Serial.println(myLib_.IMU_.getGyroscopeY());
      delay(100);
    
      break;
  }

}

