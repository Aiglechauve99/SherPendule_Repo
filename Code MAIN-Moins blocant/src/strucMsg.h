#ifndef STRUCMSG_H
#define STRUCTMSG_H

struct Msg{
    unsigned long temps;
    int etape;
    int erreur;
    float anglePendule;
    float distanceParcourue;
    float position;
    float voltageBatterie;
    float courantBatterie;
    int compteur;
};

#endif