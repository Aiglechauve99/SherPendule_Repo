#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "ICM20600.h"

#define degToRad 3.1415/360

ArduinoX AX_;
ICM20600 IMU_(true);
int X0=0;
int Y0 = 0;
int Z0 = 0;
float vitesse = 0.0;

void setup() {
  Serial.begin(115200);
  Serial.println("Début");
  AX_.init();
  IMU_.initialize();
  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);

  Serial.println("IMU init");
  X0 = IMU_.getAccelerationX();
  Y0 = IMU_.getAccelerationY();
  Z0 = IMU_.getAccelerationZ();
  
}

void loop() {
  Serial.println("----------------------------------------------");
  //Serial.println("Vitesse tangentielle : X : "+ String((IMU_.getAccelerationX()-X0))+"\tY : " + String(IMU_.getAccelerationY()-Y0)+"\tZ : "+ String(IMU_.getAccelerationZ()-Z0) + "\tTempérature : "+String(IMU_.getTemperature()));
  //Serial.println("Vitesse angulaire : X : "+ String(IMU_.getGyroscopeX())+"\tY : " + String(IMU_.getGyroscopeY())+"\tZ : "+ String(IMU_.getGyroscopeZ()) + "\tTempérature : "+String(IMU_.getTemperature()));
  Serial.println("Vitesse angulaire Y : " + String(IMU_.getGyroscopeY()));
  vitesse = IMU_.getGyroscopeY()*degToRad*0.35;
  Serial.println("Vitesse tangeantielle Y : " + String(vitesse));

  if (vitesse >= 0.5 || vitesse <= -0.5){
    Serial.println("Largage");
    digitalWrite(32, LOW);
  }
  delay(500);
}