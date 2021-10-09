#include "DRAWBOTLIB.h"

//#include "Arduino.h"


Motore_stepper motore_asse[MAX_MOTORI_STEPPER];
Finecorsa finecorsa_asse[MAX_MOTORI_STEPPER];
Pulsante bottone[MAX_BOTTONI];

Servo motore_punta;

CNC::CNC()
    {
      coordinate_X_ = 0.0f;
      coordinate_Y_ = 0.0f;
      coordinate_Z_ = 0.0f;
      
      set_coordinate_ = INCREMENTALI;
      cambio_set_coordinate_ = 0;

      set_tipo_movimento_ = RETTA;

      set_M_ = START;
      set_T_ = UTENSILE0;
      richiesta_cambio_utensile_ = 0;

      coordinata_attuale_X_ = 0.0f;
      coordinata_attuale_Y_ = 0.0f;

      coordinata_precedente_X_ = coordinata_attuale_X_;
      coordinata_precedente_Y_ = coordinata_attuale_Y_;


      cambia_X_ = 0;
      cambia_Y_ = 0;
      cambia_Z_ = 0;
      cambio_set_coordinate_ = 0;
};


void CNC::inizializza_motore(byte num_motore, float spostamento_passo , const byte *pStep, const byte *pDir, const byte *pEn)
{
  if (num_motore < MAX_MOTORI_STEPPER) {
  motore_asse[num_motore].pin_step = pStep;
  motore_asse[num_motore].pin_dir = pDir;
  motore_asse[num_motore].pin_en = pEn;

  rap_spostamento_passo_[num_motore] = spostamento_passo;

  pinMode(*motore_asse[num_motore].pin_step, OUTPUT);
  pinMode(*motore_asse[num_motore].pin_dir, OUTPUT);
  pinMode(*motore_asse[num_motore].pin_en, OUTPUT);

  digitalWrite(*motore_asse[num_motore].pin_step, LOW);
  digitalWrite(*motore_asse[num_motore].pin_dir, LOW);
  digitalWrite(*motore_asse[num_motore].pin_en, HIGH); // in origine spento
  }
}

void CNC::inizializza_pulsante(byte num_pulsante, const byte *pPulsante) 
{
  if (num_pulsante < MAX_BOTTONI) {
  bottone[num_pulsante].pin_pulsante = pPulsante;
  pinMode(*bottone[num_pulsante].pin_pulsante, INPUT);
  }
}

void CNC::inizializza_motore(byte pin_servo)
{
  motore_punta.attach(pin_servo);
  solleva_penna();
}

void CNC::attiva_finecorsa(byte num_motore, const byte *pin_finecorsa_min, const byte *pin_finecorsa_max)
{
  if (num_motore < MAX_MOTORI_STEPPER) {
  finecorsa_asse[num_motore].pin_finecorsa_max = pin_finecorsa_max;
  finecorsa_asse[num_motore].pin_finecorsa_min = pin_finecorsa_min;

  pinMode(*finecorsa_asse[num_motore].pin_finecorsa_max, INPUT);
  pinMode(*finecorsa_asse[num_motore].pin_finecorsa_min, INPUT);

  finecorsa_attivi_[num_motore] = 1;
  }
}

void CNC::disattiva_finecorsa(byte num_motore)
{
      finecorsa_attivi_[num_motore] = 0; 
}

void CNC::attiva_finecorsa(byte num_motore)
{
      finecorsa_attivi_[num_motore] = 1; 
}

bool CNC::controlla_finecorsa(byte num_motore)
{
  if (finecorsa_attivi_[num_motore] == 1) {
    if (digitalRead(*finecorsa_asse[num_motore].pin_finecorsa_min) == 0 || digitalRead(*finecorsa_asse[num_motore].pin_finecorsa_max) == 0)
      return 1;
  }

    return 0;
}

bool CNC::controlla_pulsante(byte num_pulsante)
{
    if (digitalRead(*bottone[num_pulsante].pin_pulsante) == 0 )
      return 1;
      
    return 0;
}

