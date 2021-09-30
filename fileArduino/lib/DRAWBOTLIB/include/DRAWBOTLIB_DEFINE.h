#ifndef DRAWBOTLIB_DEFINE_h
#define DRAWBOTLIB_DEFINE_h

//#define DEBUG 

#define Pi 3.1415

//#define FULLSTEP
#define MICRO16STEP

#ifdef FULLSTEP
#define VELOCITA 700
#define RAPPORTO 1
#endif
#ifdef MICRO16STEP
#define VELOCITA 20
#define RAPPORTO 16
#endif




#define MAX_MOTORI_STEPPER 2
#define MAX_BOTTONI 10

#define SERVO_MOTORE 1

#define X 0
#define Y 1
#define Z 3

#define PULSANTE_CAMBIO_UTENSILE 0

#define INCREMENTALI 91
#define ASSOLUTE 90
#define SETPOSIZIONE 92
#define RAPIDO 0
#define RETTA 1
#define ORARIO 2
#define ANTIORARIO 3
#define HOMING 28

#define START 60 //sostituisce 602
#define PAUSA 0
#define FINECORSA_OFF 42
#define FINECORSA_ON 43

#define UTENSILE1 1
#define UTENSILE0 0

#endif
