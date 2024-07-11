#include "NotreLibP2.h"

NotreLibP2::NotreLibP2(){
}

NotreLibP2::~NotreLibP2()
{
}

bool NotreLibP2::initRobot(){
    Serial.begin(115200);
    Serial.println("Initialisation");
    AX_.init();
    
    return 0;
}

bool NotreLibP2::readMsg(){
    StaticJsonDocument<500> receivedDoc;
    JsonVariant parse_msg;

    DeserializationError error = deserializeJson(receivedDoc, Serial);

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.c_str());
        return 1;
    }
    
    parse_msg = receivedDoc["etape"];

    if(parse_msg.isNull()){
        return 1;
    }
    msgRecu.etape = parse_msg.as<int>();
    return 0;
}

bool NotreLibP2::sendMsg(){

    StaticJsonDocument<500> sendedDoc;
    // Elements du message
    if(0){
        sendedDoc["temps"] = millis();
        sendedDoc["etape"] = msgAEnvoyer.etape;
        sendedDoc["erreur"] = msgAEnvoyer.erreur;
        sendedDoc["anglePendule"] = msgAEnvoyer.anglePendule;
        sendedDoc["distanceParcourue"] = msgAEnvoyer.distanceParcourue;
        sendedDoc["position"] = msgAEnvoyer.position;
        sendedDoc["voltageBatterie"] = msgAEnvoyer.voltageBatterie;
        sendedDoc["courantBatterie"] = msgAEnvoyer.courantBatterie;
        // Serialisation
        serializeJson(sendedDoc, Serial);
    }
    else{
        Serial.println("temps : "+ String(millis()));
        Serial.println("etape : "+ String(msgAEnvoyer.etape));
        Serial.println("erreur : "+ String(msgAEnvoyer.erreur));
        Serial.println("anglePendule : "+ String(msgAEnvoyer.anglePendule));
        Serial.println("distanceParcourue : "+ String(msgAEnvoyer.distanceParcourue)); 
        Serial.println("position : "+ String(msgAEnvoyer.position)); 
        Serial.println("voltageBatterie : "+ String(msgAEnvoyer.voltageBatterie)); 
        Serial.println("courantBatterie : "+ String(msgAEnvoyer.courantBatterie));
    }
  
}

bool NotreLibP2::controlMagnet(bool etat){
    digitalWrite(MAGPIN,etat);
    return 0;
}

bool NotreLibP2::etatEnergie(){
    msgAEnvoyer.voltageBatterie=AX_.getVoltage();
    msgAEnvoyer.courantBatterie=AX_.getCurrent();

    return AX_.isLowBat();
}

float NotreLibP2::EncodeurOptiPos(){
    float pos = vex.getCount()/225.441323;
    return pos;
}

bool NotreLibP2::avanceDe(float positionRequis){
    PID_A pidMoteur(1.8, 0.05, 0, 0.01);
    float correctionMoteur = 0;
    unsigned long tempsAvant = millis();
    bool goTo = true;
    float distance = 0;

    while(goTo){
        if(millis()-tempsAvant >= 100){
            tempsAvant = millis();
            
            distance = EncodeurOptiPos();
            Serial.println(distance);

            correctionMoteur = pidMoteur.calculsPIDmoteur(positionRequis, distance);
            AX_.setMotorPWM(0, correctionMoteur);
            if(correctionMoteur<0.02 && correctionMoteur>-0.02){
                goTo = false;
                Serial.println("Fin PID");
            }
        }
    }
    return 0;
}

bool NotreLibP2::oscillation(){
    unsigned long tempsAvant = 0;
    float vitesse = 0.35;
    int periode = 120;
    float deg = 0;
    bool enAvant = true;
    bool oscillation = true;
    const int bon = 40;

    do{
        deg = getAngle();
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

    AX_.setMotorPWM(0, 1.0);
    Serial.println("Pied au plancher");
    delay(500);

    AX_.setMotorPWM(0, 0);
    Serial.println("Stop");

    return 0;
}

float NotreLibP2::getAngle(){
    return analogRead(A5)*0.3203-63.95;
}

bool NotreLibP2::stabilise(float angle){
    PID_A pidPendule(0.016, 0.001, 0.1, 0.01);
    float correctionPendule = 0;
    unsigned long tempsAvant = millis();
    bool goTo = true;
    float angleMesurer = 0;

    while(goTo){
        if(millis()-tempsAvant >= 10){
            tempsAvant = millis();
            
            angleMesurer = getAngle();
            Serial.println(angleMesurer);

            correctionPendule = pidPendule.calculsPIDpendule(angle, angleMesurer);
            AX_.setMotorPWM(0, correctionPendule);

            if(correctionPendule<5 && correctionPendule>-5){
                goTo = false;
                Serial.println("Fin PID");
            }
        }
    }
    return 0;
}