void CNC::spegni_motore(byte num_motore)
{
    digitalWrite(*motore_asse[num_motore].pin_en, HIGH);
}

void CNC::accendi_motore(byte num_motore)
{
      digitalWrite(*motore_asse[num_motore].pin_en, LOW);
      delay(2);
}


void CNC::muovi_linea_retta(byte num_motore, long passi)
{

  if (passi < 0) {
    digitalWrite(*motore_asse[num_motore].pin_dir, LOW);
    passi = -passi;
  } else {
    digitalWrite(*motore_asse[num_motore].pin_dir, HIGH);
  }

  for (long i = 0; i < passi; i++) {
if(controlla_finecorsa(num_motore) == 0) {
      digitalWrite(*motore_asse[num_motore].pin_step, HIGH);
      delayMicroseconds(VELOCITA); 
      digitalWrite(*motore_asse[num_motore].pin_step, LOW);
      delayMicroseconds(VELOCITA);  
  }
  else
    break;
}

}


void CNC::muovi_diagonale(byte num_motore1, byte num_motore2, double passi1, double passi2) { 
  double rapporto;  
  long rapporto_int;
  double rapporto_decimale;
  double cont_decimali = 0;
  long comp = 0;

  if (passi1 == 0 || passi2 == 0) {
    muovi_linea_retta(num_motore1, passi1);
    muovi_linea_retta(num_motore2, passi2);
  } else {
  
  if (abs(passi1) <= abs(passi2)) {

    rapporto = abs(passi2) / abs(passi1);
    rapporto_int = (long)rapporto;
    rapporto_decimale = rapporto - rapporto_int;

      
    for (long i = 0; i < abs(passi1); i++) {
      if (passi2 >= 0) {
        muovi_linea_retta(num_motore2, rapporto_int + comp);
      }
      else {
        muovi_linea_retta(num_motore2, -rapporto_int - comp);
      }

      if (passi1 >= 0) {
        muovi_linea_retta(num_motore1, 1);
      }
      else {
        muovi_linea_retta(num_motore1, -1);
      }
      
    cont_decimali = cont_decimali + rapporto_decimale;
    
    if(cont_decimali >= 1) {
      comp = 1;
      cont_decimali = cont_decimali - 1;
    } else {
      comp = 0;
    }
    }
  } else { 
    rapporto = abs(passi1) / abs(passi2);
    rapporto_int = (long)rapporto;
    rapporto_decimale = rapporto - rapporto_int;
    
      
    for (long i = 0; i < abs(passi2); i++) {
      if (passi1 >= 0) {
        muovi_linea_retta(num_motore1, rapporto_int + comp);
      }
      else {
        muovi_linea_retta(num_motore1, -rapporto_int - comp);
      }

      if (passi2 >= 0) {
        muovi_linea_retta(num_motore2, 1);
      }
      else {
        muovi_linea_retta(num_motore2, -1);
      }
      
    cont_decimali = cont_decimali + rapporto_decimale;
    
    if(cont_decimali >= 1) {
      comp = 1;
      cont_decimali = cont_decimali - 1;
    }
    else {
      comp = 0;
    }
    }
  }
  }

}

int CNC::quadrante_cerchio(float Xp, float Yp, float Xc, float Yc) {
  int quadrante;
  if (Xp >= Xc && Yp > Yc)
    quadrante = 1;
  if (Xp > Xc && Yp <= Yc)
    quadrante = 4;
  if (Xp <= Xc && Yp < Yc)
    quadrante = 3;
  if (Xp < Xc && Yp >= Yc)
    quadrante = 2;

  return quadrante;

}

double CNC::calcola_alpha(float Xp, float Yp, float Xc, float Yc)
{
  double alpha;

  double m = (Yc - Yp) / (Xc - Xp);
  alpha = atan(abs(m));
  if (quadrante_cerchio(Xp, Yp, Xc, Yc) == 2)
    alpha = 2 * Pi - alpha;

  if (quadrante_cerchio(Xp, Yp, Xc, Yc) == 3)
    alpha = Pi + alpha;

  if (quadrante_cerchio(Xp, Yp, Xc, Yc) == 4)
    alpha = Pi - alpha;

  return alpha;
}



