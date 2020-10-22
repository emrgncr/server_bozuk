#pragma once
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <ctime>
#include <Windows.h>

class c_field {
public:
	int f = 0;
	int kapali = 0;
	int tributeli = 0;
	int joker = 0;
	int saldir = 0;
	void f_clr() {
		 f = 0;
		 kapali = 0;
		 tributeli = 0;
		 joker = 0;
		 saldir = 0;
	}
};

class c_player {
public : 

	void mezar_duzenle() {
		for (int p = 0; p < 52; p++) {
			if (mezar[51 - p] == 0) {
				int q = 51;
				while (mezar[q] == 0 && q > -1) { q--; }
				if (51 - p < q && q >= 0) {
					mezar[51 - p] = mezar[q];
					mezar[q] = 0;
		}
		}
		}
	}
	int get_mezarsize() {
		int mezs = 0;
		for (int i = 0; i < 52; i++) {
			if (mezar[i] != 0) mezs++;
		}
		return mezs;
	}
	void efektcheck(){
		for (int i = 0; i < 10; i++){
			if (efektsira[i] == 0) {
				int f = i + 1;
				while (f<10) {
					if (efektsira[f] != 0) {
						efektsira[i] = efektsira[f];
						efektonay[i] = efektonay[f];
						efektonay[f] = 0;
						efektsira[f] = 0;
						break;
					}
					f++;
				}
		}
		}
	}
	bool efektoynatcheck() {
		while (efektsira[efektoynat] == 0) {
			if (efektoynat == 0) break;
			efektoynat--;
		}
		if (efektoynat == 0 && efektsira[efektoynat] == 0) return true;
		return false;
	}
	void efektset(int setlenecek) {
		for (int i = 0; i < 10; i++) {
			if (efektsira[i] == 0) {
				efektsira[i] = setlenecek;
				efektonay[i] = setlenecek;
				if (efektoynat < i) efektoynat = i;
				return;
			}
		}
	}
	int set_valemill_kart(int set,bool setle = false){
		if (setle) valemill_kart = set;
		return valemill_kart;
	}
	int set_efektoynat(int set = 0, bool setle = false) {
		if (setle) efektoynat = set;
		return efektoynat;
	}
	int set_battlesec(int set, bool setle = false) {
		if (setle) battlesec = set;
		return battlesec;
	}
	int set_battlesecrakip(int set, bool setle = false) {
		if (setle) battlesecrakip = set;
		return battlesecrakip;
	}
	bool set_askral(bool set, bool setle = false) {
		if (setle) askral = set;
		return askral;
	}
	bool set_beskullan(bool set, bool setle = false) {
		if (setle) beskullan = set;
		return beskullan;
	}
	bool set_sekizkullan(bool set, bool setle = false) {
		if (setle) sekizkullan = set;
		return sekizkullan;
	}
	bool set_dokuzefekt(bool set, bool setle = false) {
		if (setle) dokuzefekt = set;
		return dokuzefekt;
	}
	bool set_dortefekt(bool set, bool setle = false) {
		if (setle) dortefekt = set;
		return dortefekt;
	}
	bool set_yediefekt(bool set, bool setle = false) {
		if (setle) yediefekt = set;
		return yediefekt;
	}
	bool set_altiefekt(bool set, bool setle = false) {
		if (setle) altiefekt = set;
		return altiefekt;
	}
	int set_yedisec(int set, bool setle = false) {
		if (setle) yedisec = set;
		return yedisec;
	}
	int set_altisec(int set, bool setle = false) {
		if (setle) altisec = set;
		return altisec;
	}
	int set_birakt(int set, bool setle = false) {
		if (setle) birakt = set;
		return birakt;
	}
	int set_uckullan(int set, bool setle = false) {
		if (setle) uckullan = set;
		return uckullan;
	}
	int set_efekt_kullan(int set, bool setle = false) {
		if (setle) efekt_kullan = set;
		return efekt_kullan;
	}
	int set_field_secili(int set, bool setle = false) {
		if (setle == false) {
			return field_secili;
		}
		if (setle == true) { field_secili = set; }
		return field_secili;
	}
	int set_can(int set, bool setle = false) {
		if (setle) can = set;
		return can;
	}
	int set_dokuzsummon(int set, bool setle = false) {
		if (setle) dokuzsummon = set;
		return dokuzsummon;
	}
	int set_mezar(int set,int i, bool setle = false) {
		if (setle) mezar[i] = set;
		return mezar[i];
	}
	c_field get_field(int i) {
		return field[i];
	}
	void set_field_f(int i, int set) {
		field[i].f = set;
	}
	void set_field_kapali(int i, int set) {
		field[i].kapali = set;
	}
	void set_field_joker(int i, int set) {
		field[i].joker = set;
	}
	void set_field_saldir(int i, int set) {
		field[i].saldir= set;
	}
	void set_field_tributeli(int i, int set) {
		field[i].tributeli = set;

	}
	int set_efektsira(int set, int i, bool setle = false) {
		if (setle) efektsira[i] = set;
		return efektsira[i];
	}
	int set_efektonay(int set, int i, bool setle = false) {
		if (setle) efektonay[i] = set;
		return efektonay[i];
	}
	int set_oyna(int set, int i, bool setle = false) {
		if (setle) oyna[i] = set;
		return oyna[i];
	}
	int set_el(int set, int i, bool setle = false) {
		if (setle) el[i] = set;
		return el[i];
	}
	int set_oyunturu(int set = 0, bool setle = false) {
		int sem;
		if (setle == true) { oyunturu = set; 
		}
		else {
		}
		sem = oyunturu;
		return sem;
	}
	int set_mezarsecili(int set = 0, bool setle = false) {
		if (setle == true) {
			mezarsecili = set;
		}
		return mezarsecili;
	}
	void w_oyunturu(int set = 0) {
		oyunturu = set;
	}
	int r_oyunturu() {
		return oyunturu;
	}

