#include <LiquidCrystal.h>
#include <Arduino.h>
#include "tape_taupe.h"
bool start;
unsigned long temps_start=0;
unsigned long temps_attente_start=0;
const unsigned int temps_delay_start = 2000;
unsigned long temps_scroll = 0;
unsigned long temps_attente_scroll = 0;
const unsigned long temps_entree_scroll = 250;



//deplacer la fonction liquidcristal au cpp.

void setup() {
  // put your setup code here, to run once:
  initialisation_ecran();
  Serial.begin(9600);
  mode_pin_taupes();
  mode_pin_switches_taupes();
  ChangerTaupe();
  start = false;
  Serial.print("Bonjour");
  
}

int i=1;
void loop()
{
  
  if (!start)
  {
      temps_scroll = millis();
      if (temps_scroll>= temps_attente_scroll)
      {
        scroll_ordre_debut();
        temps_attente_scroll = temps_scroll + temps_entree_scroll;
      }
      if (i==1)
      {
        Serial.print("Appuyer sur la switch de taupe 1\n");
        i++;
      }
      if ((digitalRead(Get_Taupe_Bouton_INPIN(0)) == 0))
      {
        start = true;
        temps_start = millis();
        temps_attente_start = temps_start + temps_delay_start;
        Serial.print("La partie commence");
        debut_partie();
      }
        
  }
  else
  { 
    Fonction_pause();
    changement_de_joueur();
    temps_start = millis();
    if (temps_start >= temps_attente_start)
    {
      taupe_Bouton_etat();
      actionneur_taupes();
    }
  }
}
  