void CNC::disegna_quadrante_cerchio_orario(float Xi, float Xf, float raggio, int quadrante, float spostamento_passoX, float spostamento_passoY) { //inizio deve essere prima di fine
 
  float puntoX;
  float raggio_quad = raggio * raggio;
  float puntoY = raggio;
  float precY = puntoY;
  
  float diffY;
  float comp = 0;
  float cont_comp = 0;
  
  int accuratezza = 5;

   
   long max_punti = abs(((raggio)/(spostamento_passoX * accuratezza))); 
   long diff_punti = max_punti - abs((abs(Xf-Xi)/(spostamento_passoX * accuratezza)));
  
  #ifdef DEBUG
  Serial.print("max_punti ");
  Serial.println(max_punti);
  Serial.print("diff_punti ");
  Serial.println(diff_punti);
  #endif
  
if(quadrante == 2) {
  puntoY = 0;
  for (long i = diff_punti; i < max_punti; i++) {    
    puntoX = -raggio + (abs(spostamento_passoX) * accuratezza * (i));
    precY = puntoY;
    puntoY = (float)sqrt(raggio_quad - (puntoX * puntoX));
    muovi_linea_retta(X, accuratezza);
    diffY = puntoY - precY;
    if (abs(diffY) < spostamento_passoY) {
      cont_comp = cont_comp + abs(diffY);
      if (cont_comp >= spostamento_passoY) {
        cont_comp = cont_comp - spostamento_passoY;
        comp = spostamento_passoY;
      } else {
        comp = 0;
      }
    }

    muovi_linea_retta(Y, (diffY + comp) / spostamento_passoY);
    if (puntoX >= Xf)
      break;
  }
}

if(quadrante == 1) {
  puntoY = raggio;
  for (long i = diff_punti; i < max_punti; i++) { 
    puntoX = (abs(spostamento_passoX) * accuratezza * i);
    precY = puntoY;
    puntoY = (float)sqrt(raggio_quad - (puntoX * puntoX));
    muovi_linea_retta(X, accuratezza);
    diffY = puntoY - precY;
    if (abs(diffY) < spostamento_passoY) {
      cont_comp = cont_comp + abs(diffY);
      if (cont_comp >= spostamento_passoY) {
        cont_comp = cont_comp - spostamento_passoY;
        comp = -spostamento_passoY;
      } else {
        comp = 0;
      }
    }

   muovi_linea_retta(Y, (diffY + comp) / spostamento_passoY);
    if (puntoX >= Xf)
      break;
  }
}

if(quadrante == 4) {
  puntoY = 0;
  for (long i = diff_punti; i < max_punti; i++) { 
    puntoX = raggio - (abs(spostamento_passoX) * accuratezza * i);
    precY = puntoY;
    puntoY = (float)-sqrt(raggio_quad - (puntoX * puntoX));
    muovi_linea_retta(X, -accuratezza);
    diffY = puntoY - precY;
    if (abs(diffY) < spostamento_passoY) {
      cont_comp = cont_comp + abs(diffY);
      if (cont_comp >= spostamento_passoY) {
        cont_comp = cont_comp - spostamento_passoY;
        comp = -spostamento_passoY;
      } else {
        comp = 0;
      }
    }

    muovi_linea_retta(Y, (diffY + comp) / spostamento_passoY);
    if (puntoX <= Xf)
      break;
  }
}

if(quadrante == 3) {
  puntoY = -raggio;
  for (long i = diff_punti; i < max_punti; i++) { 
    puntoX = -(abs(spostamento_passoX) * accuratezza * (i));
    precY = puntoY;
    puntoY = (float)-sqrt(raggio_quad - (puntoX * puntoX));
    muovi_linea_retta(X,-accuratezza);
    diffY = puntoY - precY;
    if (abs(diffY) < spostamento_passoY) {
      cont_comp = cont_comp + abs(diffY);
      if (cont_comp >= spostamento_passoY) {
        cont_comp = cont_comp - spostamento_passoY;
        comp = spostamento_passoY;
      } else {
        comp = 0;
      }
    }

    muovi_linea_retta(Y, (diffY + comp) / spostamento_passoY);
    if (puntoX <= Xf)
      break;
  }
}

}





