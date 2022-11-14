#include "Mouvements.h"

//----- Variables globales -----

//K pour PID couleur
const float K_Couleur = .0003;

// 1 tour de roue = 3094 Pulse
const float TOUR_PAR_PIEDS = 1.27000;

const float L=0.6167979; //en pieds   +0.005
const float RAPPORT_R = 0.843042729;
const float DIST_R = 3.80839895 * 1.5708 * TOUR_PAR_PIEDS; //longueur arc roue gauche (rayon * angle en rad) en turnnb

const float RAPPORT_Y = 0.785373831;
const float DIST_Y = 2.80839895 * 1.5708 * TOUR_PAR_PIEDS; //longueur arc roue gauche

const float RAPPORT_G = 0.663925979;
const float DIST_G = 1.80839895 * 1.5708 * TOUR_PAR_PIEDS; 


//Constantes Robot B
const float LEFT_MOTOR_FULL_TURN = 3075; //in pulses
const float RIGHT_MOTOR_FULL_TURN = 3075; //in pulses
const float TOUR_PAR_CM = (1 / 24); //t/cm
const float PULSE_PAR_CM = 128.125; //p/cm

//90deg = 1.25 tour
// const float TOUR_PAR_DEG = (1 / 72); //t/deg
// const float PULSE_PAR_DEG = 43.0083333333333; //p/deg

//90deg = 1.265 tour
const float TOUR_PAR_DEG_LEFT = (1.f/71.1462451f); //t/deg
const float PULSE_PAR_DEG_LEFT = 43.5236111f; //p/deg

//90deg = 1.255 tour
const float TOUR_PAR_DEG_RIGHT = (1.f/71.7131474f); //t/deg
// const float PULSE_PAR_DEG_RIGHT = 43.1998889f; //p/deg
const float PULSE_PAR_DEG_RIGHT = 44.0009889f; //p/deg


float vitesseMoteur0=0;   //vitesse du moteur gauche (0)
float vitesseMoteur1=0;   //vitesse du moteur droit (1)

const int moteurDroit = 0;
const int moteurGauche = 1;
const int deuxMoteurs = 2;

float TimeoutAsservissement = 0;

//variables déplacement
bool Moving = false;
float TurnNb = 0;
float Speed = 0;
float Rapport = 1;
float X = 0;
float Y = 0;
float StartX = 0;
float StartY = 0;
float K = 0.003;
float Speed_d = 0;

//99 = idle
//0 = roue gauche
//1 = roue droite
//2 = les deux roues
//3 = les deux roues differentes vitesse
int Mode = 99;

int Mouvements::GetCouleurToFollow() {
    return CouleurToFollow;
}

void Mouvements::SetCouleurToFollow(int couleur) {
    CouleurToFollow = couleur;
    capteurs.SetCouleurToFollow(couleur);
}

Capteurs* Mouvements::GetCapteurs() {
    return &capteurs;
}

void Mouvements::SetCapteurs(Capteurs* capteurs) {
    this->capteurs = *capteurs;
}

void Mouvements::MoveTapis(){
    Moving = false;
    Mode = 4;
    MotorTurn(4, 10, 0.4, 1);
}

void Mouvements::SetMoving(bool state) {
    Moving = state;
}

bool Mouvements::IsMoving() {
    return Moving;
}

void Mouvements::SuivreLigne(){
  //Setup suiveur de ligne
  Mode = 5;
  MotorTurn(Mode, 10, 0.4, 1);
}

void Mouvements::ResetMoveVariables(){
    Serial.println("ResetMoveVariables");
    Moving = false;
    TurnNb = 0;
    Speed = 0;
    Rapport = 1;
    X = 0;
    Y = 0;
    StartX = 0;
    StartY = 0;
    K = 0.0003;
    Mode = 99;
    Speed_d = 0;
}

