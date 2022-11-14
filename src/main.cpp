#include "Capteurs.h"
#include "Mouvements.h"

int etape = -1;
Capteurs capteurs;
Mouvements mouvements;

int pasDemurCpt = 0;
float CheckVirageDroiteTimeout = 0;
float CheckLigneTimeout = 0;
float CheckTapisTimeout = 0;
float CheckJumpTimeout = 0;
float CheckPoutreTimeout = 0;

bool rotateStarted = false;
bool poutreStarted = false;
int dist_minA = 0;
int dist_maxA = 0;

int dist_minR = 0;
int dist_maxR = 0;

void setup() {
  // put your setup code here, to run once:
  BoardInit();
  // capteurs = Capteurs();
  // mouvements = Mouvements(capteurs);
  mouvements.SetCapteurs(&capteurs);
  capteurs.CapteursInit();
}

// void AjustementDebut() {
//   //Tourne de 60 degrés jusqu'à ce que le capteur avant ne capte rien et que le capteur de droite capte un objet proche
//   //La seule position ou c'est possible est si le robot est dans le bon sens
//   while ((capteurs.Get_STATE_IR()[IR_AVANT_BAS] <= CAPTEUR_LOIN) && (capteurs.Get_STATE_IR()[IR_DROITE] > CAPTEUR_LOIN)) {
//     Serial.print("Distance avant: ");
//     Serial.print(capteurs.Get_STATE_IR()[IR_AVANT_BAS]);
//     Serial.print("\n");
//     Serial.print("Distance gauche: ");
//     Serial.print(capteurs.Get_STATE_IR()[IR_DROITE]);
//     Serial.print("\n");
//     mouvements.rotate(60);
//     delay(1000);
//   }

// }



void VirageDroite(){
  float time = millis();
  float *vals;

  if(time >= CheckVirageDroiteTimeout){
    vals = capteurs.Get_STATE_IR();

    if(vals[IR_AVANT_DROITE] <= 10){
      //Pas de mur Redy to turn
      pasDemurCpt++;
      CheckVirageDroiteTimeout = time + 50;
      if(pasDemurCpt >= 5){
        mouvements.TurnRight();
        etape++;
        pasDemurCpt = 0;
      }

    }else{
      //Mur
      CheckVirageDroiteTimeout = time + 50;
    }
  }
}

void rotatestart() {
  if(!rotateStarted){
    float speed = 0.15;
    int couleur = capteurs.Get_Couleur();
    if(couleur == Red)
    {
      dist_minA = 62;
      dist_maxA = 73;

      dist_minR = 64;
      dist_maxR = 74;
    }
    if(couleur == Yellow)
    {
      dist_minA =90;
      dist_maxA= 100;

      dist_minR = 93;
      dist_maxR = 103;
    }
    if(couleur == Green)
    {
      dist_minA = 138;
      dist_maxA = 148;

      dist_minR = 160;
      dist_maxR = 170;
    }
    if(couleur == Blue)
    {
      dist_minA = 400;
      dist_maxA = 600;

      dist_minR = 400;
      dist_maxR = 600;
    }
  
    MOTOR_SetSpeed(DROITE, -speed);
    MOTOR_SetSpeed(GAUCHE, speed);
    rotateStarted = true;
  }else{
     float* vals_IR;
    //definir les ranges
    vals_IR = capteurs.Get_STATE_IR();
    //print the values
    Serial.print(" avant droite: ");
    Serial.print(vals_IR[IR_AVANT_DROITE]);
    Serial.print(" droite: ");
    Serial.print(vals_IR[IR_DROITE]);
    Serial.print("\n");

    //si les le capteur droite en haut et le capteur droit en bas sont dans la range, alors t bien place
    if(dist_minR<=vals_IR[IR_DROITE] && vals_IR[IR_DROITE] <= dist_maxR && dist_minA<=vals_IR[IR_AVANT_DROITE] && vals_IR[IR_AVANT_DROITE] <= dist_maxA)
    {
      MOTOR_SetSpeed(DROITE, 0);
      MOTOR_SetSpeed(GAUCHE, 0);
      Serial.println("on est bien place");
      //sortir
      mouvements.MotorTurn(2, 10, 0.4, 1);
      etape++;
    }
  }

}

void Tapis(){
  float time = millis();
  int couleur;

  if(time >= CheckTapisTimeout){
    couleur = capteurs.Get_Couleur();

    if(couleur == Gray){
      //Pas de mur Redy to turn
      mouvements.MoveTapis();
      etape++;
    }else{
      //Mur
      CheckTapisTimeout = time + 50;
    }
  }
}

void VirageDroite2(){
  float time = millis();
  int couleur;

  if(time >= CheckVirageDroiteTimeout){
    couleur = capteurs.Get_Couleur();

    if(couleur != Gray){
        //Find la couleur a suivre
        mouvements.TurnRight();
        etape++;
    }else{
      //Mur
      CheckVirageDroiteTimeout = time + 50;
    }
  }
}