void CNC::muovi_cerchio(float coordinata_precedente_X, float coordinata_precedente_Y, //quella dove sono attualmente. Da dove parto
                   float coordinata_attuale_X, float coordinata_attuale_Y,   //quella di arrivo. Dove termiona l'arco
                   float spostamento_per_Xc, float spostamento_per_Yc,  //distanza da coordinata_precedente
                   float spostamento_passoX, float spostamento_passoY,  //quanti mm sono per ogni passo
                   bool orario)
{

  if(spostamento_passoX < 0)
    spostamento_per_Xc = -spostamento_per_Xc;
  if(spostamento_passoY < 0)
    spostamento_per_Yc = -spostamento_per_Yc;
    
  float centro_X = coordinata_precedente_X + spostamento_per_Xc;
  float centro_Y = coordinata_precedente_Y + spostamento_per_Yc;

  float raggio_quad = pow(coordinata_precedente_X - centro_X, 2) + pow(coordinata_precedente_Y - centro_Y, 2);
  float raggio = sqrt(raggio_quad);

  float Xi_rel = coordinata_precedente_X - centro_X;
  float Yi_rel = coordinata_precedente_Y - centro_Y;
  float Xf_rel = coordinata_attuale_X - centro_X ;
  float Yf_rel = coordinata_attuale_Y - centro_Y ;

  int quadrante_I = quadrante_cerchio(coordinata_precedente_X, coordinata_precedente_Y, centro_X, centro_Y);
  int quadrante_F = quadrante_cerchio(coordinata_attuale_X, coordinata_attuale_X, centro_X, centro_Y);
  int diff_quadranti = 0;
  int cont_quadranti = 0;
  int attuale_quadrante = quadrante_I;
  
  float estremiI[] = {0, raggio,0, -raggio};
  float estremiF[] = {raggio, 0,-raggio, 0};

  #ifdef DEBUG
    Serial.println();
    Serial.print("centro_X ");
    Serial.println(centro_X);
    Serial.print("centro_Y ");
    Serial.println(centro_Y);
    Serial.print("coordinata_precedente_X ");
    Serial.println(coordinata_precedente_X);
    Serial.print("coordinata_precedente_Y ");
    Serial.println(coordinata_precedente_Y);
    Serial.print("spostamento_per_Xc ");
    Serial.println(spostamento_per_Xc);
    Serial.print("spostamento_per_Yc ");
    Serial.println(spostamento_per_Yc);
    Serial.print("raggio ");
    Serial.println(raggio);
  #endif

  
  if (quadrante_I != quadrante_F) {

    if (quadrante_I == 1) {
      if (quadrante_F == 2)
        diff_quadranti = 4;
      if (quadrante_F == 3)
        diff_quadranti = 3;
      if (quadrante_F == 4)
        diff_quadranti = 2;
    }

     if (quadrante_I == 2) {
      if (quadrante_F == 1)
        diff_quadranti = 2;
      if (quadrante_F == 3)
        diff_quadranti = 4;
      if (quadrante_F == 4)
        diff_quadranti = 3;
    }

     if (quadrante_I == 3) {
      if (quadrante_F == 1)
        diff_quadranti = 3;
      if (quadrante_F == 2)
        diff_quadranti = 2;
      if (quadrante_F == 4)
        diff_quadranti = 4;
    }

     if (quadrante_I == 4) {
      if (quadrante_F == 1)
        diff_quadranti = 4;
      if (quadrante_F == 2)
        diff_quadranti = 3;
      if (quadrante_F == 3)
        diff_quadranti = 2;
    }
 // diff_quadranti--;


    
  } else {
    if (calcola_alpha(Xi_rel, Yi_rel, 0, 0) < calcola_alpha(Xf_rel, Yf_rel, 0, 0)) 
      diff_quadranti = 1;
    else
      diff_quadranti = 4;
  }
// diff_quadranti = 4;
          #ifdef DEBUG
          Serial.println();
      Serial.println("quadrante_I");
      Serial.println(quadrante_I);
      Serial.println("quadrante_F");
      Serial.println(quadrante_F);

      Serial.println("Xi_rel");
      Serial.println(Xi_rel);
      Serial.println("Yi_rel");
      Serial.println(Yi_rel);

      Serial.println("Xf_rel");
      Serial.println(Xf_rel);
      Serial.println("Yf_rel");
      Serial.println(Yf_rel);
      
      Serial.println("cont_quadranti");
      Serial.println(cont_quadranti);
      Serial.println("diff_quadranti");
      Serial.println(diff_quadranti);
        #endif
  
  if (orario == 1) { //quadrante 2 1 4 3

      while(cont_quadranti < diff_quadranti)
      {
        #ifdef DEBUG
        Serial.println();
      Serial.println("cont_quadranti");
      Serial.println(cont_quadranti);
      Serial.println("diff_quadranti");
      Serial.println(diff_quadranti);
        #endif

        if (attuale_quadrante == quadrante_I && diff_quadranti == 1){
        #ifdef DEBUG
        Serial.println("caso1");
        #endif
          disegna_quadrante_cerchio_orario( Xi_rel, Xf_rel, raggio, attuale_quadrante, spostamento_passoX, spostamento_passoY);
           
        }
          
        if(attuale_quadrante == quadrante_I && diff_quadranti != 1){
        #ifdef DEBUG
        Serial.println("caso2");
        #endif
          disegna_quadrante_cerchio_orario( Xi_rel, estremiF[attuale_quadrante-1], raggio, attuale_quadrante, spostamento_passoX, spostamento_passoY);
          
        }

        if(attuale_quadrante == quadrante_F && diff_quadranti != 1 && cont_quadranti != 0){
        #ifdef DEBUG
        Serial.println("caso3");
        #endif
          disegna_quadrante_cerchio_orario( estremiI[attuale_quadrante-1], Xf_rel, raggio, attuale_quadrante, spostamento_passoX, spostamento_passoY);
          
        }
          
        if(attuale_quadrante != quadrante_F && attuale_quadrante != quadrante_I){
        #ifdef DEBUG
        Serial.println("caso4");
        #endif
          disegna_quadrante_cerchio_orario( estremiI[attuale_quadrante-1], estremiF[attuale_quadrante-1], raggio, attuale_quadrante, spostamento_passoX, spostamento_passoY);
          
        }
          
        attuale_quadrante--;
        if (attuale_quadrante == 0)
          attuale_quadrante = 4;

        cont_quadranti++;
        
    }
  }
  if (orario == 0) {

  }



}

