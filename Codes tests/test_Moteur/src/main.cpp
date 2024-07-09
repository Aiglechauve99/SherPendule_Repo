<<<<<<< HEAD
<<<<<<< Updated upstream
=======
#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
=======
#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "PID_A.h"
>>>>>>> origin/main

ArduinoX AX_;
MotorControl MC_;

void setup() {
  Serial.begin(115200);
  AX_.init();
<<<<<<< HEAD
  //MC_.init(6,31);
  //MC_.setSpeed(-0.5);
  AX_.setMotorPWM(0,0.5);
  
}

void loop() {
  
}
>>>>>>> Stashed changes
=======
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
    PID_A pidMoteur(2, 0.5, 0, 0.01); 
    PID_A pidPendule(0, 0, 0, 0.01); 
    double positionRequis = 1.0, angleRequis = 90;
    double VitesseMoteur = 0; 
    for (int i = 0; i < 1000; ++i) { 
      double correctionMoteur = pidMoteur.calculsPID(positionRequis, EncodeurPos());
      double correctionPendule = pidPendule.calculsPID(angleRequis, PotentioDeg());

      double commandeVitesseMoteur = correctionMoteur + correctionPendule;

      VitesseMoteur = commandeVitesseMoteur;
      AX_.setMotorPWM(0, VitesseMoteur);
      Serial.println(EncodeurPos());
      }
}

>>>>>>> origin/main
