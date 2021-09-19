#ifndef DRAWBOTLIB_STRUCT_h
#define DRAWBOTLIB_STRUCT_h

struct Motore_stepper {
  const byte *pin_step;
  const byte *pin_dir;
  const byte *pin_en;
};

struct Finecorsa {
  const byte *pin_finecorsa_max;
  const byte *pin_finecorsa_min;
};

struct Pulsante {
  const byte *pin_pulsante;
};




#endif
