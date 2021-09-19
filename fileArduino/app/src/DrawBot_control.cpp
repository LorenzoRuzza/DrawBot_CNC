#include "DRAWBOTLIB.h"  
#include "Arduino.h"


#ifdef FULLSTEP
#define SPOSTAMENTO_PASSO  0.01 // se faccio 2mm a giro e servono 200 passi per fare un giro, faccio 0,01mm a passo (2/200)
#endif
#ifdef MICRO16STEP
#define SPOSTAMENTO_PASSO  0.000625 // se faccio 2mm a giro e servono 3200 passi per fare un giro, faccio 0,000625mm a passo 
#endif




CNC DrawBot;


byte Motore_Z = 9;

const byte Pulsante_cambio_utensile = 8;

const byte Motore_X_step = 3;
const byte Motore_X_dir = 4;
const byte Motore_X_en = 2;

const byte Motore_Y_step = 6;
const byte Motore_Y_dir = 7;
const byte Motore_Y_en = 5;

const byte finecorsa_Xi= 10;
const byte finecorsa_Xf = 11;

const byte finecorsa_Yi= 13;
const byte finecorsa_Yf = 12;

const byte *pPulsante_cambio_utensile = &Pulsante_cambio_utensile;

const byte *pMotore_X_step = &Motore_X_step;
const byte *pMotore_X_dir = &Motore_X_dir;
const byte *pMotore_X_en = &Motore_X_en;

const byte *pMotore_Y_step = &Motore_Y_step;
const byte *pMotore_Y_dir = &Motore_Y_dir;
const byte *pMotore_Y_en = &Motore_Y_en;

const byte *pfinecorsa_Xi = &finecorsa_Xi;
const byte *pfinecorsa_Xf = &finecorsa_Xf;

const byte *pfinecorsa_Yi = &finecorsa_Yi;
const byte *pfinecorsa_Yf = &finecorsa_Yf;





unsigned long t0 = 0;
#define Tmax 500
bool stato_motori = 0; //spenti

void setup(){
  
  Serial.begin(115200);
  DrawBot.inizializza_motore(X, -SPOSTAMENTO_PASSO, pMotore_X_step, pMotore_X_dir, pMotore_X_en);
  DrawBot.inizializza_motore(Y, SPOSTAMENTO_PASSO, pMotore_Y_step, pMotore_Y_dir, pMotore_Y_en);
  DrawBot.inizializza_motore(Motore_Z);

  DrawBot.attiva_finecorsa(X, pfinecorsa_Xi, pfinecorsa_Xf);
  DrawBot.attiva_finecorsa(Y, pfinecorsa_Yi, pfinecorsa_Yf);

  DrawBot.inizializza_pulsante(PULSANTE_CAMBIO_UTENSILE, pPulsante_cambio_utensile);
  
 
  }
void loop(){
  if ((millis() - t0) > Tmax && stato_motori == 1) {
    DrawBot.spegni_motore(X);
    DrawBot.spegni_motore(Y);
    stato_motori = 0;
  }

  if (DrawBot.ingresso_comando()) {

    
    DrawBot.leggi_comando();

    if (stato_motori == 0) {
        DrawBot.accendi_motore(X);
        DrawBot.accendi_motore(Y);
        stato_motori = 1;
    }

    DrawBot.muovi_CNC();
    

    t0 = millis();

  }

  }