//----- Méthodes ------
void Mouvements::MotorTurn(int mode, float turnNb, float speed, float rapport = 1){

  if(!Moving){
    Serial.println("MotorTurn");
    ResetMoveVariables();  
    //set global variables
    Mode = mode;
    TurnNb = turnNb;
    Speed = speed;
    Rapport = rapport;
    Speed_d = speed * rapport;

    switch (mode){
      case 6:
        //start jump
        StartX = ENCODER_Read(0);
        StartY = ENCODER_Read(1);

        MOTOR_SetSpeed(1, Speed);
        MOTOR_SetSpeed(0, Speed);
        Moving = true;
      break;
      case 5 :
      {
        // suiveur ligne
        StartX = ENCODER_Read(0);
        StartY = ENCODER_Read(1);

        MOTOR_SetSpeed(1, Speed);
        MOTOR_SetSpeed(0, Speed);
        Moving = true;
        break;

      }
      case 4 :
      {
        // tapis
        K = 0.006;
        StartX = ENCODER_Read(0);
        StartY = ENCODER_Read(1);

        MOTOR_SetSpeed(1, Speed);
        MOTOR_SetSpeed(0, Speed);
        Moving = true;
        break;

      }
      case 3 :
      {
        //tourner en avancant (premier tournant)
        //Left and Right Motor

        //depend de la distance entre les roues
        StartX = ENCODER_Read(0);
        StartY = ENCODER_Read(1);
        //droit va 1.15 fois plus vite que droit pour tourner(environ)
        MOTOR_SetSpeed(0, Speed);
        MOTOR_SetSpeed(1, Speed_d);
        Moving = true;
        break;
      }

      
      case 2 :
      {
        //Left and Right Motor
        StartX = ENCODER_Read(0);
        StartY = ENCODER_Read(1);

        MOTOR_SetSpeed(1, Speed);
        MOTOR_SetSpeed(0, Speed);
        Moving = true;
        Serial.println("movement started...");
        break;
      }

      case 1 :
      {
        //Right Motor
        Y = 0;
        StartY = ENCODER_Read(1);
        MOTOR_SetSpeed(1, speed);
        Moving = true;
        
        break;
      }

      case 0 :
      {
        //Left Motor
        X = 0;
        StartX = ENCODER_Read(0);
        MOTOR_SetSpeed(0, speed);
        Moving = true;
        break;
      }
    }
  }
}

