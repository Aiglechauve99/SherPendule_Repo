#ifndef NOTRELIBP2_H
#define NOTRELIBP2_H

#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "strucMsg.h"
#include "PID_A.h"
#include "ICM20600.h"

#define MAGPIN 32

class NotreLibP2{

public:
    ArduinoX AX_;
    ICM20600 IMU_;
    VexQuadEncoder vex;
    struct Msg msgAEnvoyer;
    struct Msg msgRecu;
    

    NotreLibP2();
    ~NotreLibP2();

    bool initRobot();
    bool readMsg();
    bool sendMsg();
    bool controlMagnet(bool etat);
    bool etatEnergie();
    bool avanceDe(float distance, float vitesseMax);
    float EncodeurOptiPos();
    bool oscillation();
    bool oscillation2();
    float getAngle();
    bool stabilise(float angle);

private:
    
};

#endif