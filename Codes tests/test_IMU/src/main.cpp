#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "ICM20600.h"

ArduinoX AX_;
ICM20600 IMU_(true);

void setup() {
  Serial.begin(115200);
  Serial.println("Début");
  AX_.init();

  Serial.println("IMU init");
  
}

void loop() {
  Serial.println("----------------------------------------------");
  Serial.println("Vitesse tangentielle : X : "+ String((IMU_.getAccelerationX()))+"\tY : " + String(IMU_.getAccelerationY())+"\tZ : "+ String(IMU_.getAccelerationZ()) + "\tTempérature : "+String(IMU_.getTemperature()));
  Serial.println("Vitesse angulaire : X : "+ String((IMU_.getAccelerationX()))+"\tY : " + String(IMU_.getAccelerationY())+"\tZ : "+ String(IMU_.getAccelerationZ()) + "\tTempérature : "+String(IMU_.getTemperature()));
  delay(100);
}