void Mouvements::Update_Mouvements(){
  if(Moving){
    float time = millis();
    if(time >= TimeoutAsservissement){
      TimeoutAsservissement = time + 100;
      Serial.println("Update_Mouvements");
      switch (Mode){
        case 6:
        //jump
         //Bouger en suivant une ligne
            X = ENCODER_Read(0);
            Y = ENCODER_Read(1);
            Serial.println("Updated");
            // Serial.print(x);
            // Serial.print(" ");
            // Serial.println(y);

            if(Y != X){
              Serial.println("Asservissement");
              //diff
              float diff = Y - X;
              Serial.print("diff : ");
              Serial.println(diff);

              //Asservissement
              Serial.println("Asservissement");
              //diff

              if(diff > 0){
              //si +grand que 0, x est pas assez grand
                float modif = diff * K;
                MOTOR_SetSpeed(0, Speed + modif);
                MOTOR_SetSpeed(1, Speed);

              }
              else if (diff < 0){
              //si - que 0, y est pas assez grand
                float modif = -diff * K;
                MOTOR_SetSpeed(1, Speed + modif);
                MOTOR_SetSpeed(0, Speed);

              }
              else{
                MOTOR_SetSpeed(0, Speed);
                MOTOR_SetSpeed(1, Speed);

              }
            }

            if(X >= LEFT_MOTOR_FULL_TURN * TurnNb + StartX){
              MOTOR_SetSpeed(0, 0);
            }
            if(Y >= RIGHT_MOTOR_FULL_TURN * TurnNb + StartY){
              MOTOR_SetSpeed(1, 0);
            }
            if(X >= LEFT_MOTOR_FULL_TURN * TurnNb + StartX && Y >= RIGHT_MOTOR_FULL_TURN * TurnNb + StartY){
              // Serial.println("End");
              MOTOR_SetSpeed(0, 0);
              MOTOR_SetSpeed(1, 0);
              Moving = false;
            }
        break;
        case 5:
            //Bouger en suivant une ligne
            Serial.println("Updated");
            // Serial.print(x);
            // Serial.print(" ");
            // Serial.println(y);

            //Asservissement
              Serial.println("Asservissement");
              //diff
              float diff = LignePID();

              if(diff > 0){
              //si +grand que 0, x est pas assez grand
                float modif = diff * K;
                MOTOR_SetSpeed(0, Speed + modif);
                MOTOR_SetSpeed(1, Speed);

              }
              else if (diff < 0){
              //si - que 0, y est pas assez grand
                float modif = -diff * K;
                MOTOR_SetSpeed(1, Speed + modif);
                MOTOR_SetSpeed(0, Speed);

              }
              else{
                MOTOR_SetSpeed(0, Speed);
                MOTOR_SetSpeed(1, Speed);

              }
        break;
        case 4 :
    
        // tapis
        X = ENCODER_Read(0);
        Y = ENCODER_Read(1);
        Serial.println("Updated");
        // Serial.print(x);
        // Serial.print(" ");
        // Serial.println(y);

        //Asservissement
        if(Y != X){
          //diff

          float diff = Y - X;
        
          if(diff > 0){
          //si +grand que 0, x est pas assez grand
            float modif = diff * K;
            MOTOR_SetSpeed(0, Speed + modif);
            MOTOR_SetSpeed(1, Speed);

        
          }
          else if (diff < 0){
          //si - que 0, y est pas assez grand
            float modif = -diff * K;
            MOTOR_SetSpeed(1, Speed + modif);
            MOTOR_SetSpeed(0, Speed);

          }
          else{
            MOTOR_SetSpeed(0, Speed);
            MOTOR_SetSpeed(1, Speed);

          }
        }
        Serial.print("end update");
        break;
          
        case 3:
        //Virage a droite
          Y = ENCODER_Read(0);
          X = ENCODER_Read(1);
          // Serial.print(x);
          // Serial.print(" ");
          // Serial.println(y);

          //Asservissement
          //si encodeur droit est different de 0.83 x encodeur gauche 
          if(Y*Rapport != X){
            //au lieu de augmenter droit seul, on augmente droit de moitie de la difference 
            //et on diminue gauche de moitie de difference == plus smooth 
            float diff = Y*Rapport - X;
            diff += ColorPID();
            
            if(diff > 0){
            //si +grand que 0, x est pas assez grand
              float modif = diff * K;
              MOTOR_SetSpeed(1, Speed_d + (1/2)*modif);
              MOTOR_SetSpeed(0, Speed-(1.2)*modif);
            }
            else if (diff < 0){
            //si - que 0, y est pas assez grand
              float modif = diff * K;
              MOTOR_SetSpeed(1, Speed_d - (1/2)*modif);
              MOTOR_SetSpeed(0, Speed+(1/2)*modif);
            }
            else{
              MOTOR_SetSpeed(0, Speed);
              MOTOR_SetSpeed(1, Speed_d);
            }
          }
          break;

          case 2:
            //Bouger en suivant une couleur
            X = ENCODER_Read(0);
            Y = ENCODER_Read(1);
            Serial.println("Updated");
            // Serial.print(x);
            // Serial.print(" ");
            // Serial.println(y);

            //Asservissement
            if(Y != X){
              Serial.println("Asservissement");
              //diff
              float diff = Y - X;
              Serial.print("diff : ");
              Serial.println(diff);

              diff += ColorPID();

              if(diff > 0){
              //si +grand que 0, x est pas assez grand
                float modif = diff * K;
                MOTOR_SetSpeed(0, Speed + modif);
                MOTOR_SetSpeed(1, Speed);

              }
              else if (diff < 0){
              //si - que 0, y est pas assez grand
                float modif = -diff * K;
                MOTOR_SetSpeed(1, Speed + modif);
                MOTOR_SetSpeed(0, Speed);

              }
              else{
                MOTOR_SetSpeed(0, Speed);
                MOTOR_SetSpeed(1, Speed);

              }
            }
            

            // if(X >= LEFT_MOTOR_FULL_TURN * TurnNb + StartX){
            //   MOTOR_SetSpeed(0, 0);
            // }
            // if(Y >= RIGHT_MOTOR_FULL_TURN * TurnNb + StartY){
            //   MOTOR_SetSpeed(1, 0);
            // }
            // if(X >= LEFT_MOTOR_FULL_TURN * TurnNb + StartX && Y >= RIGHT_MOTOR_FULL_TURN * TurnNb + StartY){
            //   // Serial.println("End");
            //   MOTOR_SetSpeed(0, 0);
            //   MOTOR_SetSpeed(1, 0);
            //   Moving = false;
            // }
          break;
          

          case 1:
            Y = ENCODER_Read(1);
              // Serial.println(Y);
              
              if(Y >= RIGHT_MOTOR_FULL_TURN * TurnNb + StartY){
                MOTOR_SetSpeed(1, 0);
                Moving = false;
              }
          break;
          

          case 0:
            X = ENCODER_Read(0);
              // Serial.print(X);
              if(X >= LEFT_MOTOR_FULL_TURN * TurnNb + StartX){
                // Serial.println("End");
                MOTOR_SetSpeed(0, 0);
                Moving = false;
              
          break;
          }
          case 99:
          default:
            MOTOR_SetSpeed(0, 0);
            MOTOR_SetSpeed(1, 0);
          break;
      }
    }
  } else{
    Mode = 99;
  }
}

