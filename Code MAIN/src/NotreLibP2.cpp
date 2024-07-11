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
    vex.init(2,3);
    attachInterrupt(vex.getPinInt(), isrWrapper, FALLING);
    vex.reset();

    
    return 0;
}

void isrWrapper(){
    vex.isr();
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
    return pos*-1;
}

bool NotreLibP2::avanceDe(float positionRequis){
    PID_A pidMoteur(1.8, 0.05, 0, 0.01);
    float VitesseMoteur = 0;
    float correctionMoteur = 0;
    unsigned long tempsAvant = millis();
    float commandeVitesseMoteur = 0;
    bool goTo = true;
    float distance = 0;

    while(goTo){
        if(millis()-tempsAvant >= 100){
            tempsAvant = millis();
            
            distance = EncodeurOptiPos();
            Serial.println(distance);

            correctionMoteur = pidMoteur.calculsPIDmoteur(positionRequis, distance);
            AX_.setMotorPWM(0, correctionMoteur);
            if(correctionMoteur<0.001 && correctionMoteur>-0.001){
                goTo = false;
                Serial.println("Fin PID");
            }
        }
    }
}

