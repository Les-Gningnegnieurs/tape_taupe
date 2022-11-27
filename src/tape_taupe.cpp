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
const unsigned long temps_verification = 250;
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
const unsigned long temps_de_jeu_par_joueur = 30000; //chaque joueur fait une partie de 1 min et celui avec le meilleur score final gagne.
bool pause = false;
bool Fin_de_partie = false;
unsigned long temps_affichage_de_temps_restant = 0;
unsigned long temps_restant  = 0;
const unsigned long temps_affichage_seconde = 1000;
unsigned long temps_restant_en_secondes = temps_de_jeu_par_joueur;



const int rs = 3, en = 2, d0 = 4, d1 = 5, d2 = 6, d3 = 7, d4 = 8, d5 = 9, d6 = 10, d7 = 11;
LiquidCrystal lcd(rs, en, d0, d1, d2, d3, d4, d5, d6, d7);
int h=0, H=0, v=0, V=0, n=0;

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


void affichage_changement_joueur()
{
    if (v==0)
    {
        lcd.clear();
    }
    lcd.setCursor(0,0);
    lcd.print("changement joueur");
    if (v<37)
    {
        lcd.scrollDisplayLeft();
        lcd.setCursor(0,1);
        lcd.print("appuyer sur ENTER pour continuer");
        v++;
    }
    else
    {
        v=0;
    }
}


void affichage_Fin_de_Partie()
{
    if (h==0)
    {
        lcd.clear();
        h++;
    }
    lcd.setCursor(0,0);
    lcd.print("FIN DE LA PARTIE");
    temps_fin = millis();
    temps_attente_fin = temps_fin + temps_entree_affichage_fin;
}

void affichage_score_final()
{

    int score_final_humain = participants.points_humain;
    int score_final_robot = participants.points_robot;

    if (H==0)
    {
        lcd.clear();
        H++;
    }
    lcd.setCursor(0,0);
    lcd.print("Robot: ");
    lcd.print(score_final_robot);
    lcd.setCursor(0,1);
    lcd.print("Humain: ");
    lcd.print(score_final_humain);
}