void CNC::homing(byte num_motore, int ripetizioni)
{
  int segno = 1;
  if (rap_spostamento_passo_[num_motore] < 0 )
     segno = -1;
    
  if ( finecorsa_attivi_[num_motore] == 1) {
    for (int i = 0; i < ripetizioni; i++) {
      while (controlla_finecorsa(num_motore) == 0) {
        muovi_linea_retta(num_motore, -1*segno);
      }
      finecorsa_attivi_[num_motore] = 0; //disabilita finecorsa
      delay(100);
      muovi_linea_retta(num_motore, 100*segno*RAPPORTO);
      delay(100);
      finecorsa_attivi_[num_motore] = 1; //riattiva finecorsa
    }
  }
}


void CNC::solleva_penna()
{
    motore_punta.write(0);
}

void CNC::abbassa_penna()
{
    motore_punta.write(90);
    delay(100);
}

void CNC::muovi_CNC() 
{

  if (cambia_Z_ == 1) {
    if (coordinata_attuale_Z_ <= 0) 
      abbassa_penna();
    else
      solleva_penna();
  }
    
   if ((set_tipo_movimento_ == RETTA || set_tipo_movimento_ == RAPIDO) && (passi_X_ != 0 || passi_Y_ != 0)){

    if (set_tipo_movimento_ == RETTA)
      abbassa_penna();
    else 
      solleva_penna();
    
    if(cambia_X_ == 1 && cambia_Y_ == 0)
      muovi_linea_retta(X, passi_X_);
      
    if(cambia_Y_ == 1 && cambia_X_ == 0)
      muovi_linea_retta(Y, passi_Y_);

    if(cambia_Y_ == 1 && cambia_X_ == 1)
      muovi_diagonale(X, Y, passi_X_, passi_Y_);
      
    }

    if (set_tipo_movimento_ == ORARIO && (set_cerchio_I_ != 0 || set_cerchio_J_ != 0)) {
      abbassa_penna();
      muovi_cerchio(coordinata_precedente_X_, coordinata_precedente_Y_, coordinata_attuale_X_, coordinata_attuale_Y_,
                    set_cerchio_I_, set_cerchio_J_, rap_spostamento_passo_[X], rap_spostamento_passo_[Y], 1);

    }

   if (set_tipo_movimento_ == HOMING) {

    solleva_penna();

      if (cambia_Y_ == 0 && cambia_Y_ == 0) {
        homing(X, 3); 
        homing(Y, 3); 
      }
    
      if (cambia_X_ == 1)
        homing(X, 3); //effettuo 3 volte la procedura
      
      if (cambia_Y_ == 1)
        homing(Y, 3); 
    }

    if (set_M_ == FINECORSA_ON) {
      attiva_finecorsa(X);
      attiva_finecorsa(Y);
    }

    if (set_M_ == FINECORSA_OFF) {
      disattiva_finecorsa(X);
      disattiva_finecorsa(Y);
    }

    
    if (set_M_ == PAUSA) {
        delay(1000);
        set_M_ = START;
    }

    if (richiesta_cambio_utensile_ == 1) {
      solleva_penna();
      while (controlla_pulsante(PULSANTE_CAMBIO_UTENSILE) == 0) {}
      abbassa_penna();
      richiesta_cambio_utensile_ = 0;
    }

    invia_feedback();

    #ifdef DEBUG
    Serial.println();
      Serial.print("coordinata_attuale_X_ ");
      Serial.println(coordinata_attuale_X_);
      Serial.print("coordinata_attuale_Y_ ");
      Serial.println(coordinata_attuale_Y_);
            Serial.print("coordinata_attuale_Z_ ");
      Serial.println(coordinata_attuale_Z_);

      Serial.print("coordinata_precedente_X_ ");
      Serial.println(coordinata_precedente_X_);
      Serial.print("coordinata_precedente_Y_ ");
      Serial.println(coordinata_precedente_Y_);
    #endif

}