void Mouvements::move(float distanceInPulse, float speed)
{
  float turnNb= distanceInPulse / LEFT_MOTOR_FULL_TURN;
  Serial.println(turnNb);
  int mode = 2;
  MotorTurn(mode,turnNb, speed, 0);
}



void Mouvements::progressiveMove(float distanceInCM)
{
  ENCODER_Reset(1);
  float pulseDepart = ENCODER_Read(1);
  float sliceAmountACC=200;
  float speed = 0;
  float middlepulse= pulseDepart +(distanceInCM*PULSE_PAR_CM)/2; //remplacer /2 par 0.90f(accelerer pour 90% de la distance)
  //float milieu=distanceInCM/2;
  float dACC=middlepulse/sliceAmountACC;
  float v = 0.15;
  float vMax=0.8f;
  float aACC = (v - vMax)/(pulseDepart - middlepulse);
  float x=0;

  if(middlepulse < 5000)
    vMax = 0.5f;
  /*Start en readant lencodeur--> continue TANT que que la DISTANCE EN PULSE est 
    plus petite que(DISTANCE EN PULSE + VALEUR INITIALE ENCODEUR)
    start le moteur a 0.15 pour la premiere iteration des 20
    faire des 20 iterations en cm(car move le convertit en tours)
    a chaque fois tu augmente la vitesse de 0.0175*/
    for(v=0.15; v<=vMax; v+=vMax/sliceAmountACC*1.10f)
    {
      // Serial.println(v);
      x=ENCODER_Read(1);
      if(x>=middlepulse){
        Serial.println("Fin ACC");
        break;
      }
    //  for(int i=0; i<sliceAmountDEC; i++)
    // {
    //   v=a*dDEC*i+vdec;

      move(dACC,v);
      //if v=vmax --> move le reste de la distance at vmax***
    }
    if(v>=vMax)
    {
      x=ENCODER_Read(1);
      float drestantePulse=(middlepulse-x);
      move(drestantePulse,vMax);
    }
  //deceleration
  x=ENCODER_Read(1);
  float pulsefinal= pulseDepart +(distanceInCM*PULSE_PAR_CM);
  float vMin=0.10;
  float vdec=v;
  float dDEC=500.f ; //a modifier
  int sliceAmountDEC=(pulsefinal-x)/dDEC ;  //donne le nb de slices (chaque slice est 1000 pulse)
  float aDEC=(vMin-vdec)/((pulsefinal-x));
  Serial.println("DECC");
  for(float i=0.f; i<=sliceAmountDEC; i++)
    {//qqchose pour dire à d'envoyer i à chaque slice donc a chaque 1000, envoyer v pour 1000 slice --> refaire calcul... 
      v=aDEC*dDEC*i+vdec*1.10f;

      x=ENCODER_Read(1);
      // Serial.print(pulsefinal);
      // Serial.print(" ");
      // Serial.print(x);
      // Serial.print(' ');
      // Serial.print(v);
      // Serial.print(" ");
      // Serial.print(dDEC * i);
      // Serial.print(" ");
      // Serial.println(sliceAmountDEC);


      if(v<=vMin)
        v=vMin;
      x=ENCODER_Read(1);
      if(x>=pulsefinal)
        { 
        v=0;
        Serial.println("fin");
        break;
        }
      move(dDEC,v);
    }
    //if v=vmax --> move le reste de la distance at vmax***

      x=ENCODER_Read(1);
      if(x<pulsefinal){
        Serial.println("restant hit");
        move(pulsefinal-x, vMin);
      }

      Serial.print("Restant : ");
      Serial.println(pulsefinal - x);
}

