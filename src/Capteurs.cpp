#include "Capteurs.h"

//----- Variable globale -----
bool STATE_SUIVEUR[] = {false, false, false};
float STATE_IR[] = {0, 0, 0};
bool ready = false;
bool start = false;
int Led_couleur_to_Follow = 0;
//R,G,B,Clear
int STATE_RGB[] = {0, 0, 0};

//R,G,B,Y
bool STATE_LED_COULEUR[] = {0,0,0,0};
int Color = 0;

//Bras
float delais_wait = 0;
int bras_etape =0;

//----- Propriétés -----

bool *Capteurs::Get_STATE_LED_COULEUR(){
    return STATE_LED_COULEUR;
}

int *Capteurs::Get_STATE_RGB(){
    return STATE_RGB;
}

float *Capteurs::Get_STATE_IR(){
    return STATE_IR;
}

bool *Capteurs::Get_STATE_SUIVEUR(){
    return STATE_SUIVEUR;
}

bool Capteurs::Get_ready(){
    return ready;
}

bool Capteurs::Get_start(){
    return start;
}


//----- Méthodes -----

void Capteurs::SetCouleurToFollow(int couleur){
    Led_couleur_to_Follow = couleur;

    switch (couleur)
    {
    case Red:
        STATE_LED_COULEUR[0] = true;
        break;
    case Green:
        STATE_LED_COULEUR[1] = true;
        break;
    case Blue:
        STATE_LED_COULEUR[2] = true;
        break;
    case Yellow:   
        STATE_LED_COULEUR[3] = true;
        break;
    default:
        break;
    }

    digitalWrite(OUTPUT_RED, STATE_LED_COULEUR[0]);
    digitalWrite(OUTPUT_GREEN, STATE_LED_COULEUR[1]);
    digitalWrite(OUTPUT_BLUE, STATE_LED_COULEUR[2]);
    digitalWrite(OUTPUT_YELLOW, STATE_LED_COULEUR[3]);
    
}

void Capteurs::ListenBumper(){
  if(ROBUS_IsBumper(3) == 1){
    ready = true;
  }
}

void Capteurs::CapteursInit(){
    tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

    SERVO_Enable(SERVO_BRAS);
    Wire.begin();

    if (tcs.begin()) {
            Serial.println("Found sensor");
    } else {
            Serial.println("No TCS34725 found ... check your connections");
            while (1); // halt!
    }

    // it helps convert RGB colors to what humans see
        for (int i = 0; i < 256; i++) {
            float x = i;
            x /= 255;
            x = pow(x, 2.5);
            x *= 255;

            if (commonAnode) {
                gammatable[i] = 255 - x;
            } else {
                gammatable[i] = x;
            }
            //Serial.println(gammatable[i]);
        }

    //initialiser les LEDs de couleur
    pinMode(OUTPUT_RED, OUTPUT);
    pinMode(OUTPUT_GREEN, OUTPUT);
    pinMode(OUTPUT_BLUE, OUTPUT);
    pinMode(OUTPUT_YELLOW, OUTPUT);
}

void Capteurs::Update_Colors(){
    uint16_t clear, red, green, blue;

    tcs.setInterrupt(false); // turn on LED

    delay(60);  // takes 50ms to read

    tcs.getRawData(&red, &green, &blue, &clear);

    tcs.setInterrupt(true);  // turn off LED

    // Serial.print("C:\t"); Serial.print(clear);
    // Serial.print("\tR:\t"); Serial.print(red);
    // Serial.print("\tG:\t"); Serial.print(green);
    // Serial.print("\tB:\t"); Serial.print(blue);

    // Figure out some basic hex code for visualization
    uint32_t sum = clear;
    float r, g, b;
    r = red; r /= sum;
    g = green; g /= sum;
    b = blue; b /= sum;
    r *= 256; g *= 256; b *= 256;

    // STATE_RGB[0] = (int)r;
    // STATE_RGB[1] = (int)g;
    // STATE_RGB[2] = (int)b;
    // Serial.print("\tR:\t"); Serial.print((int)r);
    // Serial.print("\tG:\t"); Serial.print((int)g);
    // Serial.print("\tB:\t"); Serial.print((int)b);
    // Serial.println();
    Update_ColorFound();
}

