#ifndef DRAWBOTLIB_h
#define DRAWBOTLIB_h

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

namespace CNC
{

	struct configurazione {
		int setG_; //-1 = non set
		int setM_;
		int setF_;
		int setT_;
		
		double posX_;
		double posY_;
		double posZ_;
		
		bool noX_; //true = non settato, false = settato
		bool noY_;
		bool noZ_;
		
		bool get_noX() {return noX_;};
		bool get_noY() {return noY_;};
		bool get_noZ() {return noZ_;};
		double get_posX() {return posX_;};
		double get_posY() {return posY_;};
		bool get_posZ() {return posZ_;};
		int get_setG() {return setG_;};
		int get_setM() {return setM_;};
		int get_setF() {return setF_;};
		int get_setT() {return setT_;};
		
		void imposta_configurazione(int setG, int setM, int setF, int setT, double posX, double posY, double posZ, bool noX, bool noY, bool noZ)
		{
			setG_ = setG;
			setM_ = setM;
			setF_ = setF;
			setT_ = setT;

			posX_ = posX;
			posY_ = posY;
			posZ_ = posZ;
			
			noX_ = noX;
			noY_ = noY;
			noZ_ = noZ;
		}
	};
	
	
	struct coordinate_assolute {
		double X_;
		double Y_;
		bool Z_;
		int vel_;
		int special_;
		
		void imposta_coordinate_assolute(double X, double Y, bool Z, int vel, int special)
		{
			X_ = X;
			Y_ = Y;
			Z_ = Z;
			vel_ = vel;
			special_ = special;
		}
		
		double get_X() {return X_;};
		double get_Y() {return Y_;};
		bool get_Z() {return Z_;};
		int get_vel() {return vel_;};
		int get_special() {return special_;};
	
	};
	
	struct ordine {
	
		int32_t passiX_;
		int32_t passiY_;
		bool passoZ_;
		int vel_;
		int comando_speciale_;
		
		void imposta_ordine(int32_t passiX, int32_t passiY, bool passoZ, int vel, int comando_speciale)
		{
			passiX_ = passiX;
			passiY_ = passiY;
			passoZ_ = passoZ;
			vel_ = vel;
			comando_speciale_ = comando_speciale;
		}
		
		int32_t get_passiX() {return passiX_;};
		int32_t get_passiY() {return passiY_;};
		bool get_passoZ() {return passoZ_;};
		int get_vel() {return vel_;};
		int get_special() {return comando_speciale_;};
	
		
		void stampa_ordine() 
		{
			cout<<"passiX "<<passiX_<<"  "<<"passiY "<<passiY_<<"  "<<"passoZ "<<passoZ_<<"  "<<"vel "<<vel_<<"  "<<"speciale "<<comando_speciale_<<endl;
		}

	};
	
	
	

	
	class Motore_passo 
	{
		public:
			Motore_passo() {};
			void configura_motore(byte pin_step, byte pin_dir, byte pin_en, double spostamento_passo);
			bool muovi(int32_t passi, int vel);
			void spegni();
			void accendi();
			
			void stampa_pin();
			void stampa_rap();
			
			double get_spostamento_passo() {return spostamento_passo_;};
			
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
			bool esegui_comandi();
			
			enum Gset : int {
			incrementali = 91,
			assolute = 90,
			
			rapido = 0,
			retta = 1,
			orario = 2,       //da fare
			antiorario = 3,   //da fare
			homing = 28
			};
			
			enum Mset : int {
			start = 602,
			pausa = 0
			};
			
			
		private:
			bool leggi_configurazione(int n_valori);
			float numero_sottostringa(const string& str, int posLettera);
			configurazione interpreta_comando(string cmd);
			void vettore_da_configurazione_a_coordinate(configurazione& conf_partenza, vector<configurazione>& conf_elaborate, vector<coordinate_assolute>& lista_coordinate);
			void vettore_da_coordinate_a_ordine(configurazione& conf_partenza, vector<coordinate_assolute>& lista_coordinate, vector<ordine>& lista_ordini);
			
			configurazione conf_partenza_;
			
			vector<ordine> lista_ordini_;
			
			int indice_eseg_;
			
			int32_t calcola_passi(double preX, double preY, double X, double Y);
			int F_to_udelay(int F, double spostamento_passo);
			
			bool finecorsa_min_X_;
			bool finecorsa_max_X_;
			bool finecorsa_min_Y_;
			bool finecorsa_max_Y_;
			
			
			Motore_passo motoreX;
			Motore_passo motoreY;	
			
			int max_vel_{2500};		
	};

}

#endif
