#ifndef PID_H
#define PID_H

class PID {
private:
    double kp; // proportionnel
    double ki; // intégral
    double kd; // dérivé
    double erreur_precedente;
    double integrale; // somme des erreurs
    double temps_echantillonnage;

public:
    PID(double kp, double ki, double kd, double temps_echantillonnage);
    double calculsPID(double valeur_requis, double valeur_actuelle);
};
#endif
