#include <Arduino.h>
#include <LibS3GRO.h>

ArduinoX AX_;

void setup() {
  AX_.init();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
}