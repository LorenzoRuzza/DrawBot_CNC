#ifndef DRAWBOTLIB_h
#define DRAWBOTLIB_h

#include "Arduino.h"
#include <math.h>
#include <Servo.h>

#include "DRAWBOTLIB_DEFINE.h"
#include "DRAWBOTLIB_STRUCT.h"


class CNC
{

  private:

    //variabili utili a memorizzare lo stato attuale della CNC
    bool finecorsa_attivi_[MAX_MOTORI_STEPPER] = {0};

    int set_tipo_movimento_;
    int set_M_;
    int set_T_;
    bool richiesta_cambio_utensile_;

    float set_cerchio_I_;
    float set_cerchio_J_;
    float set_cerchio_K_;

    float coordinata_attuale_X_;
    float coordinata_attuale_Y_;
    float coordinata_attuale_Z_;

    float rap_spostamento_passo_[MAX_MOTORI_STEPPER] = {0};

    float coordinata_precedente_X_;
    float coordinata_precedente_Y_;

    long passi_X_;
    long passi_Y_;

    //variabili utili alla lettura del comando in ingresso
    String ric_string_ = "";
    float coordinate_X_; //X Y Z set_coordinate__
    float coordinate_Y_;
    float coordinate_Z_;
    
    int set_coordinate_;
    bool cambio_set_coordinate_;

    bool cambia_X_;
    bool cambia_Y_;
    bool cambia_Z_;


    //funzioni private per la lettura del comando
    void leggi_set();
    void leggi_movimento();
    void calcola_passi();
    void aggiorna_coordinata_attuale();


  public:
    CNC();
    
    void inizializza_motore(byte num_motore, float spostamento_passo ,const byte *pStep, const byte *pDir, const byte *pEn);
    void inizializza_motore(byte pin_servo);
    void inizializza_pulsante(byte num_pulsante, const byte *pPulsante);
    void attiva_finecorsa(byte num_motore, const byte *pin_finecorsa_min, const byte *pin_finecorsa_max);
    void attiva_finecorsa(byte num_motore);
    void disattiva_finecorsa(byte num_motore);

    bool ingresso_comando();
    void leggi_comando();
    void invia_feedback(bool stato); 

    int quadrante_cerchio(float Xp, float Yp, float Xc, float Yc);
    double calcola_alpha(float Xp, float Yp, float Xc, float Yc);
    void disegna_quadrante_cerchio_orario(float Xi, float Xf, float raggio, int quadrante, float spostamento_passoX, float spostamento_passoY);

    bool controlla_finecorsa(byte num_motore);
    bool controlla_pulsante(byte num_pulsante);
    void muovi_linea_retta(byte num_motore, long passi);
    void muovi_diagonale(byte num_motore1, byte num_motore2, double passi1, double passi2);
    
    void muovi_cerchio(float coordinata_precedente_X, float coordinata_precedente_Y, //quella dove sono attualmente. Da dove parto
                   float coordinata_attuale_X, float coordinata_attuale_Y,   //quella di arrivo. Dove termiona l'arco
                   float spostamento_per_Xc, float spostamento_per_Yc,  //distanza da coordinata_precedente
                   float spostamento_passoX, float spostamento_passoY,  //quanti mm sono per ogni passo
                   bool orario);
                   
    void homing(byte num_motore, int ripetizioni);

    void accendi_motore(byte num_motore);
    void spegni_motore(byte num_motore);

    void solleva_penna();
    void abbassa_penna();

    void muovi_CNC();
    
    void invia_feedback();
};

#endif
