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
    pinMode(Taupe_2_pin, OUTPUT);
    pinMode(Taupe_3_pin, OUTPUT);
    pinMode(Taupe_4_pin, OUTPUT);
}

void mode_pin_switches_taupes()
{
    pinMode(outpin_switch_taupe_1, OUTPUT);
    pinMode(outpin_switch_taupe_2, OUTPUT);
    pinMode(outpin_switch_taupe_3, OUTPUT);
    pinMode(outpin_switch_taupe_4, OUTPUT);
    pinMode(inpin_switch_taupe_1, INPUT);
    pinMode(inpin_switch_taupe_2, INPUT);
    pinMode(inpin_switch_taupe_3, INPUT);
    pinMode(inpin_switch_taupe_4, INPUT);
}

int Random()
{
    int valeur_taupe = random(4);

    return valeur_taupe;
}


int actionneur_taupes()
{
    int taupe_choisie = Random();
    bool coup_reussi = false; 

    // duree d'activation de la taupe
    unsigned long temps_actuel = millis() ;
    unsigned long temps_dactivation = 2000;

    if (taupe_choisie == 0)
    {
        //activation de la taupe tant que la duree de temps maximale ne soit pas atteinte
        while (temps_actuel - temps_anterieur > temps_dactivation)
        {
            temps_anterieur = temps_actuel ;
            digitalWrite(Taupe_1_pin, HIGH);

            /*On verifie si la switch est activee (donc si la valeur vu par la pine INPUT = celle donnee par la OUTPUt). */
            if (digitalRead(inpin_switch_taupe_1) == HIGH)
            {
                coup_reussi = true;
            }
        }
        /* On desactive la taupe choisie, et la fonction termine en returnant la valeur de coup_reussi (true ou false)*/
        digitalWrite(Taupe_1_pin, LOW);
        return coup_reussi;
    }
/***********Les etapes sont les memes pour les prochaines boucles "else if"  **************/
    else if (taupe_choisie == 1)
    {
       
        while (temps_actuel - temps_anterieur > temps_dactivation)
        {
            temps_anterieur = temps_actuel ;
            digitalWrite(Taupe_2_pin, HIGH);
            if (digitalRead(inpin_switch_taupe_2) == HIGH)
            {
                coup_reussi = true;
            }
        }
        digitalWrite(Taupe_2_pin, LOW);
        return coup_reussi;
    }
    else if (taupe_choisie == 2)
    {
        while (temps_actuel - temps_anterieur > temps_dactivation)
        {
            temps_anterieur = temps_actuel ;
            digitalWrite(Taupe_3_pin, HIGH);
            if (digitalRead(inpin_switch_taupe_3) == HIGH)
            {
                coup_reussi = true;
            }
        }
        digitalWrite(Taupe_3_pin, LOW);
        return coup_reussi;
    }
    else if (taupe_choisie == 3)
    {
        while (temps_actuel - temps_anterieur > temps_dactivation)
        {
            temps_anterieur = temps_actuel ;
            digitalWrite(Taupe_4_pin, HIGH);
            if (digitalRead(inpin_switch_taupe_4) == HIGH)
            {
                coup_reussi = true;
            }
        }
        digitalWrite(Taupe_4_pin, LOW);
        return coup_reussi;
    }

    return coup_reussi;
}