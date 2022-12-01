#include <LiquidCrystal.h>
#include <Arduino.h>
#include "tape_taupe.h"
#include <MegaServo.h>


bool start = false;

unsigned long temps_start=0;
unsigned long temps_attente_start=0;
const unsigned int temps_delay_start = 2000;
unsigned long temps_scroll = 0;
unsigned long temps_attente_scroll = 0;
const unsigned long temps_entree_scroll = 250;
unsigned long temps_fin = 0;
unsigned long temps_attente_fin = 0;
const unsigned long temps_entree_affichage_fin = 5000;
int i=0;
MegaServo ST;


//deplacer la fonction liquidcristal au cpp.

void setup() {
  // put your setup code here, to run once:
  initialisation_ecran();
  Serial.begin(9600);
  mode_pin_taupes();
  mode_pin_switches_taupes();
  setupServoTaupes();
  //Serial.println(ST.attach(45));
  ChangerTaupe();
  start = false;
  //Serial.print("Bonjour\n");
  
}


void loop()
{
  if (!start)
  {
      if (menu)
      {
        choix_Bouton_etat();
        Menu();
      }
      else if (Fin_de_partie)
      {
        temps_fin = millis();
        if (i==0)
        {
          affichage_Fin_de_Partie();
          i++;
        }
        if (temps_fin>=temps_attente_fin)
        {
          affichage_score_final();
        }
        if (digitalRead(inpin_switch_menu) == LOW)
          {
              retour_menu();
          }
      }
      else{
          if (digitalRead(inpin_switch_menu) == LOW)
            {
                retour_menu();
            }
          temps_scroll = millis();
          if (temps_scroll>= temps_attente_scroll)
          {
            scroll_ordre_debut();
            temps_attente_scroll = temps_scroll + temps_entree_scroll;
          }
          if (digitalRead(inpin_switch_start) == 0)
          {

            start = true;
            temps_start = millis();
            temps_attente_start = temps_start + temps_delay_start;
            Serial.print("La partie commence");
            debut_partie();
          }
          //Serial.print(digitalRead(inpin_switch_start));
        }
      }
        
  else
  { 
    if (pause)
    {
        temps_scroll = millis();
        if (temps_scroll>= temps_attente_scroll)
        {
          affichage_changement_joueur();
          temps_attente_scroll = temps_scroll + temps_entree_scroll;
        }
        
        if (digitalRead(inpin_select_choix) == LOW)
        {
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print("La partie ");
          lcd.setCursor(0,1);
          lcd.print("reprend ");
          pause = false;
          selection_du_joueur();
          temps_actuel_durant_changement = millis();
          temps_changement_joueur = temps_actuel_durant_changement + temps_de_jeu_par_joueur; 
        }
        
    }
    else{
        if (digitalRead(inpin_switch_menu) == LOW)
          {
              retour_menu();
          }
        else 
        {
          temps_scroll = millis();
          if (temps_scroll>= temps_attente_scroll)
          {
            affichage_pointage();
            temps_attente_scroll = temps_scroll + temps_entree_scroll;
          }
          if (mode_de_jeu == multijoueurs)
          {
            changement_de_joueur();
          }
          temps_start = millis();
          if (temps_start >= temps_attente_start)
          {
            taupe_Bouton_etat();
            actionneur_taupes();
          }
        }
    }
  }

}
  

