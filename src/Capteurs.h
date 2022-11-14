#include <Arduino.h>
#include <LibRobus.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>

#ifndef CAPTEURS_H_
#define CAPTEURS_H_

#define PIN_IR_AVANT 2
#define PIN_IR_AVANT_DROITE 3
#define PIN_IR_DROITE 1

#define IR_AVANT 0
#define IR_AVANT_DROITE 2
#define IR_DROITE 3

#define CAPTEUR_LOIN 15

#define SUIVEUR A5

//500ms = 180deg
#define SERVO_BRAS 0

#define MICRO_AMBIANT A11
#define MICRO_5K A10
#define MICRO_THRESHOLD 500

//Couleur
#define commonAnode true

#define Red 100
#define Yellow 101
#define Green 102
#define Blue 103
#define Gray 105
#define White 106
#define Black 106
#define None 107

#define OUTPUT_RED 47
#define OUTPUT_GREEN 45
#define OUTPUT_BLUE 46
#define OUTPUT_YELLOW 44

//DEL

#define DEL_B 46
#define DEL_R 47
#define DEL_J 44
#define DEL_V 45

class Capteurs{

    public:
        //---- Props ----

        ///Retourne l'array des états des LEDs de couleur
        bool *Get_STATE_LED_COULEUR();

        ///Retourne l'array RGB capté par le capteur de couleur
        int *Get_STATE_RGB();

        ///Retourne l'array des distances captés par les capteurs IR
        float *Get_STATE_IR();

        ///Retourne l'array des états des capteurs suiveurs
        bool *Get_STATE_SUIVEUR();

        ///Retourne l'état ready
        bool Get_ready();

        ///Retourne l'état start
        bool Get_start();
        
        //retourne la couleur détectée
        int Get_Couleur();

        void SetCouleurToFollow(int couleur);
        //----- Méthodes -----
        
        /// @brief Swing le bras robot une fois
        void Swing();

        /// @brief Met a jour les états des capteurs (call dans la loop)
        void UpdateCapteurs();

        /// @brief Initialiser les capteurs
        void CapteursInit();

        /// @brief Ecouter l'appui du bumper pour démarrer le robot
        void ListenBumper();

    private:
        Adafruit_TCS34725 tcs;
        byte gammatable[256];


        //---- Méthodes ----

        /// @brief Lire le capteur de couleur et update l'array STATE_RGB
        void Update_Colors();

        /// @brief Lire le capteur de distance et update l'array STATE_IR
        void Update_IR();

        /// @brief Convertir les valeurs RGB en une couleur et allumer la bonne DEL
        void Update_ColorFound();

        /// @brief Lire le capteur de suiveur de ligne et update l'array STATE_SUIVEUR
        void Update_Suiveur();

        /// @brief Lire le capteur de micro et update l'état ready et start selon la fréquence 5kHz
        void Update_Micro();

};
#endif /* CAPTEURS_H_ */