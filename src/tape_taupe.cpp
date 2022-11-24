#include "tape_taupe.h"
#include <LiquidCrystal.h>

/*******************************************************************************************************
 *
 ***********VARIABLES DE LA SECTION COMMANDES TAUPES ET POINTS***********
 * 
 * *****************************************************************************************************/

/*Variables privees (c'est a dire utilisees seulement par les fonctions dans ce fichier) */
unsigned long temps_anterieur = 0;
int taupe_choisie = 0;
bool taupe_bouton_pressed = false;
bool taupe_bouton_released = true;
bool coup_sur_taupe = false;
bool taupe_up = false; //detecteur d'etat de la taupe (Montee ou Baissee)
int ancienne_taupe = 4; //valeur fictive pour pas que lors du demarrage le code lise une bouton coince
bool ancienne_taupe_up = true; 
//********
int mode_de_jeu = multijoueurs;/*******a definir dans les choix au display*******/
//********
int joueur_actuel = robot;
unsigned long temps_actuel = 0 ;
unsigned long temps_attente = 0 ;
const unsigned long temps_dactivation = 2000;
const unsigned long temps_verification = 750;
unsigned long temps_attente_de_verification = 0;
unsigned long temps_actuel_pour_verification = 0 ;

bool bouton_coince = false;
bool LED_PROB_ON = false;
int taupe_restante_a;
int taupe_restante_b;
int taupe_restante_c;
/*Definit le nom de la structure dans laquelle on compte les points*/
Pointage participants;





/*******************************************************************************************************
 *
 ***********VARIABLES DE LA SECTION COMMANDES TAUPES ET POINTS***********
 * 
 * ****************************************************************************************************/

int j=0; // valeur pour modulo choisisant joueur actif.
unsigned long temps_changement_joueur = 0;
unsigned long temps_actuel_durant_changement = 0;
const unsigned long temps_de_jeu_par_joueur = 60000; //chaque joueur fait une partie de 1 min et celui avec le meilleur score final gagne.
bool pause = false;




const int rs = 3, en = 2, d0 = 4, d1 = 5, d2 = 6, d3 = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);
int n=0;


/*******************************************************************************************************
 * 
 * SECTION DISPLAY
 *  
 * *****************************************************************************************************/




void initialisation_ecran()
{
    lcd.begin(16,2);
    lcd.setCursor(0,0);
    lcd.write("Bonjour");
    lcd.setCursor(0,1);
    //delay(3000);
    //scroll_ordre_debut();
}

void scroll_ordre_debut()
{   
    if (n<39)
    {
        lcd.setCursor(0,1);
        lcd.scrollDisplayLeft();
        lcd.print("Pour commencer, appuyer sur la taupe 1");
        n++;
    }
    else
    {
        n=0;
    }
}

void debut_partie()
{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Debut de partie");
}


void affichage_pointage()
{
    int pointage_actuel_humain = participants.points_humain;
    int pointage_actuel_robot = participants.points_robot;
    
    lcd.clear();
    if (joueur_actuel == humain)
    {
        lcd.setCursor(0,0);
        lcd.print("Humain:");
        lcd.print(pointage_actuel_humain);

    }
    if (joueur_actuel == robot)
    {
        lcd.setCursor(0,0);
        lcd.print("Points Robot:");
        lcd.print(pointage_actuel_robot);
    }
}



/*******************************************************************************************************
 *
 * SECTION COMMANDES TAUPES ET POINTS ***************************************
 * 
 * *****************************************************************************************************/

/*  Taupe_1_pin -> 2
    Taupe_2_pin -> 3
    Taupe_3_pin -> 4
    Taupe_4_pin -> 5
*/
/***IMPORTANT AJOUTER FONCTION EMPECHANT DE REMONTER LA MEME TAUPE DE SUITE !!
 * FONCTION EMPECHANT DE COMPTER LES POINTS SI PLUSIEURS COUPS SIMULTANEES! (SI UP ET BOUTON APPUYE PAS COMPTER POINTS).
 * FONCTION EMPECHANT DE COMPTER LES POINTS SI BOUTON MAINTENUS.
*/