	void f_clr() {
		 valemill_kart = 0;
		 efektoynat = 0;
		 battlesec = 0;
		 battlesecrakip = 0;
		 askral = false;
		 beskullan = false;
		 sekizkullan = false;
		 dokuzefekt = false;
		 dortefekt = false;
		 yediefekt = false;
		 altiefekt = false;
		 yedisec = 0;
		 altisec = 0;
		 birakt = 0;
		 uckullan = 0;
		 efekt_kullan = 0;
		 field_secili = 0;
		 can = 3;
		 dokuzsummon = -1;
		 //ZeroMemory(mezar, 52);
		 for (int i = 0; i < 52; i++){
			 mezar[i] = 0;
			 if (i < 10) {
				 efektsira[i] = 0;
				 efektonay[i] = 0;
				 oyna[i] = 0;
				 el[i] = 0;
			 }
		 }
		 field[0].f_clr();
		 field[1].f_clr();
		 field[2].f_clr();
		 //ZeroMemory(efektsira, 10);
		 //ZeroMemory(efektonay, 10);
		 //ZeroMemory(oyna, 10);
		// ZeroMemory(el, 10);
		 oyunturu = 0;
		 mezarsecili = -1;
	}
	void s_onayg(bool b) {
		onayg = b;
	}
	bool g_onayg() {
		return onayg;
	}

	protected:
		int mezarsecili = -1;
		bool onayg = false;
		int oyunturu = 0;
		int valemill_kart = 0;
		int efektoynat = 0;
		int battlesec = 0;
		int battlesecrakip = 0;
		bool askral = false;
		bool beskullan = false;
		bool sekizkullan = false;
		bool dokuzefekt = false;
		bool dortefekt = false;
		bool yediefekt = false;
		bool altiefekt = false;
		int yedisec = 0;
		int altisec = 0;
		int birakt = 0;
		int uckullan = 0;
		int efekt_kullan = 0;
		int field_secili = 0;
		int can = 3;
		int dokuzsummon = -1;
		int mezar[52] = {};
		c_field field[3];
		int efektsira[10] = {};
		int efektonay[10] = {};
		int oyna[10] = {};
		int el[10] = {};
};

class c_deste {
protected:
	int deck[52] = {};
public:
	bool kar1 = false;
	bool kar2 = false;
	void deck_setup(bool debug = false) {
		kar1 = false;
		kar2 = false;
		for (int i = 0; i < 52; i++) {
			deck[i] = i % 13;
			deck[i] ++;
		}
		if (debug) {
			for (int shvar = 0; shvar < 52; shvar++) {
				//std::cout << deck[shvar] << std::endl;
			}
		}
	}
	void shuffle(bool debug = false) {
		auto sa = std::chrono::system_clock::now();
		std::time_t saakso = std::chrono::system_clock::to_time_t(sa);
		srand(saakso);
		//std::default_random_engine generator;
		//std::uniform_int_distribution<int> distribution(0, 51);
		for (int shvar = 0; shvar < 52; shvar++) {
			int sget = rand()%52;

			int sea = deck[shvar];
			deck[shvar] = deck[sget];
			deck[sget] = sea;
		}
		for (int shvar = 51; shvar > 0; shvar--) {
			if (deck[shvar] == 0) {
				int shvar1 = 1;
				while (deck[shvar - shvar1] == 0&&shvar-shvar1 >=1) shvar1++;
				deck[shvar] = deck[shvar - shvar1];
				deck[shvar - shvar1] = 0;
				//if (debug) std::cout << "DEGISEN " << shvar << " " << shvar - shvar1<<std::endl;
			}

		}
		if (debug) {
			for (int shvar = 0; shvar < 52; shvar++) {
				//std::cout << deck[shvar] << std::endl;
			}
		}
		kar1 = true;
		kar2 = true;
	}
	int deck_set(int sec,int set) {
		deck[sec] = set;
		return deck[sec];
	}
	int deck_get(int sec) {
		return deck[sec];
	}
};

