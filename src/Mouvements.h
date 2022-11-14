#ifndef MOUVEMENTS_H_
#define MOUVEMENTS_H_

#include <Arduino.h>
#include <LibRobus.h>
#include "Capteurs.h"

#define GAUCHE 0
#define DROITE 1

class Mouvements {

    public:
    //Return the moving state of the robot
    bool IsMoving();

    void SetMoving(bool state);

    Capteurs* GetCapteurs();

    void SetCapteurs(Capteurs* capteurs);

    int GetCouleurToFollow();

    void SetCouleurToFollow(int couleur);

    /// @brief Permet de faire tourner le roue x nombre de fois
    /// @param mode 2 = les deux roues, 1 = roue droite, 0 = roue gauche, 3 = les 2 roues differentes vitesse
    /// @param turnNb Nombre de tours
    /// @param speed Vitesse de déplacement
    /// @param rapport Rapport de vitesse entre les roues
    void MotorTurn(int mode, float turnNb, float speed, float rapport);

    /// @brief Permet de déplacer le robot sur une distance à une certaine vitesse
    /// @param distanceInPulse Nombre de pulse a parcourir 
    /// @param speed la vitesse de déplacement
    void move(float distanceInPulse, float speed);

    /// @brief Permet de se déplacer à l'aide d'une courbe d'accélération
    ///        selon une distance en CM
    /// @param distanceInCM distance en CM
    void progressiveMove(float distanceInCM);

    /// @brief Permet au robot de tourner sur lui même
    /// @param degree Angle de rotation
    void rotate(float degree);

    /// @brief  Tourne une roue a la fois
    /// @param angleInDegrees Angle de rotation
    /// @param speed Vitesse de rotation
    void turn (float angleInDegrees, float speed);

    /// @brief Permet de reculer N'EST PAS ASYNCHRONE
    /// @param distanceInCM distance en CM
    /// @param speed Vitesse du déplacement
    void MoveBackwards(float distanceInCM, float speed);

    /// @brief Change le mode de déplacement à 3
    void TurnRight();

    /// @brief Change le mode de déplacement à 4
    void MoveTapis();

    /// @brief Update le mouvement du robot de manière asynchrone (A call dans loop)
    void Update_Mouvements();

    void SuivreLigne();

    void Jump();
    
    private:
    Capteurs capteurs;
    int CouleurToFollow;

    //Calcule le PID selon la couleur
    float ColorPID();

    //Calcule le PID selon la ligne
    float LignePID();

    //Calcule le mouvement pour le Jump
    float JumpPID();

    /// @brief Remet par defaut les variables de mouvement
    void ResetMoveVariables();


    //Suiveur de ligne
    int etat_suiveur[3];
    int retour_ligne;
};


#endif /* MOUVEMENTS_H_ */