void Capteurs::Update_ColorFound(){
    int r = STATE_RGB[0];
    int g = STATE_RGB[1];
    int b = STATE_RGB[2];

    int color = 0;

    // //reset state_led_couleur
    // for(int i = 0; i < 4; i++){
    //     STATE_LED_COULEUR[i] = false;
    // }

    //Range for red
    if(r > 87 && r < 110 && g > 60 && g < 72 && b < 85 && b > 73){
        // STATE_LED_COULEUR[0] = true;
        color = Red;
    }
    else{
        // STATE_LED_COULEUR[0] = false;

    }

    //Range for green
    if(r < 60 && r > 40 && g > 90 && g < 105 && b < 100 && b > 80){
        // STATE_LED_COULEUR[1] = true;
        color = Green;
    }else{
        // STATE_LED_COULEUR[1] = false;

    }

    //Range for blue
    if(r < 50 && r > 30 && g < 85 && g > 70 && b > 95 && b < 130){
        // STATE_LED_COULEUR[2] = true;
        color = Blue;
    }else{
        // STATE_LED_COULEUR[2] = false;

    }

    //Range for yellow
    if(r > 85 && r < 105 && g > 80 && g < 100 && b > 45 && b < 65){
        // STATE_LED_COULEUR[3] = true;
        color = Yellow;
    }else{
        // STATE_LED_COULEUR[3] = false;

    }

    //Range for gray
    if(r > 59 && r < 70 && g > 75 && g < 87 && b > 85 && b < 97){
        color = Gray;
    }

    //Range for black
    if(r < 62 && r > 53 && g > 80 && g < 86 && b < 95 && b > 90){
        color = Black;
    }

    //Range for white
    if(r > 70 && r < 85 && g > 75 && g < 95 && b > 75 && b < 95){
        color = White;
    }

    //Set les pins
    // digitalWrite(OUTPUT_RED, STATE_LED_COULEUR[0]);
    // digitalWrite(OUTPUT_GREEN, STATE_LED_COULEUR[1]);
    // digitalWrite(OUTPUT_BLUE, STATE_LED_COULEUR[2]);
    // digitalWrite(OUTPUT_YELLOW, STATE_LED_COULEUR[3]);

    // if(color == Gray){
    //   digitalWrite(OUTPUT_RED, 1);
    //   digitalWrite(OUTPUT_GREEN, 1);
    //   digitalWrite(OUTPUT_BLUE, 1);
    //   digitalWrite(OUTPUT_YELLOW, 1);
    // }

    Color = color;
}

void Capteurs::Update_IR(){
  //Changer les valeurs pour des valeurs de distance lorsque les calculs seront fait
  STATE_IR[IR_AVANT] = ROBUS_ReadIR(PIN_IR_AVANT);
  STATE_IR[IR_AVANT_DROITE] = ROBUS_ReadIR(PIN_IR_AVANT_DROITE);
  STATE_IR[IR_DROITE] = ROBUS_ReadIR(PIN_IR_DROITE);

  // Serial.print("Avant Haut: ");
  // Serial.print(AH);
  // Serial.print(" Avant Bas: ");
  // Serial.print(AB);
  // Serial.print(" Gauche: ");
  // Serial.print(G);
  // Serial.print(" ");
}

