#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

/*******************************************************************************************************/

//definit valeur maximale pour la fonction Ramdon()
//#define RAND_MAX 3

//indique les pines de connexion des taupes
#define Taupe_1_pin 2
#define Taupe_2_pin 3
#define Taupe_3_pin 4
#define Taupe_4_pin 5

/*******************************************************************************************************/


/*donne un nombre entre 0 et 3, qui correspont a une des 4 taupes. */
int Random();

/*Actionne la taupe correspondant a la valeur obtenue par la fonction Random. 
    Si Random() retourne 0 -> Taupe 1 sera activee. 
    Si Random() retourne 1 -> Taupe 2 sera activee.
    Si Random() retourne 2 -> Taupe 3 sera activee. 
    Si Random() retourne 3 -> Taupe 4 sera activee.
*/
int actionneur_taupes();

/*Definit le mode de fonctionnement des pines aux quelles sont liees les taupes comme etant en mode OUTPUT.*/
void mode_pin_taupes();