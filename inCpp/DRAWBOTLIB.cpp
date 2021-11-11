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
	
	conf_partenza_.imposta_configurazione(incrementali, 0, 0, 0, 0, 0, 0, true, true, true); //impostabili solo coordinate iniziali e tipo coordinate

	indice_eseg_ = 0;
	
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
				rap_list.push_back( ( stod(valore) ) );
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
	string linea{""};
	string comando{""};
	int stop_pos = 0;
	
	vector<configurazione> conf_elaborate;
	vector<coordinate_assolute> lista_coordinate;
	
	fstream commandFile("comandi.txt"); //da rimpiazzare con variabile globale e scrivere programma che preleva i comandi che vengono scritti sulla variabile globale
	while (getline (commandFile, linea)) {
		stop_pos = linea.find(';');
		comando = linea.substr(0, stop_pos);
		if (!comando.empty()) {
			conf_elaborate.push_back(interpreta_comando(comando));
		}

	}
	
	vettore_da_configurazione_a_coordinate(conf_partenza_, conf_elaborate, lista_coordinate);
	//conf_elaborate_.erase(conf_elaborate_.begin(), conf_elaborate_.end());
	
	vettore_da_coordinate_a_ordine(conf_partenza_, lista_coordinate, lista_ordini_);
	//lista_coordinate_.erase(lista_coordinate_.begin(), lista_coordinate_.end());
	
	/*
	for (int i = 0; i < static_cast<int>(lista_ordini_.size()); i++) {
		lista_ordini_.at(i).stampa_ordine();
	}
	*/
	
	ofstream com_file;
  	com_file.open ("com_file.txt", std::ofstream::out | std::ofstream::trunc);
  	for (int i=0; i < static_cast<int>(lista_coordinate.size()); i++)
  		com_file <<"X"<<lista_coordinate.at(i).get_X()<<"  Y"<<lista_coordinate.at(i).get_Y()<<"  Z"<<lista_coordinate.at(i).get_Z()<<"  V"<<lista_coordinate.at(i).get_vel()<<"\n";
  	com_file.close();
  	

  	
	return true;
}

float DrawBot::numero_sottostringa(const string& str, int posLettera)
{
	string str_numero{""};
	for (int i = posLettera+1; isdigit(str.at(i)) || str.at(i) == '.' || str.at(i) == '-'; i++) {
		str_numero.push_back(str.at(i));
		if (i==static_cast<int>(str.size()))
			break;
	}
	//cout<<str_numero<<endl;
	return (stod(str_numero));
}

configurazione DrawBot::interpreta_comando(string cmd)
{
	if (cmd.empty())
		exit(1);
		
	configurazione cfg;
	
	int posG = static_cast<int>(cmd.find('G'));
	int posM = static_cast<int>(cmd.find('M'));
	int posF = static_cast<int>(cmd.find('F'));
	int posT = static_cast<int>(cmd.find('T'));
	
	int posX = static_cast<int>(cmd.find('X'));
	int posY = static_cast<int>(cmd.find('Y'));
	int posZ = static_cast<int>(cmd.find('Z'));
	
	int G = -1;
	int M = -1;
	int F = -1;
	int T = -1;
	
	double X, Y;
	bool Z;
	
	bool noX{false};
	bool noY{false};
	bool noZ{false};
	
	if (posG != static_cast<int>(string::npos)) 
		G = static_cast<int>(numero_sottostringa(cmd, posG));
		
	if (posM != static_cast<int>(string::npos)) 
		M = static_cast<int>(numero_sottostringa(cmd, posM));
		
	if (posF != static_cast<int>(string::npos)) 
		F = static_cast<int>(numero_sottostringa(cmd, posF));
		
	if (posT != static_cast<int>(string::npos)) 
		T = static_cast<int>(numero_sottostringa(cmd, posT));
		
	if (posX != static_cast<int>(string::npos)) 
		X = numero_sottostringa(cmd, posX);
	else 
		noX = true;
	
	if (posY != static_cast<int>(string::npos)) 
		Y = numero_sottostringa(cmd, posY);
	else 
		noY = true;
	
	if (posZ != static_cast<int>(string::npos)) 
		if (numero_sottostringa(cmd, posZ) <= 0)
			Z = 1;
		else
			Z = 0;
	else 
		noZ = true;
	
	cfg.imposta_configurazione(G, M, F, T, X, Y, Z, noX, noY, noZ);
	
	return cfg;
}

