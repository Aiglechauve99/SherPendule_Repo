#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>

ArduinoX AX_;
IMU9DOF IMU_;

void setup() {
  Serial.begin(115200);
  Serial.println("Début");
  AX_.init();

  Serial.println("IMU init");
  if(!IMU_.isConnected()){
    Serial.println("IMU connecté");
  }
  else{
    Serial.println("IMU non connecté");
  }
  Serial.println(IMU_.getAccelX());
  
}

void loop() {
  Serial.println("X : "+ String((IMU_.getAccelX()))+"\tY : " + String(IMU_.getAccelY())+"\tZ : "+ String(IMU_.getAccelZ()) + "\tTempérature : "+String(IMU_.getTemp()));
  delay(500);
}