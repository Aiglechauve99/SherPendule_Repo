#ifndef PID_A_H
#define PID_A_H

class PID_A {
private:
    double kp; // proportionnel
    double ki; // intégral
    double kd; // dérivé
    double erreur_precedente;
    double integrale; // somme des erreurs
    double temps_echantillonnage;

public:
    PID_A(double kp, double ki, double kd, double temps_echantillonnage);
    double calculsPIDpendule(double valeur_requis, double valeur_actuelle);
    double calculsPIDmoteur(double valeur_requis, double valeur_actuelle);
    double calculsPIDpenduleIMU(double valeur_requis, double valeur_actuelle);
};
#endif