void Mouvements::rotate(float degree) {
  degree /= 2;
  float speed = 0.2;
  float k = 0.03f;
  ENCODER_Reset(GAUCHE);
  ENCODER_Reset(DROITE);
  if (degree > 0) {
    if(degree < 90)
      speed = 0.15;
    // float offsetGauche = ENCODER_Read(0);
    MOTOR_SetSpeed(DROITE, speed);
    MOTOR_SetSpeed(GAUCHE, -speed);
    while (ENCODER_Read(DROITE) < degree * PULSE_PAR_DEG_LEFT) {}
    // float x = ENCODER_Read(0);
    // float y = ENCODER_Read(1);
    // if(y != x){
    //     //diff
    //     float diff = x + y;
    //     if(diff > 0){
    //     //si +grand que 0, y est pas assez grand
    //       float modif = diff * k;
    //       MOTOR_SetSpeed(0, speed);
    //       MOTOR_SetSpeed(1, -speed - modif);
    //     }
    //     else if (diff < 0){
    //     //si - que 0, x est pas assez grand
    //       float modif = -diff * k;
    //       MOTOR_SetSpeed(1, speed);
    //       MOTOR_SetSpeed(0, speed + modif);
    //     }
    //     else{
    //       MOTOR_SetSpeed(0, speed);
    //       MOTOR_SetSpeed(1, -speed);
    //     }
    //   }
  }
  else {
    // float offsetDroite = ENCODER_Read(1);
    if(degree > -90)
      speed = 0.15;
    MOTOR_SetSpeed(DROITE, -speed);
    MOTOR_SetSpeed(GAUCHE, speed);
    while (ENCODER_Read(GAUCHE) < -degree * PULSE_PAR_DEG_RIGHT) {
      // float x = ENCODER_Read(0);
      // float y = ENCODER_Read(1);
      // if(y != x){
      //   //diff
      //   float diff = y + x;
      //   if(diff > 0){
      //   //si +grand que 0, x est pas assez grand
      //     float modif = diff * k;
      //     MOTOR_SetSpeed(0, -speed - modif);
      //     MOTOR_SetSpeed(1, speed);
      //   }
      //   else if (diff < 0){
      //   //si - que 0, y est pas assez grand
      //     float modif = diff * k;
      //     MOTOR_SetSpeed(1, speed + modif);
      //     MOTOR_SetSpeed(0, -speed);
      //   }
      //   else{
      //     MOTOR_SetSpeed(0, -speed);
      //     MOTOR_SetSpeed(1, speed);
      //   }
      // }
    }
  }
  MOTOR_SetSpeed(GAUCHE, 0);
  MOTOR_SetSpeed(DROITE, 0);
  ENCODER_Reset(GAUCHE);
  ENCODER_Reset(DROITE);
}

void Mouvements::MoveBackwards(float distanceInCM, float speed){
  ENCODER_Reset(0);
  ENCODER_Reset(1);

  float distanceInPulse = distanceInCM * PULSE_PAR_CM;

  MOTOR_SetSpeed(0, -speed);
  MOTOR_SetSpeed(1, -speed);

  while(ENCODER_Read(0) > -distanceInPulse && ENCODER_Read(1) > -distanceInPulse){
    //Asservissement
    float x = ENCODER_Read(0);
    float y = ENCODER_Read(1);
    float k = 0.0003f;
    if(y != x){
      //diff
      float diff = y - x;
      if(diff > 0){
      //si +grand que 0, y est pas assez grand
        float modif = diff * k;
        MOTOR_SetSpeed(0, -speed);
        MOTOR_SetSpeed(1, -speed - modif);
      }
      else if (diff < 0){
      //si - que 0, x est pas assez grand
        float modif = diff * k;
        MOTOR_SetSpeed(0, -speed + modif);
        MOTOR_SetSpeed(1, -speed);
      }
      else{
        MOTOR_SetSpeed(0, -speed);
        MOTOR_SetSpeed(1, -speed);
      }
    }
  }
  MOTOR_SetSpeed(0, 0);
  MOTOR_SetSpeed(1, 0);
  ENCODER_Reset(0);
  ENCODER_Reset(1);
}

void Mouvements::TurnRight(){
  float rapport = 1;

  switch (CouleurToFollow){
    case Green:
      rapport = RAPPORT_G;
      break;
    case Red:
      rapport = RAPPORT_R;
      break;
    case Yellow:
      rapport = RAPPORT_Y;
      break;
  }
  Moving = false;
  MotorTurn(3, 10, 0.40, rapport);

}

