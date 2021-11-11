#include "DRAWBOTLIB.h"  

using namespace std;
using namespace CNC;

int main(){
 
 DrawBot CNC_LORLO;
 
 CNC_LORLO.leggi_comandi();
 CNC_LORLO.esegui_comandi();

}