bool CNC::ingresso_comando()
{
  if (Serial.available() > 0)
    return 1;
  else
    return 0;
}


void CNC::leggi_set() 
{
  int posG = ric_string_.indexOf("G");
  int posM = ric_string_.indexOf("M");
  int posT = ric_string_.indexOf("T");

  if (posG != -1) { //G28 X50
    int settaggio_G = ric_string_.substring(posG + 1, posG + 3).toInt();

    if (settaggio_G == INCREMENTALI) {
      if (set_coordinate_ != INCREMENTALI)
        cambio_set_coordinate_ = 1;

      set_coordinate_ = INCREMENTALI;
    }
    if (settaggio_G == ASSOLUTE) {
      if (set_coordinate_ != ASSOLUTE)
        cambio_set_coordinate_ = 1;

      set_coordinate_ = ASSOLUTE;
    }

    if (settaggio_G == HOMING) {
      set_tipo_movimento_ = HOMING;
    }
    if (settaggio_G == RAPIDO) {
      set_tipo_movimento_ = RAPIDO;
    }
    if (settaggio_G == RETTA) {
      set_tipo_movimento_ = RETTA;
    }
    if (settaggio_G == ORARIO) {
      set_tipo_movimento_ = ORARIO;
    }
    if (settaggio_G == ANTIORARIO) {
      set_tipo_movimento_ = ANTIORARIO;
    }
    if (settaggio_G == SETPOSIZIONE) {
      set_tipo_movimento_ = SETPOSIZIONE;
    }
    ric_string_.remove(posG, posG + 3); //aggiungere un carattere

  }

  if (posM != -1) {
    int settaggio_M = ric_string_.substring(posM + 1, posM + 3).toInt();

    if (settaggio_M == FINECORSA_OFF) {
      set_M_ = FINECORSA_OFF;
    }
    if (settaggio_M == FINECORSA_ON) {
      set_M_ = FINECORSA_ON;
    }
    if (settaggio_M == PAUSA) {
      set_M_ = PAUSA;
    }
    if (settaggio_M == START) { //M602 atteso
      ric_string_.remove(posM + 3);
      set_M_ = START;
    }


    ric_string_.remove(posM, posM + 3);

  }

  if (posT != -1) {
    int settaggio_T = ric_string_.substring(posT + 1, posT + 3).toInt();

    if (settaggio_T == UTENSILE0) {
      if (set_T_ != UTENSILE0)
        richiesta_cambio_utensile_ = 1;

      set_T_ = UTENSILE0;
    }
    if (settaggio_T == UTENSILE1) {
      if (set_T_ != UTENSILE1)
        richiesta_cambio_utensile_ = 1;

      set_T_ = UTENSILE1;
    }


    ric_string_.remove(posT, posT + 3);

  }
}

