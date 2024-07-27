#include <Arduino.h>

#include "NotreLibP2.h"
#include <ArduinoJson.h>

NotreLibP2 myLib_;

enum State {READY,PICK,APPROACH,SWING,STABILISE,DROP, GOHOME,STOP, MESURE, SEND} state, previousState;
unsigned long tempsAvant = 0;
unsigned long tempsAvantMsg = 0;
unsigned long tempsAvantCmptr = 0;
bool compte = false;
float position = 0;


void setup() {
  myLib_.initRobot();

  myLib_.vex.init(2,3);
  attachInterrupt(myLib_.vex.getPinInt(), []{myLib_.vex.isr();}, FALLING);
  myLib_.vex.reset();
  //state = PICK;
  //state = MESURE;
  //state = SEND;
  state = READY;
  myLib_.msgAEnvoyer.etape = READY;
}

void loop() {

  // Message disponible sur le buffer
  if(Serial.available() > 0){
    myLib_.readMsg();
    if(!myLib_.getDemarrage()){
      state = STOP;
    }
  }

  // Envoie des messages
  if((millis()-tempsAvantMsg > 500) && myLib_.getDemarrage()){
    tempsAvantMsg = millis();
    myLib_.getDataPourMessage();
    myLib_.sendMsg();

  }

  if((millis()-tempsAvantCmptr > 50)){
    tempsAvantCmptr = millis();
    myLib_.calculDistance();

  }


  switch(state){
    case READY:
      //Serial.println("State: READY");
      
      if(myLib_.getDemarrage() == 1){
        if(previousState == DROP){
          Serial.println("State: PICK");
          state=PICK;
          previousState = READY;
          myLib_.msgAEnvoyer.etape = PICK;
        }
        else{
          Serial.println("State: GOHOME");
          state=GOHOME;
          previousState = READY;
          myLib_.msgAEnvoyer.etape = GOHOME;
        }
      }
      break;
      
    case PICK:
      //Serial.println("State: PICK");
      myLib_.controlMagnet(HIGH);
      if(!compte){
        tempsAvant = millis();
        compte = true;
      }

      if(millis()-tempsAvant >= 3000){
        compte = false;
        Serial.println("State: APPROACH");
        state = APPROACH;
        previousState = PICK;
        myLib_.msgAEnvoyer.etape = APPROACH;
      }
      break;

    case APPROACH:
      //Serial.println("State: APPROACH");
      // Position pour oscillation 1
      /*
      if(!myLib_.avanceDe(0.3, 0.4)){
        state = SWING;
      }
      */

      // Position pour oscillation 2
      if(!myLib_.avanceDe(0.45, 0.4)){
        myLib_.AX_.setMotorPWM(0,0);
        delay(100);
        state = SWING;
        myLib_.msgAEnvoyer.etape = SWING;
      }

      break;

    case SWING:
      //Serial.println("State: SWING");

      //myLib_.oscillation();
      if(!myLib_.oscillation2()){
        Serial.println("State: STABILISE");
        state = STABILISE;
        previousState = SWING;
        myLib_.msgAEnvoyer.etape = STABILISE;
      }
      //state = STOP;
      
      break;

    case STABILISE:
      //Serial.println("State: STABILISE");
      /*
      while(1){
        myLib_.stabilise(90);
      }*/

     

      if(!myLib_.stabilise(90)){
        Serial.println("Stable");

        Serial.println("State: DROP");
        state = DROP;
        previousState = STABILISE;
        myLib_.msgAEnvoyer.etape = DROP;
        
      }
      
      break;

    case DROP:
      //Serial.println("State: DROP");
      //position = myLib_.EncodeurOptiPos();
      //Serial.println(position);

      if(!myLib_.avanceDe((1.22), 0.4)){
        myLib_.controlMagnet(LOW);
        myLib_.incrementCmptrEtape();

        Serial.println("State: GOHOME");
        state = GOHOME;
        previousState = DROP;
        myLib_.msgAEnvoyer.etape = GOHOME;
      }
     
      break;

    case GOHOME:
      //Serial.println("State: GOHOME");
      //Serial.println(myLib_.getAngle());
      if(previousState==DROP){
        if(!myLib_.avanceDe(0, 0.7)){
          Serial.println("State: PICK");
          state = PICK;
          previousState = GOHOME;
          myLib_.msgAEnvoyer.etape = PICK;
        }
      }

      else{
        if(!myLib_.avanceDe(0, 0.3)){
          Serial.println("State: PICK");
          state = PICK;
          previousState = GOHOME;
          myLib_.msgAEnvoyer.etape = PICK;
        }
      }
            
      break;

    case STOP:
      //Serial.println("State: OBSTACLE");
      myLib_.AX_.setMotorPWM(0,0);
      state = READY;
      previousState = STOP;
      myLib_.msgAEnvoyer.etape = READY;
    
      break;

    case MESURE:
      // Etat de tests
      Serial.println("State: MESURE");
      //Serial.println(myLib_.getAngle());
      Serial.println(myLib_.IMU_.getGyroscopeY());
      delay(100);
    
      break;

      case SEND:
        // Etat de tests
        //Serial.println("State: SEND");
        if(myLib_.getDemarrage()){
          for(int i=1; i<7; i++){
            myLib_.msgAEnvoyer.temps=i*1000;
            myLib_.msgAEnvoyer.position=0.25*i;
            myLib_.msgAEnvoyer.etape = i;
            myLib_.etatEnergie();
            myLib_.sendMsg();


            delay(1000);
          }
        }
      

    
      break;
  }

}

