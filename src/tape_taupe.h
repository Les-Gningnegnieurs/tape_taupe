#include <Arduino.h>
#include <LiquidCrystal.h>
//#include <LibRobus.h>
//#include <Wire.h>
//#include <Adafruit_TCS34725.h>
//#include <LiquidCrystal_I2C>

/*******************************************************************************************************/

//definit valeur maximale pour la fonction Ramdon()
//#define RAND_MAX 3

//indique les pines de connexion des taupes
#define Taupe_1_pin 25
#define Taupe_2_pin 24
#define Taupe_3_pin 23
#define Taupe_4_pin 22
#define LED_PROBLEME 27

//indique les pines de connexion des switch de detection des coups (sur les taupes)
#define outpin_switch_taupe_1 26 
#define outpin_switch_taupe_2 26
#define outpin_switch_taupe_3 26
#define outpin_switch_taupe_4 26
#define inpin_switch_taupe_1 31
#define inpin_switch_taupe_2 30
#define inpin_switch_taupe_3 29
#define inpin_switch_taupe_4 28

//indique des variables pour le mode de jeu et pour le pointage
#define multijoueurs 2
#define partie_solo 1
#define robot 1
#define humain 2

//fonctionnement jeu
#define outpin_switch_pause 32 
#define inpin_switch_pause 33

#define outpin_switch_choix_G 34
#define outpin_select_choix 34
#define inpin_switch_choix_G 35
#define inpin_select_choix 36

/*Display 
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);*/


/* structure servant de compteur de points*/

struct Pointage
{
    int points_robot = 0;
    int points_humain = 0;
}typedef Pointage;

extern bool menu;
extern bool start;
extern int mode_de_jeu;
extern bool pause;
extern LiquidCrystal lcd;
extern unsigned long temps_changement_joueur;
extern unsigned long temps_actuel_durant_changement;
extern const unsigned long temps_de_jeu_par_joueur;
extern bool Fin_de_partie;
extern unsigned long temps_fin;
extern unsigned long temps_attente_fin;
extern const unsigned long temps_entree_affichage_fin;
extern unsigned long temps_affichage_de_temps_restant;
extern unsigned long temps_restant;
extern const unsigned long temps_affichage_seconde;
extern unsigned long temps_restant_en_secondes;

extern unsigned long temps_start;
extern unsigned long temps_attente_start;
extern const unsigned int temps_delay_start;
extern unsigned long temps_scroll;
extern unsigned long temps_attente_scroll;
extern const unsigned long temps_entree_scroll;
extern unsigned long temps_fin;
extern unsigned long temps_attente_fin;
extern const unsigned long temps_entree_affichage_fin;
extern int i;
/*******************************************************************************************************
 *
 * SECTION COMMANDES TAUPES ET POINTS
 * 
 * *****************************************************************************************************/

void Start();

/*Definit le mode de fonctionnement des pines aux quelles sont liees les taupes (systeme up-down) comme etant en mode OUTPUT.*/
void mode_pin_taupes();

/*Definit le mode de fonctionnement des pines des detecteur de coups (sur les taupes) comme etant en mode OUTPUT ou INPUT.*/
void mode_pin_switches_taupes();

/*donne un nombre entre 0 et 3, qui correspont a une des 4 taupes. */
int Random();

/*Fonction qui change aleatoirement la taupe utilisee par le systeme (elle change 1 fois par loop, lorsque le temps d'activation est terminee ou que le bouton detecteur de coup est actionnee)*/
void ChangerTaupe();

/*fonction permettant de compter le score du robot et du participant*/
void compteur_de_points(int Nb_jouers);

/*Fontion qui retourne le numero de pin correspondant a la taupe choisie par la fonciton ChangerTaupe(). Le "int index" en parametres c'est la taupe choisie par la fonciton ChangerTaupe() et correspond aux valeurs des "case" de la fonction switch.*/
int GetTaupeOUTPin(int index);

/*Fonction qui retourne le numero de pin INPUT correspondant au bouton (detecteur de coup) de la taupe choisie. Le "int idx" en parametres c'est la taupe choisie par la fonciton ChangerTaupe() et correspond aux valeurs des "case" de la fonction switch.*/
int Get_Taupe_Bouton_INPIN(int idx);

/*Fonction qui lit l'etat du bouton (detecteur de coup) de la taupe choisie */
void taupe_Bouton_etat();

void boutons_coinces();

void etat_ancienne_taupe();

void identification_des_taupes_pour_boutons_coinces();

/*Fonction qui permet de faire monter la taupe selectionnee durant un temps d'activation predefini et de la redescendre lorsque ce temps est passee ou que la bonne taupe est frappee (Si la taupe est redescendue,on definit une nouvelle taupe pour la prochaine loop )*/
bool Activation_UP_DOWN_taupes();

/*Actionne la taupe correspondant a la valeur obtenue par la fonction Random. 
    Si Random() retourne 0 -> Taupe 1 sera activee. 
    Si Random() retourne 1 -> Taupe 2 sera activee.
    Si Random() retourne 2 -> Taupe 3 sera activee. 
    Si Random() retourne 3 -> Taupe 4 sera activee.*/
void actionneur_taupes();


/*******************************************************************************************************
 *
 * SECTION FONCTIONNEMENT DU JEU
 * 
 * *****************************************************************************************************/

//si j modulo 2 donne 0 (donc si j est paire alors quand on le divise par 2 il n'y a pas de restes) alors c'est le tour du joueur humain de jouer.
void selection_du_joueur();

void fin_de_partie();

void changement_de_joueur();

/*void temps_pour_changement_joueur();*/

void pause_changement_jouer();

void Reinitialisation_jeu();

//void Fonction_pause();
/*******************************************************************************************************
 * 
 * SECTION DISPLAY
 * 
 * *****************************************************************************************************/
void affichage_Fin_de_Partie();

void affichage_score_final();

void initialisation_ecran();

void affichage_pointage();

void scroll_ordre_debut();

void debut_partie();

void Menu();

void retour_menu();

void choix_Bouton_etat();

void Choix_mode_jeu();

void Select();

void affichage_changement_joueur();