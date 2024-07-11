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