void CNC::leggi_movimento()
{

  int posY = ric_string_.indexOf("Y");
  int posX = ric_string_.indexOf("X");
  int posZ = ric_string_.indexOf("Z");

  int posI = ric_string_.indexOf("I");
  int posJ = ric_string_.indexOf("J");

  if (set_tipo_movimento_ == ORARIO || set_tipo_movimento_ == ANTIORARIO) {
    if ((posI == -1) && (posJ == -1)) { //no coordinate e Gincremntali inserite. Non muovi assi.
      set_cerchio_I_ = 0;
      set_cerchio_J_ = 0;
    }
    if ((posI != -1) && (posJ == -1)) {
      set_cerchio_I_ = ric_string_.substring(posI + 1).toFloat();
      set_cerchio_J_ = 0;
      ric_string_.remove(posI);
    }
    if ((posJ != -1) && (posI == -1)) {
      set_cerchio_J_ = ric_string_.substring(posJ + 1).toFloat();
      set_cerchio_I_ = 0;
      ric_string_.remove(posJ);
    }
    if ((posJ != -1) && (posI != -1)) {
      set_cerchio_I_ = ric_string_.substring(posI + 1, posJ).toFloat();
      set_cerchio_J_ = ric_string_.substring(posJ + 1).toFloat();
      ric_string_.remove(posI);
    }
  }



  if (posX != -1 && posY == -1) { //stringa del tipo Xnumero, comanda di muovere solo X di numero
    ric_string_.remove(posX, 1);
    coordinate_X_ = ric_string_.toFloat();
    cambia_X_ = 1;

  }

  if (posY != -1 && posX == -1) { //stringa del tipo Ynumero, comanda di muovere solo Y di numero
    ric_string_.remove(posY, 1);
    coordinate_Y_ = ric_string_.toFloat();
    cambia_Y_ = 1;

  }

  if ((posY != -1) && (posX != -1)) { //stringa del tipo XnumeroYnumero, comanda di muovere solo Y di numero
    coordinate_X_ = ric_string_.substring(posX + 1, posY - 1).toFloat();
    coordinate_Y_ = ric_string_.substring(posY + 1).toFloat();
    cambia_X_ = 1;
    cambia_Y_ = 1;

  }

  if ((posZ != -1) ) { //è indicata la Z
    cambia_Z_ = 1;
    
    String numZ = "";
    for(int i = 1; isDigit(ric_string_[posZ+i]) == true || ric_string_[posZ+i] == '-' || ric_string_[posZ+i] == '+' || ric_string_[posZ+i] == '.'; i++) {
      numZ += ric_string_[posZ+i];
    }
    coordinate_Z_ = numZ.toFloat();
      
  }


}


