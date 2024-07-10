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

bool readMsg(){
    StaticJsonDocument<500> receivedDoc;
    JsonVariant parse_msg;

    DeserializationError error = deserializeJson(receivedDoc, Serial);
    
    parse_msg = receivedDoc["temps"];
    if(!parse_msg.isNull()){
        msgRecu.temps = 
    }
}

