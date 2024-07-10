struct Msg{
    unsigned long temps;
    int etape;
    float consigne;
    int etatEtape; // En cours 0, Fini = 1, Erreur = -1
    float anglePendule;
    float vitesseMoteur;
    float distanceParcourue;
    float distance;
    float voltageBatterie;
    float courantBatterie;
};