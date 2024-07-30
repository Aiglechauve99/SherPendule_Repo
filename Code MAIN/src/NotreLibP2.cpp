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
    IMU_.initialize();
    
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

    if(parse_msg.as<int>() == 1){
        Demarrage = true;
    }
    else{
        Demarrage = false;
    }
    //msgRecu.etape = parse_msg.as<int>();
    return 0;
}

bool NotreLibP2::sendMsg(){

    StaticJsonDocument<500> sendedDoc;
    // Elements du message
    if(1){
        sendedDoc["temps"] = msgAEnvoyer.temps;
        sendedDoc["etape"] = msgAEnvoyer.etape;
        sendedDoc["erreur"] = msgAEnvoyer.erreur;
        sendedDoc["anglePendule"] = msgAEnvoyer.anglePendule;
        sendedDoc["distanceParcourue"] = msgAEnvoyer.distanceParcourue;
        sendedDoc["position"] = msgAEnvoyer.position;
        sendedDoc["voltageBatterie"] = msgAEnvoyer.voltageBatterie;
        sendedDoc["courantBatterie"] = msgAEnvoyer.courantBatterie;
        sendedDoc["compteurEtape"] = msgAEnvoyer.compteur;
        // Serialisation
        serializeJson(sendedDoc, Serial);
        Serial.println("");
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

bool NotreLibP2::getDataPourMessage(){
    msgAEnvoyer.temps = millis();
    msgAEnvoyer.erreur = erreur;
    msgAEnvoyer.position = EncodeurOptiPos();
    msgAEnvoyer.anglePendule = getAngle();
    msgAEnvoyer.distanceParcourue = distanceParcourue;
    msgAEnvoyer.position = EncodeurOptiPos();
    msgAEnvoyer.voltageBatterie=AX_.getVoltage();
    msgAEnvoyer.courantBatterie=AX_.getCurrent();
    msgAEnvoyer.compteur = compteurEtape;
}

void NotreLibP2::setErreur(){
    erreur = true; 
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

bool NotreLibP2::avanceDe(float positionRequis, float vitesseMax){
    PID_A pidMoteur(1.8, 0.05, 0, 0.01);
    float correctionMoteur = 0;
    unsigned long tempsAvant = millis();
    bool goTo = true;
    float distance = 0;
    float distanceAvant = 0;

    int compteur = 0;

    while(goTo){
        if(millis()-tempsAvant >= 100){
            tempsAvant = millis();
            
            distance = EncodeurOptiPos();
            Serial.println(distance);

            correctionMoteur = pidMoteur.calculsPIDmoteur(positionRequis, distance, vitesseMax);
            AX_.setMotorPWM(0, correctionMoteur);

            compteur++;
            if(correctionMoteur<0.02 && correctionMoteur>-0.02){
                goTo = false;
                Serial.println("Fin PID moteur");
            }
            Serial.println("Compteur PID : "+String(compteur));
            if(compteur >= 25 && distance <=0.05){
                goTo = false;
                vex.reset();
                AX_.setMotorPWM(0,0);
                Serial.println("-------------------------->Reset distance");
            }
        }

        
    }

    //distanceParcourue+=EncodeurOptiPos();
    return 0;
}

bool NotreLibP2::oscillation(){
    unsigned long tempsAvant = 0;
    float vitesse = 0.35;
    int periode = 120;
    float deg = 0;
    float vitesseIMU = 0;
    bool enAvant = true;
    bool oscillation = true;
    const int bon = 40;

    do{
        deg = getAngle();
        vitesseIMU = IMU_.getGyroscopeY();
        if(deg < 30 && vitesseIMU < 0){
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
    delay(300);

    for(int i =100; i>0; i--){
        AX_.setMotorPWM(0, i*0.01);
        delay(10);
    }

    return 0;
}

bool NotreLibP2::oscillation2(){
    return avanceDe(1.22, 1);
}

float NotreLibP2::getAngle(){
    return analogRead(A5)*0.3203-63.95;
}

bool NotreLibP2::stabilise(float angle){
    PID_A PID_PenduleIMU(0.005, 0, 0.0003, 0.01);
    PID_A PID_PendulePotentio(0.016, 0, 0.003, 0.01);

    double correctionPendule = 0;
    double correctionPenduleIMU = 0;
    unsigned long tempsAvant = millis();
    bool goTo = true;
    float angleMesurer = 0;
    float vitesseCible = 0;
    float commandeMoteur = 0;
    float vitesseIMU = 0;
    //int compteurReussite = 0;

    while(goTo){
        if(millis()-tempsAvant >= 10){
            tempsAvant = millis();
            
            angleMesurer = getAngle();
            vitesseIMU = IMU_.getGyroscopeY();
            Serial.println(angleMesurer);

            correctionPendule = PID_PendulePotentio.calculsPIDpendule(angle, angleMesurer);
            correctionPenduleIMU = PID_PenduleIMU.calculsPIDpenduleIMU(vitesseCible, IMU_.getGyroscopeY());
            
            commandeMoteur = correctionPendule + correctionPenduleIMU;
            //commandeMoteur = correctionPendule;

            /*if(angleMesurer>160 or angleMesurer < 20){
                commandeMoteur = 0;
            }*/

            //Serial.println("Pendule : "+String(correctionPendule)+" IMU : " + String(correctionPenduleIMU,5));
            AX_.setMotorPWM(0, commandeMoteur);

           if((correctionPendule<0.03 && correctionPendule>-0.03) && (vitesseIMU > -3 && vitesseIMU < 3)){
                goTo = false;
                Serial.println("Fin PID");
            }
        }
    }
    return 0;
}

bool NotreLibP2::getDemarrage(){
    return Demarrage;
}

void NotreLibP2::calculDistance(){
    distanceParcourue += abs((EncodeurOptiPos()-anciennePosition));
    anciennePosition = EncodeurOptiPos();
}

void NotreLibP2::incrementCmptrEtape(){
    compteurEtape++;
}



