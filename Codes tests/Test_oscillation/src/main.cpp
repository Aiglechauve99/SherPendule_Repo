#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
#define bon 40

ArduinoX AX_;
long int tempsAvant = 0;
float vitesse = 0.35;
int periode = 120;
float deg = 0;
bool enAvant = true;
bool oscillation = true;

void setup() {
  Serial.begin(115200);
  AX_.init();
  pinMode(32, OUTPUT);
  digitalWrite(32, HIGH);
  delay(3000);
}

void loop() {
  do{

    deg = analogRead(A5)*0.3203-63.95;
    if(deg>150){
      oscillation = false;
      tempsAvant = millis();
      break;
    }

    if (millis()-tempsAvant >= periode){
    tempsAvant = millis();

    if(periode < 1000){
      periode += bon;
    }
    
    vitesse *= -1;
    AX_.setMotorPWM(0, vitesse);
    Serial.println("Periode : "+ String(periode)+" Angle : " + String(deg));
    
    }
  }while(oscillation);

  if(enAvant){
    AX_.setMotorPWM(0, -1.0);
    Serial.println("Pied au plancher");
    delay(650);
    enAvant = false;
  }
  else{
    AX_.setMotorPWM(0, 0);
    enAvant = false;
    Serial.println("Stop");
  }
  
}