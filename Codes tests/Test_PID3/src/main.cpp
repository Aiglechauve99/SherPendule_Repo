#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "PID_A.h"

ArduinoX AX_;
MotorControl MC_;
VexQuadEncoder vex; // Encodeur optique pour roue flottante

void setup() {
  Serial.begin(115200);

  vex.init(2,3);
  attachInterrupt(vex.getPinInt(), []{vex.isr();}, FALLING);
  vex.reset();

  AX_.init();
  MC_.init(8,7);
}

double EncodeurOptiPos(){
  double pos = vex.getCount()/225.441323;
  return pos*-1;
}

double PotentioDeg(){
  double deg = analogRead(A5)*0.3203-63.95;
  return deg;
}

double Vmoteur(){
  double deltadeg = 90 - PotentioDeg();
  double correction = deltadeg*0.012;
  return correction;
}

void Stabiliser(){
  bool i = LOW;
  int y = 0;
  int x = 0;
  while (i != HIGH){
    AX_.setMotorPWM(0, Vmoteur());
    while ((PotentioDeg()>85) & (PotentioDeg()<95) & (y<100)){
        delay(5);
        y = y+1;
        if (y > 198)
          i = HIGH;
    }
  }
  AX_.setMotorPWM(0, 0);
}

void PIDMoteur2(double pos){
  PID_A pidMoteur(1.8, 0.05, 0, 0.01); 
   // PID_A pidPendule(0.016, 0.001, 0.1, 0.01);
    
    float positionRequis = pos;
    //float angleRequis = 90;
    double VitesseMoteur = 0; 
    double correctionMoteur;
      for (int i = 0; i < 1000; ++i) 
      { 
          correctionMoteur = pidMoteur.calculsPIDmoteur(positionRequis, EncodeurOptiPos());
          double commandeVitesseMoteur = correctionMoteur;
          VitesseMoteur = commandeVitesseMoteur*-1;
          AX_.setMotorPWM(0, VitesseMoteur);
      }
  }

void PIDMoteur(double pos){
  for(int i = 0; i < 5; ++i) {
      PIDMoteur2(pos);
    }
}

void PIDPendule2(int angle){
  PID_A pidPendule(0.016, 0.001, 0.1, 0.01);
    
    float angleRequis = angle;
    double VitesseMoteur = 0; 
    for (int i = 0; i < 1000; ++i) { 
        double correctionPendule = pidPendule.calculsPIDpendule(angleRequis, PotentioDeg());
        double commandeVitesseMoteur = correctionPendule;
        VitesseMoteur = commandeVitesseMoteur;
        AX_.setMotorPWM(0, VitesseMoteur);
    }
}
void PIDPendule(int angle){
  for(int i = 0; i < 10; ++i) {
      PIDPendule2(angle);
    }
}
void loop() {
    PIDPendule(90);
    delay(10000);
}

