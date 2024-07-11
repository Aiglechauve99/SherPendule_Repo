#ifndef NOTRELIBP2_H
#define NOTRELIBP2_H

#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "strucMsg.h"
#include "PID_A.h"

#define MAGPIN 32

class NotreLibP2{

public:
    ArduinoX AX_;
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
    bool avanceDe(float distance);
    float EncodeurOptiPos();

private:
    
};

#endif