void DrawBot::vettore_da_configurazione_a_coordinate(configurazione& conf_partenza, vector<configurazione>& conf_elaborate, vector<coordinate_assolute>& lista_coordinate)
{
	int G_cord = incrementali;
	int G_mov = assolute;
	int special = -1;
	int vel = conf_partenza.get_setF();
	
	double X = conf_partenza.get_posX();
	double Y = conf_partenza.get_posY();
	bool Z = conf_partenza.get_posZ();
		
	if (conf_partenza.get_setG() == incrementali || conf_partenza.get_setG() == assolute)
		G_cord = conf_partenza.get_setG();
	if (conf_partenza.get_setG() == retta || conf_partenza.get_setG() == orario || conf_partenza.get_setG() == antiorario)
		G_mov = conf_partenza.get_setG();
	if (conf_partenza.get_setG() == homing)
		special = homing;
		
	
	for (int i = 0; i < static_cast<int>(conf_elaborate.size()); i++) {
		if (conf_elaborate.at(i).get_setG() == incrementali || conf_elaborate.at(i).get_setG() == assolute)
			G_cord = conf_elaborate.at(i).get_setG();
		if (conf_elaborate.at(i).get_setG() == retta || conf_elaborate.at(i).get_setG() == orario || conf_elaborate.at(i).get_setG() == antiorario)
			G_mov = conf_elaborate.at(i).get_setG();
		if (conf_elaborate.at(i).get_setG() == homing)
			special = homing;
		else 
			special = -1;
		if (conf_elaborate.at(i).get_setF() != -1) {
			vel = conf_elaborate.at(i).get_setF();
		}
		if (G_mov == rapido) 
			vel = max_vel_;
		
		if (G_cord == assolute) {
			if (!conf_elaborate.at(i).get_noX())
				X = conf_elaborate.at(i).get_posX();
			if (!conf_elaborate.at(i).get_noY())
				Y = conf_elaborate.at(i).get_posY();
			if (!conf_elaborate.at(i).get_noZ())
				Z = conf_elaborate.at(i).get_posZ();
			
		} else if (G_cord == incrementali) {
			if (!conf_elaborate.at(i).get_noX())
				X = X + conf_elaborate.at(i).get_posX();
			if (!conf_elaborate.at(i).get_noY())
				Y = Y + conf_elaborate.at(i).get_posY();
			if (!conf_elaborate.at(i).get_noZ())
				Z = conf_elaborate.at(i).get_posZ();
		}
		
			
		coordinate_assolute p;	
		lista_coordinate.push_back(p);
		lista_coordinate.at(i).imposta_coordinate_assolute(X, Y, Z, vel, special);
	}
	
}

void DrawBot::vettore_da_coordinate_a_ordine(configurazione& conf_partenza, vector<coordinate_assolute>& lista_coordinate, vector<ordine>& lista_ordini)
{
	double X_pre = conf_partenza.get_posX();
	double Y_pre = conf_partenza.get_posY();
	
	double X;
	double Y;
	bool passoZ;
	
	double rap_X = motoreX.get_spostamento_passo();
	double rap_Y = motoreY.get_spostamento_passo();
	
	int32_t passiX = 0;
	int32_t passiY = 0;
	int vel = F_to_udelay(conf_partenza.get_setF(), motoreX.get_spostamento_passo());
	int speciale = -1;
	
	ordine p;
	
	for (int i = 0; i < static_cast<int>(lista_coordinate.size()); i++) {
		X = lista_coordinate.at(i).get_X();
		Y = lista_coordinate.at(i).get_Y();
		passoZ = lista_coordinate.at(i).get_Z();
		passiX = static_cast<int32_t>((X - X_pre)/rap_X);
		passiY = static_cast<int32_t>((Y - Y_pre)/rap_Y);
		X_pre = X;
		Y_pre = Y;
		vel = F_to_udelay(lista_coordinate.at(i).get_vel(), motoreX.get_spostamento_passo());
		speciale = lista_coordinate.at(i).get_special();
		lista_ordini.push_back(p);
		lista_ordini.at(i).imposta_ordine(passiX, passiY, passoZ, vel, speciale);
	}
	
}

int DrawBot::F_to_udelay(int F, double spostamento_passo) //F in mm/min
{	//mm/min * passo/mm = passo/min
	return ((F / spostamento_passo) / (60000000)) / 2;
}

bool DrawBot::esegui_comandi() // da aggiungere homing
{	
	int32_t passiX;
	int32_t passiY;
	int32_t rap = 0;
	double res = 0;
	int32_t comp = 0;
	int32_t cont_comp = 0;
	
	int udelay_vel;
	
	for (int i = 0; i < static_cast<int>(lista_ordini_.size()); i++) {
		comp = 0;
		cont_comp = 0;
		passiX = lista_ordini_.at(indice_eseg_).get_passiX();
		passiY = lista_ordini_.at(indice_eseg_).get_passiY();
		udelay_vel = lista_ordini_.at(indice_eseg_).get_vel();
		
		if (passiX == 0 || passiY == 0) {
			motoreY.muovi(passiY, udelay_vel);
			motoreX.muovi(passiX, udelay_vel);
		
		} else if (passiX >= passiY) {
			rap = passiX / passiY;
			res = (static_cast<double>(passiX) / static_cast<double>(passiY)) - static_cast<double>(rap);
			for (int32_t i = 0; i < passiY; i++) {
				motoreX.muovi(rap + comp, udelay_vel);
				motoreY.muovi(1, udelay_vel);
				if (cont_comp >= 1) {
					comp = 1;
					cont_comp = cont_comp - 1;
				} else {
					comp = 0;
					cont_comp = cont_comp + res;
				}
			}
		} else {
			rap = passiY / passiX;
			res = (static_cast<double>(passiY) / static_cast<double>(passiX)) - static_cast<double>(rap);
			for (int32_t i = 0; i < passiX; i++) {
				motoreY.muovi(rap + comp, udelay_vel);
				motoreX.muovi(1, udelay_vel);
				if (cont_comp >= 1) {
					comp = 1;
					cont_comp = cont_comp - 1;
				} else {
					comp = 0;
					cont_comp = cont_comp + res;
				}
			}
	}
		
	
}
}


void Motore_passo::stampa_pin()
{
		cout<<"pin step = "<< static_cast<int>(pin_step_)<<endl;
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