//**** À METTRE DANS LA LOOP ****
//Valeurs du output du suiveur de ligne
//0-10 = les 3 dels sont éteints
//590-600 = led rouge allumé seulement
//300-310 = led jaune allumé seulement
//145-155 = led bleue allumé seulement
//895-905 = led rouge et jaune allumé seulement
//740-755 = led rouge et bleue allumé seulement
//450-460 = led jaune et bleue allumé seulement
//1023 = les 3 dels sont allumés
void Capteurs::Update_Suiveur(){
  int valeurSuiveur = analogRead(SUIVEUR);
  if(valeurSuiveur < 10){
    STATE_SUIVEUR[0] = false;
    STATE_SUIVEUR[1] = false;
    STATE_SUIVEUR[2] = false;
  }
  else if(valeurSuiveur < 600 && valeurSuiveur > 590){
    STATE_SUIVEUR[0] = true;
    STATE_SUIVEUR[1] = false;
    STATE_SUIVEUR[2] = false;
  }
  else if(valeurSuiveur < 310 && valeurSuiveur > 300){
    STATE_SUIVEUR[0] = false;
    STATE_SUIVEUR[1] = true;
    STATE_SUIVEUR[2] = false;
  }
  else if(valeurSuiveur < 155 && valeurSuiveur > 145){
    STATE_SUIVEUR[0] = false;
    STATE_SUIVEUR[1] = false;
    STATE_SUIVEUR[2] = true;
  }
  else if(valeurSuiveur < 905 && valeurSuiveur > 895){
    STATE_SUIVEUR[0] = true;
    STATE_SUIVEUR[1] = true;
    STATE_SUIVEUR[2] = false;
  }
  else if(valeurSuiveur < 755 && valeurSuiveur > 740){
    STATE_SUIVEUR[0] = true;
    STATE_SUIVEUR[1] = false;
    STATE_SUIVEUR[2] = true;
  }
  else if(valeurSuiveur < 460 && valeurSuiveur > 450){
    STATE_SUIVEUR[0] = false;
    STATE_SUIVEUR[1] = true;
    STATE_SUIVEUR[2] = true;
  }
  else{
    STATE_SUIVEUR[0] = true;
    STATE_SUIVEUR[1] = true;
    STATE_SUIVEUR[2] = true;
  }

  // Serial.print("Suiveur: ");
  // Serial.print(" Rouge : ");
  // Serial.print(STATE_SUIVEUR[0]);
  // Serial.print(" Jaune : ");
  // Serial.print(STATE_SUIVEUR[1]);
  // Serial.print(" Bleu : ");
  // Serial.print(STATE_SUIVEUR[2]);
}

void Capteurs::Update_Micro(){
  float ambiant = 0;
  float M5K = 0;
  float diff = 0;

  ambiant = analogRead(MICRO_AMBIANT);
  M5K = analogRead(MICRO_5K);
  diff = M5K - ambiant;

  if(diff >= MICRO_THRESHOLD){
    start = true;
  }
  else{
    start = false;
  }

  // Serial.print("Ambiant: ");
  // Serial.print(ambiant);
  // Serial.print(" 5K: ");
  // Serial.print(M5K);
  // Serial.print(" Diff: ");
  // Serial.print(diff);
  // Serial.print(" State:  ");
  // Serial.println(start);
}

void Capteurs::Swing(){
float delais_actuel = millis();

if (delais_actuel>=delais_wait)

{
  if(bras_etape==0)
  {
    delais_wait=600+delais_actuel;
    SERVO_SetAngle(SERVO_BRAS, 180);
    bras_etape++;
  }
   else if(bras_etape==1)
  {
    delais_wait=600+delais_actuel;
    SERVO_SetAngle(SERVO_BRAS, 0);
    bras_etape++;
  }
   else if(bras_etape==2)
  {
    delais_wait=600+delais_actuel;
    SERVO_SetAngle(SERVO_BRAS, 90);
    bras_etape=0;
  }
}
}

void Capteurs::UpdateCapteurs(){
  Update_IR();
  Update_Suiveur();
  Update_Micro();
  Update_Colors();
}

int Capteurs::Get_Couleur(){
  return Color;
}
