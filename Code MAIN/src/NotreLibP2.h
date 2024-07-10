#ifndef NOTRELIBP2
#define NOTRELIBP2
#include <LibS3GRO.h>

#include <ArduinoJson.h>
#include "strucMsg.h"

class NotreLibP2{

public:
    ArduinoX AX_;

    
    StaticJsonDocument<500> sendedDoc;
    struct Msg msgAEnvoyer;
    struct Msg msgRecu;

    

    NotreLibP2();
    ~NotreLibP2();

    bool initRobot();
    bool readMsg();

private:

};

#endif