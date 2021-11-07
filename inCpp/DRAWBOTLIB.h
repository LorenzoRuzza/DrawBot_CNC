#ifndef DRAWBOTLIB_h
#define DRAWBOTLIB_h

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

namespace CNC
{
	class Motore_passo 
	{
		public:
			Motore_passo() {};
			void configura_motore(byte pin_step, byte pin_dir, byte pin_en, double spostamento_passo);
			bool muovi(int32_t passi);
			void spegni();
			void accendi();
			
			void stampa_pin();
			void stampa_rap();

			
		private:
			byte pin_step_;
			byte pin_dir_;
			byte pin_en_;
			double spostamento_passo_;
	};
	
	
	class DrawBot
	{
	
		public:
			DrawBot();
			
			bool leggi_comandi();
			
			enum Gset : int {
			incrementali = 91,
			assolute = 90,
			setposizione = 92,
			
			rapido = 0,
			retta = 1,
			orario = 2,
			antiorario = 3,
			homing = 28
			};
			
			enum Mset : int {
			start = 602,
			pausa = 0
			};
			
			
					
		
		private:
			bool leggi_configurazione(int n_valori);
			
			int setGmov_;
			int setGcord_;
			int setT_;
			int setM_;
			int setF_;
		
			//int32_t passiX_;
			//int32_t passiY_;
			//bool passoZ_;
			
			bool finecorsa_min_X_;
			bool finecorsa_max_X_;
			bool finecorsa_min_Y_;
			bool finecorsa_max_Y_;
			
			int32_t posX_;
			int32_t posY_;
			bool posZ_;
			
			Motore_passo motoreX;
			Motore_passo motoreY;			
	};

}

#endif
