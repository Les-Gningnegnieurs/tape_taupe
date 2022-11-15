#include "Capteurs.h"
#include "Mouvements.h"
#include "tape_taupe.h"

int etape = -1;
Capteurs capteurs;
Mouvements mouvements;

int pasDemurCpt = 0;
float CheckVirageDroiteTimeout = 0;
float CheckLigneTimeout = 0;
float CheckTapisTimeout = 0;
float CheckJumpTimeout = 0;
float CheckPoutreTimeout = 0;

bool rotateStarted = false;
bool poutreStarted = false;
int dist_minA = 0;
int dist_maxA = 0;

int dist_minR = 0;
int dist_maxR = 0;

void setup() {
  // put your setup code here, to run once:
  BoardInit();

}


void loop()
{

  Random();
  delay(10000);
}