void SuivreLigne(){
  float time = millis();
  int couleur;

  if(time >= CheckLigneTimeout){
    couleur = capteurs.Get_Couleur();

    if(couleur == White || couleur == Black){
        //Zone ligne
        mouvements.SuivreLigne();
        etape++;
    }else{
      //Mur
      CheckLigneTimeout = time + 50;
    }
  }
}

void Jump(){
  float time = millis();
  int couleur;

  if(time >= CheckJumpTimeout){
    couleur = capteurs.Get_Couleur();

    if(couleur != White && couleur != Black){
        //Zone ligne
        mouvements.Jump();
        etape++;
    }else{
      //Mur
      CheckJumpTimeout = time + 50;
    }
  }
}

void PoutreDeBois(){
  float time = millis();

  if(time >= CheckPoutreTimeout){
      if(!poutreStarted){
        int couleur = capteurs.Get_Couleur();
        if(couleur == Red)
        {
          dist_minA = 62;
          dist_maxA = 73;

          dist_minR = 64;
          dist_maxR = 74;
        }
        if(couleur == Yellow)
        {
          dist_minA =90;
          dist_maxA= 100;

          dist_minR = 93;
          dist_maxR = 103;
        }
        if(couleur == Green)
        {
          dist_minA = 138;
          dist_maxA = 148;

          dist_minR = 160;
          dist_maxR = 170;
        }
        if(couleur == Blue)
        {
          dist_minA = 400;
          dist_maxA = 600;

          dist_minR = 400;
          dist_maxR = 600;
        }
      
        poutreStarted = true;
      }else{
        float* vals_IR;
        //definir les ranges
        vals_IR = capteurs.Get_STATE_IR();
        //print the values
        // Serial.print(" avant droite: ");
        // Serial.print(vals_IR[IR_AVANT_DROITE]);
        // Serial.print(" droite: ");
        // Serial.print(vals_IR[IR_DROITE]);
        // Serial.print("\n");

        //si les le capteur droite en haut et le capteur droit en bas sont dans la range, alors t bien place
        if(dist_minR<=vals_IR[IR_DROITE] && vals_IR[IR_DROITE] <= dist_maxR && dist_minA<=vals_IR[IR_AVANT_DROITE] && vals_IR[IR_AVANT_DROITE] <= dist_maxA)
        {
          Serial.println("poutre trouve");
          //TODO: spécial mode poutre
          mouvements.MotorTurn(2, 10, 0.4, 1);
          etape++;
        }
      }
  }else{
      //Mur
      CheckPoutreTimeout = time + 50;
  }
}



//Trouver couleur start
//Rotate vers la droite tant que les deux capteur de droite ne voient pas le mur (distance pré définie selon la couleur de départ)

void handle_etapes()
{
  switch(etape)
  {
    //case 0 est retrouver sa ligne de depart
    case -1:
      mouvements.SetCouleurToFollow(capteurs.Get_Couleur());
      etape++;
    break;
    case 0:
    //départ
      rotatestart();
      capteurs.ListenBumper();
      // if(capteurs.Get_ready()){
      //   if(capteurs.Get_start()){
      //     rotatestart();
      //   }
      // }

    break;
    case 1:
      //avancer jusqu'a ce qu'on voit le virage et commencer a tourner
      VirageDroite();
    break;

    case 2: 
      //tourner tant que capteur COULEUR ne voit pas tapis et avancer en ligne droite
      Tapis();
    break;

    case 3: //section 2 (tapis)
      //dès qu'on revoit une couleur, commencer a tourner
      VirageDroite2();
    break;

    case 4: //section 3 (deuxieme tournant)
      //tourner tant que capteur IR ne voit pas la barre de bois ultra close ou (pour le vert si il voit du bleu)
      PoutreDeBois();
    break;

    case 5: //section 4 et 5 (deuxieme ligne droite)
      //capteur voit du blanc ou du noir --> suit la ligne
      SuivreLigne();
    break;

    case 6: //section 6, 7 et 8(suiveur de ligne)
      //on voit une couleur --> Jump
      Jump();
    break;

    case 7: //section 9
      //Recommencer le premier virage
      mouvements.MotorTurn(2, 10, 0.4, 1);
      VirageDroite();
    break;
    
    case 8: //section 10 (finir le parcours avec ligne droite)
      //tourner tant que capteur COULEUR ne voit pas tapis et avancer en ligne droite
      Tapis();
    break;
    
    case 9:
       //dès qu'on revoit une couleur, commencer a tourner
      VirageDroite2();
    break;

  }
}

void loop()
{
  capteurs.UpdateCapteurs();
  mouvements.Update_Mouvements();
  
  // handle_etapes();

  mouvements.MotorTurn(2, 10, 0.4, 1);
}