void mode_pin_taupes()
{
    pinMode(Taupe_1_pin, OUTPUT);
    pinMode(Taupe_2_pin, OUTPUT);
    pinMode(Taupe_3_pin, OUTPUT);
    pinMode(Taupe_4_pin, OUTPUT);
    pinMode(LED_PROBLEME, OUTPUT);
    pinMode(outpin_switch_pause,OUTPUT);
    pinMode(inpin_switch_pause,INPUT_PULLUP);
}

void mode_pin_switches_taupes()
{
    pinMode(outpin_switch_taupe_1, OUTPUT);
    pinMode(outpin_switch_taupe_2, OUTPUT);
    pinMode(outpin_switch_taupe_3, OUTPUT);
    pinMode(outpin_switch_taupe_4, OUTPUT);
    pinMode(inpin_switch_taupe_1, INPUT_PULLUP);
    pinMode(inpin_switch_taupe_2, INPUT_PULLUP);
    pinMode(inpin_switch_taupe_3, INPUT_PULLUP);
    pinMode(inpin_switch_taupe_4, INPUT_PULLUP);
}

int Random()
{
    int valeur_taupe = random(4);

    return valeur_taupe;
}

void ChangerTaupe(){
    taupe_choisie = Random();
    if (taupe_choisie == ancienne_taupe)
    {
        ChangerTaupe();
    }
    
}

int GetTaupeOUTPin(int index){
    switch (index)
    {
    default:
    case 0:
        return Taupe_1_pin;
        break;
    case 1:
        return Taupe_2_pin;
        break;
    case 2:
        return Taupe_3_pin;
        break;
    case 3:
        return Taupe_4_pin;
        break;
    }
}

int Get_Taupe_Bouton_INPIN(int idx){
    switch (idx)
    {
        default:
        case 0:
            return inpin_switch_taupe_1;
            break;
        case 1:
            return inpin_switch_taupe_2;
            break;
        case 2:
            return inpin_switch_taupe_3;
            break;
        case 3:
            return inpin_switch_taupe_4;
            break;
    }
}

void identification_des_taupes_pour_boutons_coinces()
{   
    switch (ancienne_taupe)
    {
        default:
        case 0:
            switch (taupe_choisie)
            {
            case 1:
                taupe_restante_a = 2,taupe_restante_b = 3;
                break;
            
            case 2: 
                taupe_restante_a = 1,taupe_restante_b = 3;
                break;
            case 3:
                taupe_restante_a = 1,taupe_restante_b = 2;
                break;
            }
            break;
    
        case 1:
            switch (taupe_choisie)
            {
            case 0:
                taupe_restante_a = 2,taupe_restante_b = 3;
                break;
            
            case 2: 
                taupe_restante_a = 0,taupe_restante_b = 3;
                break;
            case 3:
                taupe_restante_a = 0,taupe_restante_b = 2;
                break;
            }
            break;
        case 2:
            switch (taupe_choisie)
            {
            case 0:
                taupe_restante_a = 1,taupe_restante_b = 3;
                break;
            
            case 1: 
                taupe_restante_a = 0,taupe_restante_b = 3;
                break;
            case 3:
                taupe_restante_a = 0,taupe_restante_b = 1;
                break;
            }
            break;
        case 3:
            switch (taupe_choisie)
            {
            case 0:
                taupe_restante_a = 1,taupe_restante_b = 2;
                break;
            
            case 1: 
                taupe_restante_a = 0,taupe_restante_b = 2;
                break;
            case 2:
                taupe_restante_a = 0,taupe_restante_b = 1;
                break;
            }
            break;
    }
}

void etat_ancienne_taupe()
{
    if (digitalRead(GetTaupeOUTPin(ancienne_taupe)) == LOW)
    {
        ancienne_taupe_up = false;
    }
    else if (digitalRead(GetTaupeOUTPin(ancienne_taupe)) == HIGH)
    {
        ancienne_taupe_up = true;
    }
}

