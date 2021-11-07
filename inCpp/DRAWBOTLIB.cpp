#include "DRAWBOTLIB.h"

using namespace CNC;
using namespace std;


DrawBot::DrawBot()
{
	if (leggi_configurazione(8)) {
		cout<<"configurazione completata" << endl;
		cout<<"MOTORE X" << endl;
		motoreX.stampa_pin();
		motoreX.stampa_rap();
		cout<<"MOTRORE Y" << endl;
		motoreY.stampa_pin();
		motoreY.stampa_rap();
	}
	
	setGmov_ = retta;
	setGcord_ = incrementali;
	setT_ = 0;
	setM_ = 0;
	setF_ = 0;

	posX_ = 0;
	posY_ = 0;
	posZ_ = 0;
}


bool DrawBot::leggi_configurazione(int n_valori)
{
	string configurazione{""};
	string valore{""};
	vector<byte> pin_num_list;
	vector<double> rap_list;
	
	int start_pos;
	
	fstream configFile("config.txt");
	while (getline (configFile, configurazione)) {
		if (!configurazione.empty()) {
			if (configurazione.at(0) == '$') {
				start_pos = configurazione.find("=") + 1;
				valore = configurazione.substr(start_pos);
				pin_num_list.push_back( static_cast<byte>( stoi(valore) ) );
				
			} else if (configurazione.at(0) == '&') {
				start_pos = configurazione.find("=") + 1;
				valore = configurazione.substr(start_pos);
				rap_list.push_back( static_cast<double>( stod(valore) ) );
			}
		}
	}
	
	configFile.close();
	
	if ((static_cast<int>(pin_num_list.size()) + static_cast<int>(rap_list.size())) != n_valori)
		return false;
	
	motoreX.configura_motore(pin_num_list.at(0), pin_num_list.at(1), pin_num_list.at(2), rap_list.at(0));
	motoreY.configura_motore(pin_num_list.at(3), pin_num_list.at(4), pin_num_list.at(5), rap_list.at(1));
	
	
	return true;
}

bool DrawBot::leggi_comandi()
{
	string comando{""};
	fstream commandFile("comandi.txt"); //da rimpiazzare con variabile globale e scrivere programma che preleva i comandi che vengono scritti sulla variabile globale
	while (getline (commandFile, comando)) {
		
	}
	
}

void Motore_passo::stampa_pin()
{
		cout<<"pin step = "<<static_cast<int>(pin_step_)<<endl;
		cout<<"pin dir = "<< static_cast<int>(pin_dir_)<<endl;
		cout<<"pin en = "<< static_cast<int>(pin_en_)<<endl;
}

void Motore_passo::stampa_rap()
{
		cout<<"spostamento passo = "<<spostamento_passo_<<endl;
}

void Motore_passo::configura_motore(byte pin_step, byte pin_dir, byte pin_en, double spostamento_passo)
{
	pin_step_ = pin_step;
	pin_dir_ = pin_dir;
	pin_en_ = pin_en;
	
	spostamento_passo_ = spostamento_passo;
	
}
