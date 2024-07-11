#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "PID_A.h"
#include "ICM20600.h"

#define degToRad 3.1415/360

ArduinoX AX_;
MotorControl MC_;
VexQuadEncoder vex; // Encodeur optique pour roue flottante

ICM20600 IMU_(true);
int X0=0;
int Y0 = 0;
int Z0 = 0;
float vitesse = 0.0;

void setup() {
  Serial.begin(115200);

  vex.init(2,3);
  attachInterrupt(vex.getPinInt(), []{vex.isr();}, FALLING);
  vex.reset();

  AX_.init();
  MC_.init(8,7);
  IMU_.initialize();
  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);

  X0 = IMU_.getAccelerationX();
  Y0 = IMU_.getAccelerationY();
  Z0 = IMU_.getAccelerationZ();
  //pinMode(32, OUTPUT);
  //digitalWrite(32, HIGH);
}

double EncodeurOptiPos(){
  double pos = vex.getCount()/225.441323;
  return pos;
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
        delay(10);
        y = y+1;
        if (y > 198)
          i = HIGH;
    }
  }
  AX_.setMotorPWM(0, 0);
}

void loop() 
{
  PID_A PID_PenduleIMU(0.001, 0, 0, 0);

  double vitesseCible = 0;
  double VitesseMoteur = 0;

  for (int i = 0; i < 1000; ++i) 
  {
    //Serial.println("Vitesse angulaire Y : " + String(IMU_.getGyroscopeY()));
    double correctionPenduleIMU = PID_PenduleIMU.calculsPIDpenduleIMU(vitesseCible, IMU_.getGyroscopeY());

    Serial.println("correction: " + String(correctionPenduleIMU));

    double commandeVitesseMoteur = correctionPenduleIMU;

    VitesseMoteur = commandeVitesseMoteur;
    AX_.setMotorPWM(0, VitesseMoteur);

    delay(2000);

  }
}