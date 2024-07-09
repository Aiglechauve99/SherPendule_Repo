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
  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);
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
    //PID_A pidMoteur(1.8, 0.05, 0, 0.01); 
    PID_A pidPendule(20, 0, 0, 0.01); 
    float positionRequis = 0, angleRequis = 45;
    double VitesseMoteur = 0; 
    for (int i = 0; i < 1000; ++i) { 
      //double correctionMoteur = pidMoteur.calculsPIDmoteur(positionRequis, EncodeurPos());
      double correctionPendule = pidPendule.calculsPIDpendule(angleRequis, PotentioDeg());

      double commandeVitesseMoteur = correctionPendule;

      VitesseMoteur = commandeVitesseMoteur;
      AX_.setMotorPWM(0, VitesseMoteur);
      Serial.println(correctionPendule);
      }
}