void boutons_coinces()
{
    etat_ancienne_taupe();
    identification_des_taupes_pour_boutons_coinces();
    if ((!ancienne_taupe_up) && (temps_actuel >= temps_attente_de_verification))
    {
        if ((digitalRead(Get_Taupe_Bouton_INPIN(ancienne_taupe)) == 1) && (digitalRead(Get_Taupe_Bouton_INPIN(taupe_restante_a)) == 1) && (digitalRead(Get_Taupe_Bouton_INPIN(taupe_restante_b)) == 1))
        {
            bouton_coince = false;
        }

        else if ((digitalRead(Get_Taupe_Bouton_INPIN(ancienne_taupe)) == 0) || (digitalRead(Get_Taupe_Bouton_INPIN(taupe_restante_a)) == 0) || (digitalRead(Get_Taupe_Bouton_INPIN(taupe_restante_b)) == 0))
        {
            bouton_coince = true;
        }
    }
}



void taupe_Bouton_etat()
{
    //si le bouton etait relachee lors de la derniere lecture, on verifie si maintenant il est enfonce. (taupe_released et taupe_pressed = valeur booleenne)
    if (taupe_bouton_released)
    {
        //si il est enfonce (donc taupe_released = vrai), on definit les valeur booleennes des variables.
        if (digitalRead( Get_Taupe_Bouton_INPIN(taupe_choisie)) == 0)
        { 
            taupe_bouton_released = false; //le bouton de detection n'est pas relachee.
            taupe_bouton_pressed = true; //le bouton de detection est donc enfonce. 
        }
    }
    //si le bouton etait enfoncee lors de la derniere lecture, on verifie si maintenant il est relachee. 
    else if (digitalRead(Get_Taupe_Bouton_INPIN(taupe_choisie)) == 1)
    {
        
        taupe_bouton_released = true;
        taupe_bouton_pressed = false;
        Serial.print("\nbouton relachee :) ");
        Serial.print("\n");
    }
    //si le bouton etait enfoncee lors de la derniere lecture et qu'il l'est encore, on le definit comme false car il s'agit du meme coup (donc on ne le compte pas deux fois). 
    else
    {
        taupe_bouton_pressed = false;
    }
    
}


bool Activation_UP_DOWN_taupes(){
    if (!bouton_coince)
    {
        //Si la taupe est descendue, on entre dans le "if" et on monte la taupe, on part le timer.
        if (!taupe_up)
        {
            digitalWrite(GetTaupeOUTPin(taupe_choisie), HIGH);
            taupe_up = true;
            temps_attente = temps_actuel + temps_dactivation;
        }
        //Verifie si le temps d'activation est encore respectee. Si oui, on verifie si le bouton de detecteur de coup est actionne.
        if (temps_actuel <= temps_attente)
        {
            /*On verifie si la switch est activee (donc si la valeur vu par la pine INPUT = celle donnee par la pine OUTPUt). */
            if (taupe_bouton_pressed)
            {
                coup_sur_taupe = true;
            }
        }
        if ((coup_sur_taupe) || (temps_actuel >= temps_attente)  )
        {
            /* On desactive la taupe choisie*/ 
            digitalWrite(GetTaupeOUTPin(taupe_choisie), LOW);
            temps_attente_de_verification = temps_actuel + temps_verification;
            /*on indique que la taupe n'est pas en position montee*/
            taupe_up = false;
            /*On definit une nouvelle taupe pour la prochaine loop*/
            ancienne_taupe = taupe_choisie;
            ChangerTaupe();
        }
    }
    return coup_sur_taupe;
}

void compteur_de_points()
{
    switch (mode_de_jeu)
    {
    case multijoueurs:
        if (coup_sur_taupe)
        {
            if (joueur_actuel == robot)
            {
                lcd.clear();
                participants.points_robot++;
                affichage_pointage();
                Serial.print("\n\t\tpointage robot\n: ");
                Serial.print("\t\t\t");
                Serial.print(participants.points_robot);
                Serial.print("\t\t\t\n");
                coup_sur_taupe = false;
            }
            else 
            {
                lcd.clear();
                participants.points_humain++;
                affichage_pointage();
                Serial.print("\n\t\tpointage humain: ");
                Serial.print("\t\t\t");
                Serial.print(participants.points_humain);
                Serial.print("\t\t\t\n");
                coup_sur_taupe = false;
            }
        }
        break;
    case partie_solo:
    
        if (coup_sur_taupe)
        {
             if (joueur_actuel == robot)
            {
                lcd.clear();
                participants.points_robot++;
                affichage_pointage();
                Serial.print("\n\t\tpointage robot\n: ");
                Serial.print("\t\t\t");
                Serial.print(participants.points_robot);
                Serial.print("\t\t\t\n");
                coup_sur_taupe = false;
            }
        }
        break;
    }
}