float Mouvements::ColorPID(){
  //lire capteur
  //comparer couleur avec celle suivie 
  // gauche -1, droite 1, etc
  //diffCouleur = facteur couleur * PID C;
  
  float diff, diffCouleur, Facteur_Couleur ; // = diff + diffCouleur;
  int couleurActuelle = capteurs.Get_Couleur();
  int couleurToFollow = CouleurToFollow;
  
  Facteur_Couleur = couleurToFollow - couleurActuelle; 

  diffCouleur = Facteur_Couleur * K_Couleur;
  diff=0;
  diff = diff + diffCouleur;

  return diff;


  


  
  //si diff < 0, la rouge gauche est modifiée
  //si diff > 0, la rouge droite est modifiée

}


float Mouvements::LignePID(){
  //lire capteur ligne
  bool *suiveur = capteurs.Get_STATE_SUIVEUR(); 
  float diff = 0;
  if (suiveur[2]==true && etat_suiveur[0]==false && etat_suiveur[1]== false)
  {
    etat_suiveur[2]==true;
    etat_suiveur[0]==false;
    etat_suiveur[1]==false;
  }
  else if (suiveur[2]==true && etat_suiveur[0]==true && etat_suiveur[1]== false)
  {
    etat_suiveur[2]==true;
    etat_suiveur[0]==true;
    etat_suiveur[1]==false;
    retour_ligne=-15; //il touche deux ligne, il doit faire virage brusque a droite
  }
else if (suiveur[2]==true && etat_suiveur[0]==true && etat_suiveur[1]== true)
  {
    etat_suiveur[2]==true;
    etat_suiveur[0]==true;
    etat_suiveur[1]==true;
    retour_ligne=-20;//faire un 90 a droite
  }
  else if (suiveur[2]==true && etat_suiveur[0]==false && etat_suiveur[1]== true)
  {
    etat_suiveur[2]==true;
    etat_suiveur[0]==false;
    etat_suiveur[1]==true;
    retour_ligne=-10; // touche deux ligne il doit faire virage brusque a droite
  }
  else if (suiveur[2]==false && etat_suiveur[0]==true && etat_suiveur[1]== false)
  {
    etat_suiveur[2]==false;
    etat_suiveur[0]==true;
    etat_suiveur[1]==false;
    retour_ligne=5;
  }
  else if (suiveur[2]==false && etat_suiveur[0]==true && etat_suiveur[1]== true)
  {
    etat_suiveur[2]==false;
    etat_suiveur[0]==true;
    etat_suiveur[1]==true;
    retour_ligne =-10; // les deux capteur sur les cote sont sur une ligne on le fais virer beaoucoup droite pour essayer de le ramener
  }
  else if (suiveur[2]==false && etat_suiveur[0]==false && etat_suiveur[1]== true)
  {
    etat_suiveur[2]==false;
    etat_suiveur[0]==false;
    etat_suiveur[1]==true;
    retour_ligne=-5;
  }
  else if (suiveur[2]==false && etat_suiveur[0]==false && etat_suiveur[1]== false)
  {
    if (etat_suiveur[0]==1 && etat_suiveur[1]==0 && etat_suiveur[2]==0)
    {
      retour_ligne = 10; //il est sortit des lignes par la droite
    }
    else if (etat_suiveur[0]==0 && etat_suiveur[1]==1 && etat_suiveur[2]==0)
    {
      retour_ligne = -10; //il est sortit des lignes par la gauche
    }
    else
    {
    etat_suiveur[2]==false;
    etat_suiveur[0]==false;
    etat_suiveur[1]==false;
    }
  }
  //trouver une diff en fonction des lignes lues

  //si diff < 0, la rouge gauche est modifiée
  //si diff > 0, la rouge droite est modifiée

  return diff;
}

float Mouvements::JumpPID(){
  //arranger le pid pour que le robot saute
  float diff = 0;

  //si diff < 0, la rouge gauche est modifiée
  //si diff > 0, la rouge droite est modifiée


  return diff;
}

void Mouvements::Jump(){
  //setup les paramètres necessaires pour le saut
  Mode = 6;
  Speed = 0.3;//à déterminer
  MotorTurn(Mode, 10, Speed, 1);
}