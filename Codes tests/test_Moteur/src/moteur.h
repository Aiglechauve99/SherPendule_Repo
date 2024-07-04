#include <Arduino.h>
#include <LibS3GRO.h>
#include <ArduinoJson.h>
#include "PID.h"

using namespace std;

#ifndef MOTEUR_H
#define MOTEUR_H

class moteur
{
    public:
        moteur();
        ~moteur();
        void move(int dist, int speed, double accelFactor);
        void accel(double accelFactor);


};

#endif