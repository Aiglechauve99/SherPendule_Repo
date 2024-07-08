#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "PID_A.h"

ArduinoX AX_;
MotorControl MC_;

void setup() {
  Serial.begin(115200);
  AX_.init();
  MC_.init(8,7);
}
double PotentioDeg(){
  double deg = analogRead(A5)/4.6;
  return deg;
}
double EncodeurPos(){
  double pos = AX_.readEncoder(0)*0.0001246;
  return pos;
}
void loop() {
    PID_A pidMoteur(2, 0.1, 0, 0.01); 
    PID_A pidPendule(20, 0, 0, 0.01); 
    float positionRequis = 0, angleRequis = 88;
    double VitesseMoteur = 0; 
    for (int i = 0; i < 1000; ++i) { 
      double correctionMoteur = pidMoteur.calculsPIDmoteur(positionRequis, EncodeurPos());
      double correctionPendule = pidPendule.calculsPIDpendule(angleRequis, PotentioDeg());

      double commandeVitesseMoteur = correctionMoteur - correctionPendule;

      VitesseMoteur = commandeVitesseMoteur*0.4;
      AX_.setMotorPWM(0, VitesseMoteur);
      Serial.println(EncodeurPos());
      }
}

