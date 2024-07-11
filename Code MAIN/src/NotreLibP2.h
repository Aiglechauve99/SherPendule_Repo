#ifndef NOTRELIBP2_H
#define NOTRELIBP2_H

#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "strucMsg.h"

class NotreLibP2{

public:
    ArduinoX AX_;
    struct Msg msgAEnvoyer;
    struct Msg msgRecu;
    

    NotreLibP2();
    ~NotreLibP2();

    bool initRobot();
    bool readMsg();
    bool sendMsg();

private:
    
};

#endif