void actionneur_taupes()
{
    // lecture du temps depuis le debut du programme
    temps_actuel = millis() ;
    // = selection du mode de jeu;
    if (mode_de_jeu == multijoueurs)
    {
        //verifier l'indentificateur de tour 
        //Appelle les fonctions pour faire monter/descendre les taupes et pour compter les points.
        //multijouer a retravailler
        if (!bouton_coince)
        {
            if (LED_PROB_ON)
            {
                digitalWrite(LED_PROBLEME,LOW);
                LED_PROB_ON = false;
            }
            
            boutons_coinces();
            Activation_UP_DOWN_taupes();
            compteur_de_points();
            
        }
        else if (bouton_coince)
        {
            digitalWrite(GetTaupeOUTPin(taupe_choisie), LOW);
            if (!LED_PROB_ON)
            {
                Serial.print("\n\nbouton coincee!!!\n\n");
                digitalWrite(LED_PROBLEME,HIGH);
                LED_PROB_ON = true;
            }
            boutons_coinces();
        }
    }
    else if (mode_de_jeu == partie_solo)
    {
        
        if (!bouton_coince)
        {
            if (LED_PROB_ON)
            {
                digitalWrite(LED_PROBLEME,LOW);
                LED_PROB_ON = false;
            }
            
            boutons_coinces();
            Activation_UP_DOWN_taupes();
            compteur_de_points();
            
        }
        else if (bouton_coince)
        {
            digitalWrite(GetTaupeOUTPin(taupe_choisie), LOW);
            if (!LED_PROB_ON)
            {
                Serial.print("\n\nbouton coincee!!!\n\n");
                digitalWrite(LED_PROBLEME,HIGH);
                LED_PROB_ON = true;
            }
            boutons_coinces();
        }
    }
}

/*******************************************************************************************************
 *
 * SECTION FONCTIONNEMENT DU JEU
 * 
 * *****************************************************************************************************/

void selection_du_joueur()
{
    if (j%2==0) // si j modulo 2 donne 0 (donc si j est paire et si quand on divise par 2 il n'y a pas de restes) alors c'est le tour du joueur humain de jouer.
    {
        joueur_actuel = humain;
    }
    else
    {
        joueur_actuel = robot;
    }
}

void changement_de_joueur()
{
    temps_actuel_durant_changement = millis();
    
    
    if (temps_actuel_durant_changement >= temps_changement_joueur)
    {
        if (j>0)
        {
            pause_changement_jouer();
        }
        temps_changement_joueur = temps_actuel_durant_changement + temps_de_jeu_par_joueur;
        Serial.print("\ntemps_actuel_durant_changement : ");
        Serial.print(temps_actuel_durant_changement);
        Serial.print("\n");
        Serial.print("\ntemps_changement_joueur : ");
        Serial.print(temps_changement_joueur);
        Serial.print("\n");
        j++;
        selection_du_joueur();
        Serial.print("\nJoueur_actuel : ");
        Serial.print(joueur_actuel);
        Serial.print("\n");
        //pause_changement_jouer();
    }
}

/*void temps_pour_changement_joueur()//a enlever
{
    temps_actuel_durant_changement = millis();
    temps_changement_joueur = temps_actuel_durant_changement + temps_de_jeu_par_joueur;
}*/

void pause_changement_jouer()
{
    Serial.print("\nChangement de joueur, la partie reprend dans 30 secondes");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Changement joeur");
    lcd.setCursor(0,1);
    lcd.print("Attendre 30s");
    digitalWrite(GetTaupeOUTPin(taupe_choisie), LOW);
    delay(30000);
    temps_actuel_durant_changement = millis();
    affichage_pointage();
}

int l;
void Fonction_pause()
{
    if (digitalRead(inpin_switch_pause) == HIGH)
    {   
        l=0;
    }
    else if (digitalRead(inpin_switch_pause) == LOW)
    {
        if (l==0)
        {
            Serial.print("Le jeu est en pause");
            l++;
        }
        Fonction_pause();
        
    }
}