class c_tur_kontrol {
protected:
	int oybit = 0;
	int tur = 0;
	int tur_durum = 0;
	int oyun_dur = 0;
	int saldirkey = 0;
public:
	int r_oyit() {
		return oybit;
	}
	void s_oybit(int i) {
		oybit = i;
	}
	void f_clr() {
		oybit = 0;
		tur = 0;
		tur_durum = 0;
		oyun_dur = 0;
		saldirkey = 0;
	}
	int get_tur() {
		return tur;
	}
	int get_tur_durum(bool dene = true) {
		if (dene) tur_durum = tur % 9;
		return tur_durum;
	}
	void tur_arttir(int add, bool dene = true) {
		tur += add;
		if(dene) tur_durum = tur % 9;
	}
	int get_oyun_dur(int set = 0, bool setle=false) {
		if (setle == true) { oyun_dur = set; }
		return oyun_dur;
	}
	int get_saldirkey(int set, bool setle = false) {
		if (setle == true) {
		saldirkey = set; 
		if (set == 0) {
			//std::cout << "allah allah";
		}
		}
		return saldirkey;
	}

};


class c_efektlist {
public:
	int efekt = 0;
	int p1d1 = 0;
	int p1d2 = 0;
	int p1d3 = 0;
	int p1d4 = 0;
	int p2d1 = 0;
	int p2d2 = 0;
	int p2d3 = 0;
	int p2d4 = 0;
	
	void clr() {
		 efekt = 0;
		 p1d1 = 0;
		 p1d2 = 0;
		 p1d3 = 0;
		 p1d4 = 0;
		 p2d1 = 0;
		 p2d2 = 0;
		 p2d3 = 0;
		 p2d4 = 0;
	}

	void checkclr() {
		if (efekt == 0) {
			p1d1 = 0;
			p1d2 = 0;
			p1d3 = 0;
			p1d4 = 0;
			p2d1 = 0;
			p2d2 = 0;
			p2d3 = 0;
			p2d4 = 0;
		}
	}
};



class c_serverkontrol {
public:
	int karla = 0;
	int efekciz = 0;
	int p1_mousex = 0;
	int p1_mousey = 0;
	int p1_mouserel = 0;
	int p1_sayfa = 0;

	int p2_mousex = 0;
	int p2_mousey = 0;
	int p2_mouserel = 0;
	int p2_sayfa = 0;

	bool oyunbit = false;
	int oyuntekrarp1 = 0;
	int oyuntekrarp2 = 0;
	int shufl = 0;

	int p1_elclick[10] = {};
	int p2_elclick[10] = {};

	bool p1_fclick[3] = {false,false,false};
	bool p1_rfclick[3] = { false,false,false };

	void set_oyuntekrarp1(int set = 0) {
		oyuntekrarp1 = set;
	}
	void set_oyuntekrarp2(int set = 0) {
		oyuntekrarp2 = set;
	}

	bool p2_fclick[3] = { false,false,false };
	bool p2_rfclick[3] = { false,false,false };

	void f_clr() {
		karla = 0;
		efekciz = 0;
		 p1_mousex = 0;
		 p1_mousey = 0;
		 p1_mouserel = 0;
		 p1_sayfa = 0;

		 p2_mousex = 0;
		 p2_mousey = 0;
		 p2_mouserel = 0;
		 p2_sayfa = 0;

		 oyunbit = false;
		 oyuntekrarp1 = 0;
		 oyuntekrarp2 = 0;
		 shufl = 0;
		 for (int i = 0; i < 10; i++) {
			 p1_elclick[i] = 0;
			 p2_elclick[i] = 0;
			 if (i < 3) {
				 p1_fclick[i] = 0;
				 p1_rfclick[i] = 0;

				 p2_fclick[i] = 0;
				 p2_rfclick[i] = 0;
			 }
		 }
	}
};



