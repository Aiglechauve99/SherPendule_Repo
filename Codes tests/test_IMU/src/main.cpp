#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>

ArduinoX AX_;
IMU9DOF IMU_;

void setup() {
  Serial.begin(115200);
  Serial.println("Début");
  AX_.init();
  IMU_.init();


  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // Ça fait flasher la led sur le Arduino
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}