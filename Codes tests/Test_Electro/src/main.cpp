#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>

ArduinoX AX_;
uint8_t MAGPIN = 32;


void setup() {
  Serial.begin(115200);
  AX_.init();
  
  pinMode(MAGPIN, OUTPUT);
 
}

void loop() {
  // Ça fait flasher la led sur le Arduino

  digitalWrite(MAGPIN,HIGH);
  
  Serial.println("electro on high");
  delay(5000);
  digitalWrite(MAGPIN,LOW);
  
  Serial.println("electro on low");
  delay(5000);
}