void CNC::calcola_passi()
{
  if (set_tipo_movimento_ == RETTA || set_tipo_movimento_ == RAPIDO || set_tipo_movimento_ == ORARIO || set_tipo_movimento_ == ANTIORARIO) { //uniche possibilità di movimento
  if (set_coordinate_ == ASSOLUTE) {

    if (cambia_X_ == 1) {
      if (coordinate_X_ == coordinata_attuale_X_ || coordinate_X_ < 0)
        passi_X_ = 0;
      else
        passi_X_ = round((coordinate_X_ - coordinata_attuale_X_) / rap_spostamento_passo_[X]);
    }

    if (cambia_Y_ == 1) {
      if (coordinate_Y_ == coordinata_attuale_Y_ || coordinate_Y_ < 0)
        passi_Y_ = 0;
      else
        passi_Y_ = round((coordinate_Y_ - coordinata_attuale_Y_) / rap_spostamento_passo_[Y]);

    }
  }


  if (set_coordinate_ == INCREMENTALI) {
    
    if (cambia_X_ == 1) {
      if (coordinate_X_ == 0)
        passi_X_ = 0;
      else
        passi_X_ = round(coordinate_X_ / rap_spostamento_passo_[X]);
    }
    
    if (cambia_Y_ == 1) {
      if (coordinate_Y_ == 0)
        passi_Y_ = 0;
      else
        passi_Y_ = round(coordinate_Y_ / rap_spostamento_passo_[Y]);

    }
  }
  } else  {
    passi_X_ = 0;
    passi_Y_ = 0;
  }

}

void CNC::leggi_comando()
{

  passi_X_ = 0;
  passi_Y_ = 0;

  coordinate_X_ = 0;
  coordinate_Y_ = 0;
  coordinate_Z_ = 0;

  cambia_X_ = 0;
  cambia_Y_ = 0;
  cambia_Z_ = 0;

  ric_string_ = Serial.readStringUntil(';');

  //Serial.readStringUntil('\n'); //per ripulire il buffer


  leggi_set();

  leggi_movimento();

  calcola_passi();

  aggiorna_coordinata_attuale();

}

void CNC::aggiorna_coordinata_attuale()
{
  
  coordinata_precedente_X_ = coordinata_attuale_X_;
  coordinata_precedente_Y_ = coordinata_attuale_Y_;
  
  if (set_tipo_movimento_ != SETPOSIZIONE) {
    
  if (passi_X_ != 0)
    coordinata_attuale_X_ =  coordinata_attuale_X_ + (passi_X_ * rap_spostamento_passo_[X]);

  if (passi_Y_ != 0)
    coordinata_attuale_Y_ =  coordinata_attuale_Y_ + (passi_Y_ * rap_spostamento_passo_[Y]);
  
  } else {
    if (cambia_X_ == 1)
      coordinata_attuale_X_ = coordinate_X_;
      
    if (cambia_Y_ == 1)
      coordinata_attuale_Y_ = coordinate_Y_;


  }

  if (set_tipo_movimento_ == HOMING) {
    if (cambia_X_ == 1)
      coordinata_attuale_X_ = 0;

    if (cambia_Y_ == 1)
      coordinata_attuale_Y_ = 0;
     
  }

  if (cambia_Z_ == 1)
      coordinata_attuale_Z_ = coordinate_Z_;

}

void CNC::invia_feedback()
{
  if (controlla_finecorsa(X) == 0 && controlla_finecorsa(Y) == 0) {
    Serial.print("y");
  }
  else {
    Serial.print("n");
  }
//  Serial.print("X");
//  Serial.print(coordinata_attuale_X_, 2);
//  Serial.print(" ");
//  Serial.print("Y");
//  Serial.print(coordinata_attuale_Y_, 2);
//  Serial.print(";");
}
