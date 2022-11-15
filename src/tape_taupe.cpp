#include "tape_taupe.h"



unsigned long temps_anterieur = 0;

/*  Taupe_1_pin -> 2
    Taupe_2_pin -> 3
    Taupe_3_pin -> 4
    Taupe_4_pin -> 5
*/
void mode_pin_taupes()
{
    pinMode(Taupe_1_pin, OUTPUT);
    //digitalWrite(Taupe_1_pin, LOW);

    pinMode(Taupe_2_pin, OUTPUT);
    //digitalWrite(Taupe_2_pin, LOW);
    
    pinMode(Taupe_3_pin, OUTPUT);
    //digitalWrite(Taupe_3_pin, LOW);
    
    pinMode(Taupe_4_pin, OUTPUT);
    //digitalWrite(Taupe_4_pin, LOW);
}


int Random()
{
    int valeur_taupe = random(4);

    return valeur_taupe;
}


int actionneur_taupes()
{
    int taupe_choisie = Random();
    //***int taupe_frappee;

    // duree d'activation de la taupe
    unsigned long temps_actuel = millis() ;
    unsigned long temps_dactivation = 2000;

    if (taupe_choisie==0)
    {
        //activation de la taupe tant que la duree de temps maximale ne soit pas atteinte
        while (temps_actuel - temps_anterieur > temps_dactivation)
        {
            temps_anterieur = temps_actuel ;
            digitalWrite(Taupe_1_pin, HIGH);
            //*** a faire peut-etre ailleurs: inclure la detection de la switch sur la taupe et ajouter un compteur (taupe_touchee) de points

        }
    }

    else if (taupe_choisie==1)
    {
        //activation de la taupe tant que la duree de temps maximale ne soit pas atteinte
        while (temps_actuel - temps_anterieur > temps_dactivation)
        {
            temps_anterieur = temps_actuel ;
            digitalWrite(Taupe_2_pin, HIGH);
        }
        
    }
    else if (taupe_choisie==2)
    {
        while (temps_actuel - temps_anterieur > temps_dactivation)
        {
            temps_anterieur = temps_actuel ;
            digitalWrite(Taupe_3_pin, HIGH);
        }
    }
    else if (taupe_choisie==3)
    {
        while (temps_actuel - temps_anterieur > temps_dactivation)
        {
            temps_anterieur = temps_actuel ;
            digitalWrite(Taupe_4_pin, HIGH);
        }
        
    }

    return taupe_choisie;
}