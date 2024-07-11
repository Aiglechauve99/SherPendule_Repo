#include "PID_A.h"

PID_A::PID_A(double kp, double ki, double kd, double temps_echantillonnage) 
{
        this -> kp = kp;
        this -> ki = ki;
        this -> kd = kd;
        this -> temps_echantillonnage = temps_echantillonnage;
        this -> erreur_precedente = 0;
        this -> integrale = 0;
}

double PID_A::calculsPIDmoteur(double valeur_requis, double valeur_actuelle) 
{
        double erreur = valeur_requis - valeur_actuelle;
        integrale += erreur * temps_echantillonnage;
        double derive = (erreur - erreur_precedente) / temps_echantillonnage;
        double correction = kp * erreur + ki * integrale + kd * derive;
        erreur_precedente = erreur;
        if (correction < 0.1 & correction > 0.02)
                correction = 0.15;
        if (correction > -0.1 & correction < -0.02)
                correction = -0.15;
        /*if (correction > 0.4)
                correction = 0.4;
        if (correction < -0.4)
                correction = -0.4;*/
        return correction;
}
double PID_A::calculsPIDpendule(double valeur_requis, double valeur_actuelle) 
{
        double erreur = valeur_requis - valeur_actuelle;
        if (erreur < 5 & erreur > -5)
                erreur = 0;
        integrale += erreur * temps_echantillonnage;
        double derive = (erreur - erreur_precedente) / temps_echantillonnage;
        double correction = kp * erreur + ki * integrale + kd * derive;
        erreur_precedente = erreur;
        /*if (correction < 0.1 & correction > 0.01)
                correction = 0.15;
        if (correction > -0.1 & correction < -0.01)
                correction = -0.15;
        if (correction > 0.3)
                correction = 0.3;
        if (correction < -0.3)
                correction = -0.3;*/
        return correction;
}