void decompte_temps_restant()
{   
    if (V==0)
    {
        lcd.clear();
        V++;
    }
    if (temps_actuel_durant_changement >= temps_affichage_de_temps_restant)
    {
        temps_restant = temps_changement_joueur - temps_actuel_durant_changement;

        temps_restant_en_secondes = ((temps_restant)/1000);
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
    decompte_temps_restant();
    lcd.clear();
    if (joueur_actuel == humain)
    {
        lcd.setCursor(0,0);
        lcd.print("Humain:");
        lcd.print(pointage_actuel_humain);
        lcd.setCursor(0,1);
        lcd.print("Temps: ");
        lcd.print(temps_restant_en_secondes);
    }
    if (joueur_actuel == robot)
    {
        lcd.setCursor(0,0);
        lcd.print("Points Robot:");
        lcd.print(pointage_actuel_robot);
        lcd.setCursor(0,1);
        lcd.print("Temps: ");
        lcd.print(temps_restant_en_secondes);
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
    pinMode(outpin_switch_choix_G,OUTPUT);
    pinMode(outpin_select_choix,OUTPUT);
    pinMode(inpin_switch_choix_G ,INPUT_PULLUP);
    pinMode(inpin_select_choix,INPUT_PULLUP);
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
    Serial.print(taupe_choisie);
    
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
    if (/*(!ancienne_taupe_up) &&*/ (temps_actuel >= temps_attente_de_verification))
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
                //affichage_pointage();
                /*Serial.print("\n\t\tpointage robot\n: ");
                Serial.print("\t\t\t");
                Serial.print(participants.points_robot);
                Serial.print("\t\t\t\n");*/
                coup_sur_taupe = false;
            }
            else 
            {
                lcd.clear();
                participants.points_humain++;
                //affichage_pointage();
                /*Serial.print("\n\t\tpointage humain: ");
                Serial.print("\t\t\t");
                Serial.print(participants.points_humain);
                Serial.print("\t\t\t\n");*/
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
                //affichage_pointage();
                /*Serial.print("\n\t\tpointage robot\n: ");
                Serial.print("\t\t\t");
                Serial.print(participants.points_robot);
                Serial.print("\t\t\t\n");*/
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
void fin_de_partie()
{
    Fin_de_partie = true;
    start = false;
}

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
        temps_changement_joueur = temps_actuel_durant_changement + temps_de_jeu_par_joueur;
        temps_affichage_de_temps_restant = temps_actuel_durant_changement + temps_affichage_seconde;
        switch (j)
        {
        case 0:
            j++;
            break;
        case 1:
            j++;
            pause_changement_jouer();
            break;
        case 2:
            fin_de_partie();
            break;
        }
    }
}


int b = 0;
void pause_changement_jouer()
{
    Serial.print("\nChangement de joueur, la partie reprend dans 30 secondes");
   
    //lcd.clear();
    //b++;
    
    //affichage_changement_joueur();
    digitalWrite(GetTaupeOUTPin(taupe_choisie), LOW);
    pause = true;
    //delay(30000);
    temps_actuel_durant_changement = millis();
    //affichage_pointage();
}

/*int l;
void Fonction_pause() //inpin volee pour star switch. a refaire***
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
}*/


int choix=1;
int choix_anterieur=0;
bool select = false;
bool menu = true;
bool choix_bouton_released = true;
bool choix_bouton_pressed = false;

void choix_Bouton_etat()
{
    //si le bouton etait relachee lors de la derniere lecture, on verifie si maintenant il est enfonce. (taupe_released et taupe_pressed = valeur booleenne)
    if (choix_bouton_released)
    {
        //si il est enfonce (donc taupe_released = vrai), on definit les valeur booleennes des variables.
        if (digitalRead(inpin_switch_choix_G) == LOW)
        { 
            choix_bouton_released = false; //le bouton de detection n'est pas relachee.
            choix_bouton_pressed = true; //le bouton de detection est donc enfonce. 
            Serial.print("bouton choix pressed");
        }
    }
    //si le bouton etait enfoncee lors de la derniere lecture, on verifie si maintenant il est relachee. 
    else if (digitalRead(inpin_switch_choix_G) == HIGH)
    {
        
        choix_bouton_released = true;
        choix_bouton_pressed = false;
        Serial.print("\nbouton choix relachee :) ");
        Serial.print("\n");
    }
    //si le bouton etait enfoncee lors de la derniere lecture et qu'il l'est encore, on le definit comme false car il s'agit du meme coup (donc on ne le compte pas deux fois). 
    else
    {
        choix_bouton_pressed = false;
    }
}

void Select()
{
    if (select)
    {
        switch (choix)
        {
        default:
        case 1:
            //lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("partie solo");
            lcd.setCursor(0,1);
            //lcd.print("selectionnee");
            mode_de_jeu = partie_solo;
            select = false;
            menu = false;
            break;

        case 2:
            //lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("multijoueurs");
            lcd.setCursor(0,1);
            //lcd.print("selectionnee");
            mode_de_jeu = multijoueurs;
            select = false;
            menu = false;
            break;
        
        }
    }
}


void Choix_mode_jeu()
{
    if (choix_bouton_pressed)
    {
        lcd.clear();
        choix++;
        if (choix ==3)
        {
            choix = 1 ;
            //options_choix();
        }
        //choix_bouton_pressed = false;
        //choix_bouton_released = true;
        Serial.print("\nchoix\n");
        Serial.print(choix);
        Serial.print("\n\n");
    }
    switch (choix)
    {
    default:
    case 1:
        lcd.setCursor(0,1);
        lcd.print("partie solo");
        if (digitalRead(inpin_select_choix) == LOW)
        {
            lcd.clear();
            select = true;
        }
        break;

    case 2:
        lcd.setCursor(0,1);
        lcd.print("multijoueur");
        if (digitalRead(inpin_select_choix) == LOW)
        {
            lcd.clear();
            select = true;
        }
        break;
    }
    //Select();
}

void retour_menu()
{
    lcd.clear();
    menu = true;
    start = false;
    Reinitialisation_jeu();
}


int k=1;
void Menu() //a terminer
{
    if (k==1)
    {
        lcd.clear();
        k++;
    }
    lcd.setCursor(0,0);
    lcd.print("Menu: ");

    Choix_mode_jeu();
    Select();
}

void Reinitialisation_jeu()
{
    digitalWrite(GetTaupeOUTPin(taupe_choisie), LOW);
    ChangerTaupe();
    temps_anterieur = 0;
    //taupe_choisie = 0;
    taupe_bouton_pressed = false;
    taupe_bouton_released = true;
    coup_sur_taupe = false;
    taupe_up = false; //detecteur d'etat de la taupe (Montee ou Baissee)
    //ancienne_taupe = 4; //valeur fictive pour pas que lors du demarrage le code lise une bouton coince
    ancienne_taupe_up = true; 
    //********
    mode_de_jeu = multijoueurs;/*******a definir dans les choix au display*******/
    //********
    joueur_actuel = robot;
    temps_actuel = 0 ;
    temps_attente = 0 ;
    temps_attente_de_verification = 0;
    temps_actuel_pour_verification = 0 ;
    bouton_coince = false;
    LED_PROB_ON = false;
    j=0; // valeur pour modulo choisisant joueur actif.
    temps_changement_joueur = 0;
    temps_actuel_durant_changement = 0;
    pause = false;
    Fin_de_partie = false;
    temps_affichage_de_temps_restant = 0;
    temps_restant  = 0;
    temps_restant_en_secondes = temps_de_jeu_par_joueur;
    h=0, H=0, v=0, V=0, n=0;
    start = false;
    temps_start=0;
    temps_attente_start=0;
    temps_scroll = 0;
    temps_attente_scroll = 0;
    temps_fin = 0;
    temps_attente_fin = 0;
    i=0;
    participants.points_humain=0;
    participants.points_robot=0;
}

