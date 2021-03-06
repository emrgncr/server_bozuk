#pragma once
#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include "oynat.h"
#include <sstream>


int okub = 0;
std::string chrtos(char* chr, int start, int size) {
	std::string s = "";
	for (int i = start; i < start + size; i++) {
		s += chr[i];
	}
	return s;
}

int myPow(int x, int p)
{
	if (p == 0) return 1;
	if (p == 1) return x;

	int tmp = myPow(x, p / 2);
	if (p % 2 == 0) return tmp * tmp;
	else return x * tmp * tmp;
}

class okunan {
public:
	int32_t gelenint = 0; //buffer_u8/u16/u32
	std::string gelenstr = "";//buffer_string
	bool gelenbool = false; //buffer_bool
};

okunan okubit(int kacinci, std::string okustr, char buf[],bool debug = false) {
	okunan ret;
	ret.gelenstr = "";
	ret.gelenbool = false;
	ret.gelenint = 0;
	int kacbit = 0;
	if (okustr == "buffer_u8") kacbit = 1;
	if (okustr == "buffer_u16") kacbit = 2;
	if (okustr == "buffer_u32") kacbit = 4;

	
	if (kacbit > 0) {
		okub += kacbit;
		int32_t k = 0;
		for (int i = (kacinci - 1) * 8; i < (kacinci + kacbit - 1) * 8; ++i) {
			//if (i % (kacbit * 8) == 0) std::cout << " ";
			int f = std::floor(i / 8);
			bool t = buf[f] & (1 << (i % 8));
			if (t) k += myPow(2, ((i - ((kacinci - 1) * 8)) % (kacbit * 8)));
			/*if ((i + 1) % kacbit == 0) {
				std::cout << k;
				k = 0;
			}*/

		}
		//k -= 5;
		if(debug) std::cout << k << " ";
		ret.gelenint = k;
		return ret;
	}
	else {
		if (okustr == "buffer_string") {
			int i = 0;
			std::string bs;
			std::stringstream bb;
			while (buf[kacinci + i - 1] != NULL && i < 100) {
				bb << buf[kacinci + i - 1];
				i++;
			}
			bb >> bs;
			if (debug) std::cout << bs << " ";
			ret.gelenstr = bs;
			okub += i + 1;
			return ret;
		}
		if (okustr == "buffer_bool") {
			ret.gelenbool = buf[kacinci];
			okub++;
			if (debug) { if (ret.gelenbool) std::cout << "true "; else std::cout << "false "; }
			//std::cout << ret.gelenbool << " ";
			return ret;
		}
	}
	return ret;
}

class c_s_top {
public:
	SOCKET sock = NULL;
	bool oyunda = false;
	int oyuncu = -1;
	bool hata = false;
	bool is_oyunda() { return oyunda; }
	void set_oyunda(bool set) { oyunda = set; }
	bool is_bos() {
		if (sock == NULL) { return true; }
		else { return false;}
	}
	void setsock(SOCKET soc) {
		if (hata == false) {
		sock = soc;
		oyuncu = 1;
		}
	}
	void f_fd_clr(fd_set* set) {
		closesocket(sock);
		FD_CLR(sock, set);
	sock = NULL;
	oyunda = false;
	oyuncu = -1;
	hata = false;
	}
};

c_s_top f_s_bul(int oy,int arysize,c_s_top babus[],bool debug = false) {
	for (int i = 0; i < arysize; i++) {
		if (babus[i].oyuncu == oy) {
			if (debug) std::cout << "BABUSU BULDUK  " << i << std::endl;
			return babus[i];
		}
	}
	if (debug) {
		std::cout << "BABUSU BULAMADIK " << std::endl;
	}
	c_s_top bos;
	return bos;
}

c_s_top f_s_bosbul(int arysize, c_s_top babus[],bool debug = false) {
	for (int i = 0; i < arysize; i++) {
		if (babus[i].is_bos()) {
			if (debug) std::cout << " BOS BABUS BULUNDU XD " << i << std::endl;
			return babus[i];
		}
	}
	if(debug) std::cout << " BOS BABUS YOK LA AQ D: " << std::endl;
	c_s_top bos;
	bos.hata = true;
	return bos;
}

int f_s_sbul(SOCKET soc, int arysize, c_s_top babus[], bool debug = false) {
	for (int i = 0; i < arysize; i++) {
		std::cout << "BABUS " << i << " : " << babus[i].sock << std::endl;
		if (babus[i].sock == soc) {
			if (debug) std::cout << "SBABUSU BULDUK  " << i << std::endl;
			return i;
		}
	}
	if (debug) {
		std::cout << "SBABUSU BULAMADIK " << std::endl;
	}

	return -2;
}

int f_s_ibosbul(int arysize, c_s_top babus[], bool debug = false){
	for (int i = 0; i < arysize; i++) {
		if (babus[i].is_bos()) {
			if (debug) std::cout << " iBOS BABUS BULUNDU XD " << i << std::endl;
			return i;
		}
	}
	if (debug) std::cout << " iBOS BABUS YOK LA AQ D: " << std::endl;
	return -2;
}




class c_oyunbilgisi {
public:
	int i_s = 0;
	int max_i_s = 5;
	c_efektlist efcontrol[5];
	c_deste g_deck() {
		return deck;
	}

	int ef_bosbul() {
		for (int i = 0; i < max_i_s; i++) {
			if (efcontrol[i].efekt == 0) {
				efcontrol[i].clr();
				return i;
			}
		}
		return 4;
	}

	void setplayer_mezarsecili(int p,int set) {
		if (p == 1) {
			pl1.set_mezarsecili(set, true);
		}
		if (p == 2) {
			pl2.set_mezarsecili(set, true);
		}
	}

	void settksaldirkey(int s) {
		tk.get_saldirkey(s, true);
	}
	void setpl1efektonay(int a, int b, bool c) {
			pl1.set_efektonay(a, b, c);
	}
	void setpl2efektonay(int a, int b, bool c) {
		pl2.set_efektonay(a, b, c);
	}
	void msg2dopl1() {
		if (pl1.set_efektsira(0, pl1.set_efektoynat(0, false), false) == 6) {
			if (s_yerekoyufak_p1(pl1.set_altisec(0, false), 6, pl1.set_field_secili(0, false)) == 0) {//R�SK BUDUR
				pl1.set_el(0, pl1.set_altisec(0, false), true);
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 6;
					efcontrol[i_s].p1d1 = pl1.set_field_secili(0, false);
					efcontrol[i_s].p2d1 = pl1.set_field_secili(0, false) + 3;
				}
			}
			pl1.set_altisec(0, true);
		}
		if (pl1.set_efektsira(0, pl1.set_efektoynat(0, false), false) == 7) {
			if (s_yerekoyufak_p1(pl1.set_yedisec(0, false), 7, pl1.set_field_secili(0, false)) == 0) {//R�SK BUDUR
				pl1.set_el(0, pl1.set_yedisec(0, false), true);
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 7;
					efcontrol[i_s].p1d1 = pl1.set_field_secili(0, false);
					efcontrol[i_s].p2d1 = pl1.set_field_secili(0, false) + 3;
				}
			}
			pl1.set_yedisec(0, true);
		}
		pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
		pl1.set_efektonay(0, pl1.set_efektoynat(0, false), true);
		if (pl1.set_efektoynat(0, false) > 0) pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
	}
	void msg2dopl2() {
		if (pl2.set_efektsira(0, pl2.set_efektoynat(0, false), false) == 6) {
			if (s_yerekoyufak_p2(pl2.set_altisec(0, false), 6, pl2.set_field_secili(0, false)) == 0) {//R�SK BUDUR
				pl2.set_el(0, pl2.set_altisec(0, false), true);
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 6;
					efcontrol[i_s].p2d1 = pl2.set_field_secili(0, false);
					efcontrol[i_s].p1d1 = pl2.set_field_secili(0, false) + 3;
				}
			}
			pl2.set_altisec(0, true);
		}
		if (pl2.set_efektsira(0, pl2.set_efektoynat(0, false), false) == 7) {
			if (s_yerekoyufak_p2(pl2.set_yedisec(0, false), 7, pl2.set_field_secili(0, false)) == 0) {//R�SK BUDUR
				pl2.set_el(0, pl2.set_yedisec(0, false), true);
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 7;
					efcontrol[i_s].p2d1 = pl2.set_field_secili(0, false);
					efcontrol[i_s].p1d1 = pl2.set_field_secili(0, false) + 3;
				}
			}
			pl2.set_yedisec(0, true);
		}
		pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
		pl2.set_efektonay(0, pl2.set_efektoynat(0, false), true);
		if (pl2.set_efektoynat(0, false) > 0) pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);

	}
	c_tur_kontrol get_tk() {
		return tk;
	}
	void set_player(int player/*1 p1 2 p2*/, int set) {
		switch (player) {
		case 1:
			player1 = set;
		break;
		case 2:
			player2 = set;
		break;
		}
	}
	int get_player(int player/*1 p1 2 p2*/) {
		switch (player) {
		case 1:
			return player1;
			break;
		case 2:
			return player2;
			break;
		}
	}
	bool get_bos() { return bosmu;}
	void set_bos(bool aa) { bosmu = aa; }
	void f_clr() {
		player1 = -1;
		player2 = -1;
		bosmu = true;
		bosyer = -1;
		deste2 = -1;
		pl1.f_clr();
		pl2.f_clr();
		deck.deck_setup();
		deck.shuffle();
		tk.f_clr();
		svc.f_clr();
	}
	void f_setup() {
		bosyer = -1;
		deste2 = -1;
		pl1.f_clr();
		pl2.f_clr();
		deck.deck_setup();
		deck.shuffle();
		tk.f_clr();
		svc.f_clr();
	}
	void f_karehasapla(c_s_top cst1[]){
		tur_hesapla(pl1, pl2,cst1,player1,player2);
		tur_hesapla(pl2, pl1,cst1,player2,player1);

	}

	unsigned char ceviri8(int i) {
		unsigned char t;
		t = (uint8_t)&i;
		return t;
	}

	void player_tur_asamasi(c_player o1, c_player o2, c_s_top cstop1[], int oy1, int oy2) {
		if (o1.set_oyunturu() == 3) {
			//player 1
			if (tk.get_oyun_dur() == 0) {
				pl1.set_efektoynat(0, true);
				if (tk.get_tur_durum() == pl1.set_oyunturu() && pl1.set_efekt_kullan(0, false) == 0 && pl1.set_altiefekt(0, false) == 0 && pl1.set_uckullan(0, false) == 0 && pl1.set_yediefekt(0, false) == 0 && pl1.set_dokuzefekt(0, false) == 0 && pl1.set_dortefekt(0, false) == 0) {
					for (int i = 0; i < 3; i++) {
						if (pl1.get_field(i).f > 13) pl1.set_field_f(i, pl1.get_field(i).f % 13);
						if (pl2.get_field(i).f > 13) pl2.set_field_f(i, pl2.get_field(i).f % 13);
					}
					//p1 mi 2 mi onu kontrol etcem xd
					if (pl1.set_oyunturu() == 3) {
						//p1
						if (svc.p1_fclick[0] == true) {
							pl1.set_field_secili(0, true);
							std::cout << "f0" << std::endl;
						}
						if (svc.p1_fclick[1] == true) {
							pl1.set_field_secili(1, true);
							std::cout << "f1" << std::endl;
						}
						if (svc.p1_fclick[2] == true) {
							pl1.set_field_secili(2, true);
							std::cout << "f2" << std::endl;
						}
						//std::cout << "PL1 FIELD SECILI  " << pl1.set_field_secili(3, false) << std::endl;
					}
					if (pl1.set_oyunturu() == 6) {
						//p2
						if (svc.p2_fclick[0] == true) pl1.set_field_secili(0, true);
						if (svc.p2_fclick[1] == true) pl1.set_field_secili(1, true);
						if (svc.p2_fclick[2] == true) pl1.set_field_secili(2, true);
					}
					for (int ta_var = 0; ta_var < 10; ta_var++) {
						if (pl1.set_oyunturu() == 3) {
							//p1
							pl1.set_oyna(0, ta_var, true);
							pl1.set_oyna(svc.p1_elclick[ta_var], ta_var, true);
							std::cout << ta_var << " : " << pl1.set_el(0, ta_var, false) << std::endl;

							while (pl1.set_el(0, ta_var, false) > 13) {
								std::cout << "NEDEN OLMUYO OGLUM1" << std::endl << std::endl << std::endl << std::endl;
								pl1.set_el(pl1.set_el(0, ta_var, false) - 13, ta_var, true);
							}

							if (pl1.set_oyna(0, ta_var, false) == 1) {
								svc.p1_elclick[ta_var] = 0;
								pl1.set_oyna(0, ta_var, true);

								switch (pl1.set_el(0, ta_var, false)) {
								case 1:
									if (s_yerekoybuyuk_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 1;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 2:
									if (s_yerekoyufak_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else { 
										
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 2;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
										}
										
										pl1.efektset(2); }
									break;
								case 3:
									if (s_yerekoyufak_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else {

										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 3;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 4:
									if (s_yerekoyufak_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else {

										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 4;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 5:
									if (s_yerekoyufak_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else {

										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 5;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 6:
									pl1.efektset(6);
									pl1.set_altisec(ta_var, true);
									break;
								case 7:
									pl1.efektset(7);
									pl1.set_yedisec(ta_var, true);
									break;
								case 8:
									if (pl1.get_field(pl1.set_field_secili(0, false)).f != 0) {

										if (s_yerekoybuyuk_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
										else {


											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 14;
												efcontrol[i_s].p1d1 = ta_var;
												efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
												efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
											}

											pl1.efektset(8);

										}

									}
									else {

										if (s_yerekoyufak_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
										else {


											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 8;
												efcontrol[i_s].p1d1 = ta_var;
												efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
												efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
											}

										}

									}
									break;
								case 9:

									if (pl1.get_field(pl1.set_field_secili(0, false)).f != 0) {

										if (s_yerekoybuyuk_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
										else {
											pl1.efektset(9);


											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 15;
												efcontrol[i_s].p1d1 = ta_var;
												efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
												efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
											}
										}

									}
									else {

										if (s_yerekoyufak_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
										else {

											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 9;
												efcontrol[i_s].p1d1 = ta_var;
												efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
												efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
											}

										}

									}
									break;

								case 10:
									//burda kald�m
									if (s_yerekoybuyuk_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else {

										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 10;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 11:
									if (s_yerekoybuyuk_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else {
										int mill = 0;
										while (deck.deck_get(mill) == 0 && mill < 51) mill++;
										pl1.set_mezar(deck.deck_get(mill), 51, true);
										pl1.mezar_duzenle();
										pl1.set_valemill_kart(deck.deck_get(mill), true);
										deck.deck_set(mill, 0);
										pl1.efektset(11);


										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 11;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
											efcontrol[i_s].p1d3 = pl1.set_valemill_kart(0, false);
											efcontrol[i_s].p2d3 = pl1.set_valemill_kart(0, false);
										}
									}
									break;
								case 12:
									if (s_yerekoybuyuk_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else {


										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 12;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
										}

										pl1.efektset(12);
									}
									break;
								case 13:
									if (s_yerekoybuyuk_p1(ta_var, pl1.set_el(0, ta_var, false), pl1.set_field_secili(0, false)) < 0) { break; }
									else {


										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 13;
											efcontrol[i_s].p1d1 = ta_var;
											efcontrol[i_s].p1d2 = pl1.set_field_secili(0, false);
											efcontrol[i_s].p2d2 = pl1.set_field_secili(0, false) + 3;
										}

										int dene = 0;
										for (int i = 0; i < 3; i++) {
											if (pl1.get_field(i).f == 1 || pl1.get_field(i).f == 14) {
												dene = 1;
												break;
											}
										}

										if (dene == 1) {

											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 27;
												efcontrol[i_s].p1d1 = 1;
											}

											for (int i = 0; i < 3; i++) {
												if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker == 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(pl1.get_field(i).f, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i, pl1.get_field(i).f); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker != 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(11, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i, 11); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker == 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(pl2.get_field(i).f, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, pl2.get_field(i).f); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker != 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(11, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, 11); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }

											}
											pl1.mezar_duzenle();
											pl2.mezar_duzenle();
											pl1.set_askral(1, true);
										}
									}
									break;
								}
							}


						}

						




					}
				}
			}
		}
		if (o1.set_oyunturu() == 6) {
			//player 2
			if (tk.get_oyun_dur() == 0) {
				pl2.set_efektoynat(0, true);
				if (tk.get_tur_durum() == pl2.set_oyunturu() && pl2.set_efekt_kullan(0, false) == 0 && pl2.set_altiefekt(0, false) == 0 && pl2.set_uckullan(0, false) == 0 && pl2.set_yediefekt(0, false) == 0 && pl2.set_dokuzefekt(0, false) == 0 && pl2.set_dortefekt(0, false) == 0) {
					for (int i = 0; i < 3; i++) {
						if (pl2.get_field(i).f > 13) pl2.set_field_f(i, pl2.get_field(i).f % 13);
						if (pl1.get_field(i).f > 13) pl1.set_field_f(i, pl1.get_field(i).f % 13);
					}
					//p1 mi 2 mi onu kontrol etcem xd
					if (pl2.set_oyunturu() == 3) {
						//p1
						if (svc.p1_fclick[0] == true) {
							pl1.set_field_secili(0, true);
							std::cout << "f0" << std::endl;
						}
						if (svc.p1_fclick[1] == true) {
							pl1.set_field_secili(1, true);
							std::cout << "f1" << std::endl;
						}
						if (svc.p1_fclick[2] == true) {
							pl1.set_field_secili(2, true);
							std::cout << "f2" << std::endl;
						}
						//std::cout << "PL1 FIELD SECILI  " << pl2.set_field_secili(3, false) << std::endl;
					}
					if (pl2.set_oyunturu() == 6) {
						//p2
						if (svc.p2_fclick[0] == true) pl2.set_field_secili(0, true);
						if (svc.p2_fclick[1] == true) pl2.set_field_secili(1, true);
						if (svc.p2_fclick[2] == true) pl2.set_field_secili(2, true);
					}
					for (int ta_var = 0; ta_var < 10; ta_var++) {
						if (pl2.set_oyunturu() == 3) {
							//p1
							pl2.set_oyna(0, ta_var, true);
							pl2.set_oyna(svc.p1_elclick[ta_var], ta_var, true);


							if (pl2.set_oyna(0, ta_var, false) == 1) {
								switch (pl2.set_el(0, ta_var, false)) {
								case 1:
									if (s_yerekoybuyuk_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 1;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 2:
									if (s_yerekoyufak_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else { 
										
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 2;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
										
										pl2.efektset(2); }
									break;
								case 3:
									if (s_yerekoyufak_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {

										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 3;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 4:
									if (s_yerekoyufak_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 4;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 5:
									if (s_yerekoyufak_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 5;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 6:
									pl2.efektset(6);
									pl2.set_altisec(ta_var, true);
									break;
								case 7:
									pl2.efektset(7);
									pl2.set_yedisec(ta_var, true);
									break;
								case 8:
									if (pl2.get_field(pl2.set_field_secili(0, false)).f != 0) {

										if (s_yerekoybuyuk_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
										else {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 14;
												efcontrol[i_s].p2d1 = ta_var;
												efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
												efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											}
											pl2.efektset(8);
										}

									}
									else {

										if (s_yerekoyufak_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
										else {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 8;
												efcontrol[i_s].p2d1 = ta_var;
												efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
												efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											}
										}
									}
									break;
								case 9:

									if (pl2.get_field(pl2.set_field_secili(0, false)).f != 0) {

										if (s_yerekoybuyuk_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
										else {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 15;
												efcontrol[i_s].p2d1 = ta_var;
												efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
												efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											}
											pl2.efektset(9);

										}

									}
									else {

										if (s_yerekoyufak_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
										else {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 9;
												efcontrol[i_s].p2d1 = ta_var;
												efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
												efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											}
										}
									}
									break;

								case 10:
									//burda kald�m
									if (s_yerekoybuyuk_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 10;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 11:
									if (s_yerekoybuyuk_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										int mill = 0;
										while (deck.deck_get(mill) == 0 && mill < 51) mill++;
										pl2.set_mezar(deck.deck_get(mill), 51, true);
										pl2.mezar_duzenle();
										pl2.set_valemill_kart(deck.deck_get(mill), true);
										deck.deck_set(mill, 0);
										pl2.efektset(11);

										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 11;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											efcontrol[i_s].p1d3 = pl2.set_valemill_kart(0, false);
											efcontrol[i_s].p2d3 = pl2.set_valemill_kart(0, false);
										}
									}
									break;
								case 12:
									if (s_yerekoybuyuk_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 12;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
										pl2.efektset(12);

									}
									break;
								case 13:
									if (s_yerekoybuyuk_p1(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										int dene = 0;
										for (int i = 0; i < 3; i++) {
											if (pl2.get_field(i).f == 1 || pl2.get_field(i).f == 14) {
												dene = 1;
												break;
											}
										}

										if (dene == 1) {
											for (int i = 0; i < 3; i++) {
												if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker == 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(pl2.get_field(i).f, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i, pl2.get_field(i).f); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker != 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(11, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i, 11); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker == 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(pl1.get_field(i).f, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, pl1.get_field(i).f); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker != 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(11, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, 11); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }

											}
											pl2.mezar_duzenle();
											pl1.mezar_duzenle();
											pl2.set_askral(1, true);
										}
									}
									break;
								}
							}


						}

						if (pl2.set_oyunturu() == 6) {
							//p2
							pl2.set_oyna(0, ta_var, true);
							pl2.set_oyna(svc.p2_elclick[ta_var], ta_var, true);

							std::cout << ta_var << " : " << pl2.set_el(0, ta_var, false) << std::endl;

							while (pl2.set_el(0, ta_var, false) > 13) {
								std::cout << "NEDEN OLMUYO OGLUM2" << std::endl << std::endl << std::endl << std::endl << std::endl;
								pl2.set_el(pl2.set_el(0, ta_var, false) - 13, ta_var, true);
							}

							if (pl2.set_oyna(0, ta_var, false) == 1) {
								svc.p2_elclick[ta_var] = 0;
								pl2.set_oyna(0, ta_var, true);



								switch (pl2.set_el(0, ta_var, false)) {
								case 1:
									if (s_yerekoybuyuk_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 1;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 2:
									if (s_yerekoyufak_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 2;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
										
										pl2.efektset(2); }
									break;
								case 3:
									if (s_yerekoyufak_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 3;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 4:
									if (s_yerekoyufak_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 4;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 5:
									if (s_yerekoyufak_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 5;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 6:
									pl2.efektset(6);
									pl2.set_altisec(ta_var, true);
									break;
								case 7:
									pl2.efektset(7);
									pl2.set_yedisec(ta_var, true);
									break;
								case 8:
									if (pl2.get_field(pl2.set_field_secili(0, false)).f != 0) {

										if (s_yerekoybuyuk_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
										else {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 14;
												efcontrol[i_s].p2d1 = ta_var;
												efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
												efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											}
											pl2.efektset(8);
										}

									}
									else {

										if (s_yerekoyufak_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
										else {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 8;
												efcontrol[i_s].p2d1 = ta_var;
												efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
												efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											}
										}
									}
									break;
								case 9:

									if (pl2.get_field(pl2.set_field_secili(0, false)).f != 0) {

										if (s_yerekoybuyuk_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
										else {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 15;
												efcontrol[i_s].p2d1 = ta_var;
												efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
												efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											}
											pl2.efektset(9);
										}

									}
									else {

										if (s_yerekoyufak_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
										else {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 9;
												efcontrol[i_s].p2d1 = ta_var;
												efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
												efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											}
										}
									}
									break;

								case 10:
									//burda kald�m
									if (s_yerekoybuyuk_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 10;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
									}
									break;
								case 11:
									if (s_yerekoybuyuk_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										int mill = 0;
										while (deck.deck_get(mill) == 0 && mill < 51) mill++;
										pl2.set_mezar(deck.deck_get(mill), 51, true);
										pl2.mezar_duzenle();
										pl2.set_valemill_kart(deck.deck_get(mill), true);
										deck.deck_set(mill, 0);
										pl2.efektset(11);
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 11;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
											efcontrol[i_s].p1d3 = pl2.set_valemill_kart(0, false);
											efcontrol[i_s].p2d3 = pl2.set_valemill_kart(0, false);
										}
									}
									break;
								case 12:
									if (s_yerekoybuyuk_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 12;
											efcontrol[i_s].p2d1 = ta_var;
											efcontrol[i_s].p2d2 = pl2.set_field_secili(0, false);
											efcontrol[i_s].p1d2 = pl2.set_field_secili(0, false) + 3;
										}
										pl2.efektset(12);
									}
									break;
								case 13:
									if (s_yerekoybuyuk_p2(ta_var, pl2.set_el(0, ta_var, false), pl2.set_field_secili(0, false)) < 0) { break; }
									else {
										int dene = 0;
										for (int i = 0; i < 3; i++) {
											if (pl2.get_field(i).f == 1 || pl2.get_field(i).f == 14) {
												dene = 1;
												break;
											}
										}

										if (dene == 1) {
											i_s = ef_bosbul();
											if (i_s < max_i_s) {
												efcontrol[i_s].clr();
												efcontrol[i_s].efekt = 27;
												efcontrol[i_s].p2d1 = 1;
											}
											for (int i = 0; i < 3; i++) {
												if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker == 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(pl2.get_field(i).f, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i, pl2.get_field(i).f); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker != 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(11, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i, 11); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker == 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(pl1.get_field(i).f, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, pl1.get_field(i).f); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
												if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker != 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(11, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, 11); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }

											}
											pl2.mezar_duzenle();
											pl1.mezar_duzenle();
											pl2.set_askral(1, true);
										}
									}
									break;
								}
							}


						}




					}
				}
			}
		}

	}

	void player_savas_asamasi(c_player o1,c_player o2,c_s_top cstop1[],int oy1,int oy2){
		if (o1.set_oyunturu() == 3) {
			if (tk.get_oyun_dur() == 0) {
				if (tk.get_tur_durum() == 4 && tk.get_tur() != 4) {
					if (pl1.set_askral(0, false) == true) {
						for (int i = 0; i < 3; i++) {
							if (pl1.get_field(i).f == 13) { pl1.set_field_saldir(i, 1); }
						}
					}
					if (svc.p1_fclick[0] == true) pl1.set_battlesec(0, true);
					if (svc.p1_fclick[1] == true) pl1.set_battlesec(1, true);
					if (svc.p1_fclick[2] == true) pl1.set_battlesec(2, true);

					if (svc.p1_rfclick[0] == true) pl1.set_battlesecrakip(0, true);
					if (svc.p1_rfclick[1] == true) pl1.set_battlesecrakip(1, true);
					if (svc.p1_rfclick[2] == true) pl1.set_battlesecrakip(2, true);

					if (pl1.get_field(pl1.set_battlesec(0, false)).f == 1) pl1.set_field_f(pl1.set_battlesec(0, false), 14);
					if (pl2.get_field(pl1.set_battlesecrakip(0, false)).f == 1) pl2.set_field_f(pl1.set_battlesecrakip(0, false), 14);
					if (pl1.set_battlesec(0, false) == 3) {
						tk.tur_arttir(1, true);
					}
					if (tk.get_saldirkey(0, false) == 1 && pl1.get_field(pl1.set_battlesec(0, false)).saldir == 0) {
						if (pl1.get_field(pl1.set_battlesec(0, false)).f != 0 && pl2.get_field(pl1.set_battlesecrakip(0, false)).f != 0) {
							if ((pl1.get_field(pl1.set_battlesec(0, false)).f == 13 && pl1.set_askral(0, false) == false) || (pl1.get_field(pl1.set_battlesec(0, false)).f != 13)) {
								if (pl1.get_field(pl1.set_battlesec(0, false)).f > pl2.get_field(pl1.set_battlesecrakip(0, false)).f) {
									pl1.set_field_saldir(pl1.set_battlesec(0, false), 1);
									if (pl2.get_field(pl1.set_battlesecrakip(0, false)).joker == 1) {
										pl2.set_field_joker(pl1.set_battlesecrakip(0, false), 0);
										pl2.set_field_f(pl1.set_battlesecrakip(0, false), 0);
										pl2.set_field_tributeli(pl1.set_battlesecrakip(0, false), 0);
										pl2.set_mezar(11, 51, true);
									}
									else {
										pl2.set_mezar(pl2.get_field(pl1.set_battlesecrakip(0, false)).f, 51, true);
										pl2.set_field_joker(pl1.set_battlesecrakip(0, false), 0);
										pl2.set_field_f(pl1.set_battlesecrakip(0, false), 0);
										pl2.set_field_tributeli(pl1.set_battlesecrakip(0, false), 0);
									}
									pl2.mezar_duzenle();
								}
								else {
									if (pl1.get_field(pl1.set_battlesec(0, false)).f == pl2.get_field(pl1.set_battlesecrakip(0, false)).f) {
										pl1.set_field_saldir(pl1.set_battlesec(0, false), 1);
										if (pl1.get_field(pl1.set_battlesec(0, false)).f == 14) pl1.set_field_f(pl1.set_battlesec(0, false), 1);

										if (pl2.get_field(pl1.set_battlesecrakip(0, false)).joker == 1) {
											pl2.set_field_joker(pl1.set_battlesecrakip(0, false), 0);
											pl2.set_field_f(pl1.set_battlesecrakip(0, false), 0);
											pl2.set_field_tributeli(pl1.set_battlesecrakip(0, false), 0);
											pl2.set_mezar(11, 51, true);
										}
										else {
											pl2.set_mezar(pl2.get_field(pl1.set_battlesecrakip(0, false)).f, 51, true);
											pl2.set_field_joker(pl1.set_battlesecrakip(0, false), 0);
											pl2.set_field_f(pl1.set_battlesecrakip(0, false), 0);
											pl2.set_field_tributeli(pl1.set_battlesecrakip(0, false), 0);
										}
										pl2.mezar_duzenle();
										if (pl1.get_field(pl1.set_battlesec(0, false)).kapali == 1) {
											if (pl1.get_field(pl1.set_battlesec(0, false)).joker == 1) {
												pl1.set_field_joker(pl1.set_battlesec(0, false), 0);
												pl1.set_field_f(pl1.set_battlesec(0, false), 0);
												pl1.set_field_kapali(pl1.set_battlesec(0, false), 0);
												pl1.set_field_tributeli(pl1.set_battlesec(0, false), 0);
												deck.deck_set(0, 11);
											}
											else {
												pl1.set_field_kapali(pl1.set_battlesec(0, false), 0);
												deck.deck_set(0, pl1.get_field(pl1.set_battlesec(0, false)).f);
												pl1.set_field_tributeli(pl1.set_battlesec(0, false), 0);
												pl1.set_field_f(pl1.set_battlesec(0, false), 0);
											}
											deck.shuffle();
										}
										else {
											if (pl1.get_field(pl1.set_battlesec(0, false)).joker == 1) {
												pl1.set_field_joker(pl1.set_battlesec(0, false), 0);
												pl1.set_field_f(pl1.set_battlesec(0, false), 0);
												pl1.set_field_tributeli(pl1.set_battlesec(0, false), 0);
												pl1.set_mezar(11, 51, true);
											}
											else {
												pl1.set_mezar(pl1.get_field(pl1.set_battlesec(0, false)).f, 51, true);
												pl1.set_field_f(pl1.set_battlesec(0, false), 0);
												pl1.set_field_tributeli(pl1.set_battlesec(0, false), 0);
											}
										}
										pl1.mezar_duzenle();


									}
								}
							}
						}

					}
					tk.get_saldirkey(0, true);
				}
				if (pl1.get_field(pl1.set_battlesec(0, false)).f > 13) {
					pl1.set_field_f(pl1.set_battlesec(0, false), pl1.get_field(pl1.set_battlesec(0, false)).f % 13);
				}

			}

			
		}
		if (o1.set_oyunturu() == 6) {
				if (tk.get_oyun_dur() == 0) {
					if (tk.get_tur_durum() == 7) {
						if (pl2.set_askral(0, false) == true) {

							for (int i = 0; i < 3; i++) {
								if (pl2.get_field(i).f == 13) { pl2.set_field_saldir(i, 1); }
							}
						}
						if (svc.p2_fclick[0] == true) pl2.set_battlesec(0, true);
						if (svc.p2_fclick[1] == true) pl2.set_battlesec(1, true);
						if (svc.p2_fclick[2] == true) pl2.set_battlesec(2, true);
						//std::cout << pl2.get_field(0).saldir << " " << pl2.get_field(1).saldir << " " << pl2.get_field(2).saldir << " "<< tk.get_saldirkey(0,false) <<std::endl;

						//std::cout << "pl2battlesec " << pl2.set_battlesec(3, false);

						if (svc.p2_rfclick[0] == true) pl2.set_battlesecrakip(0, true);
						if (svc.p2_rfclick[1] == true) pl2.set_battlesecrakip(1, true);
						if (svc.p2_rfclick[2] == true) pl2.set_battlesecrakip(2, true);

						//std::cout << " pl2bsecr " << pl2.set_battlesecrakip(3, false) << std::endl;

						if (pl2.get_field(pl2.set_battlesec(0, false)).f == 1) pl2.set_field_f(pl2.set_battlesec(0, false), 14);
						if (pl1.get_field(pl2.set_battlesecrakip(0, false)).f == 1) pl1.set_field_f(pl2.set_battlesecrakip(0, false), 14);
						if (pl2.set_battlesec(0, false) == 3) {
							tk.tur_arttir(1, true);
						}
						if (tk.get_saldirkey(0, false) == 1 && pl2.get_field(pl2.set_battlesec(0, false)).saldir == 0) {
							if (pl2.get_field(pl2.set_battlesec(0, false)).f != 0 && pl1.get_field(pl2.set_battlesecrakip(0, false)).f != 0) {
								if ((pl2.get_field(pl2.set_battlesec(0, false)).f == 13 && pl2.set_askral(0, false) == false) || (pl2.get_field(pl2.set_battlesec(0, false)).f != 13)) {
									if (pl2.get_field(pl2.set_battlesec(0, false)).f > pl1.get_field(pl2.set_battlesecrakip(0, false)).f) {
										pl2.set_field_saldir(pl2.set_battlesec(0, false), 1);
										if (pl1.get_field(pl2.set_battlesecrakip(0, false)).joker == 1) {
											pl1.set_field_joker(pl2.set_battlesecrakip(0, false), 0);
											pl1.set_field_f(pl2.set_battlesecrakip(0, false), 0);
											pl1.set_field_tributeli(pl2.set_battlesecrakip(0, false), 0);
											pl1.set_mezar(11, 51, true);
										}
										else {
											pl1.set_mezar(pl1.get_field(pl2.set_battlesecrakip(0, false)).f, 51, true);
											pl1.set_field_joker(pl2.set_battlesecrakip(0, false), 0);
											pl1.set_field_f(pl2.set_battlesecrakip(0, false), 0);
											pl1.set_field_tributeli(pl2.set_battlesecrakip(0, false), 0);
										}
										pl1.mezar_duzenle();
									}
									else {
										if (pl2.get_field(pl2.set_battlesec(0, false)).f == pl1.get_field(pl2.set_battlesecrakip(0, false)).f) {
											pl2.set_field_saldir(pl2.set_battlesec(0, false), 1);
											if (pl2.get_field(pl2.set_battlesec(0, false)).f == 14) pl2.set_field_f(pl2.set_battlesec(0, false), 1);

											if (pl1.get_field(pl2.set_battlesecrakip(0, false)).joker == 1) {
												pl1.set_field_joker(pl2.set_battlesecrakip(0, false), 0);
												pl1.set_field_f(pl2.set_battlesecrakip(0, false), 0);
												pl1.set_field_tributeli(pl2.set_battlesecrakip(0, false), 0);
												pl1.set_mezar(11, 51, true);
											}
											else {
												pl1.set_mezar(pl1.get_field(pl2.set_battlesecrakip(0, false)).f, 51, true);
												pl1.set_field_joker(pl2.set_battlesecrakip(0, false), 0);
												pl1.set_field_f(pl2.set_battlesecrakip(0, false), 0);
												pl1.set_field_tributeli(pl2.set_battlesecrakip(0, false), 0);
											}
											pl1.mezar_duzenle();
											if (pl2.get_field(pl2.set_battlesec(0, false)).kapali == 1) {
												if (pl2.get_field(pl2.set_battlesec(0, false)).joker == 1) {
													pl2.set_field_joker(pl2.set_battlesec(0, false), 0);
													pl2.set_field_f(pl2.set_battlesec(0, false), 0);
													pl2.set_field_kapali(pl2.set_battlesec(0, false), 0);
													pl2.set_field_tributeli(pl2.set_battlesec(0, false), 0);
													deck.deck_set(0, 11);
												}
												else {
													pl2.set_field_kapali(pl2.set_battlesec(0, false), 0);
													deck.deck_set(0, pl2.get_field(pl2.set_battlesec(0, false)).f);
													pl2.set_field_tributeli(pl2.set_battlesec(0, false), 0);
													pl2.set_field_f(pl2.set_battlesec(0, false), 0);
												}
												deck.shuffle();
											}
											else {
												if (pl2.get_field(pl2.set_battlesec(0, false)).joker == 1) {
													pl2.set_field_joker(pl2.set_battlesec(0, false), 0);
													pl2.set_field_f(pl2.set_battlesec(0, false), 0);
													pl2.set_field_tributeli(pl2.set_battlesec(0, false), 0);
													pl2.set_mezar(11, 51, true);
												}
												else {
													pl2.set_mezar(pl2.get_field(pl2.set_battlesec(0, false)).f, 51, true);
													pl2.set_field_f(pl2.set_battlesec(0, false), 0);
													pl2.set_field_tributeli(pl2.set_battlesec(0, false), 0);
												}
											}
											pl2.mezar_duzenle();


										}
									}
								}
							}

						}
						tk.get_saldirkey(0, true);
					}
					if (pl2.get_field(pl2.set_battlesec(0, false)).f > 13) {
						pl2.set_field_f(pl2.set_battlesec(0, false), pl2.get_field(pl2.set_battlesec(0, false)).f % 13);
					}

				}

				
			}
		
		
	}

	void onayg_p1(SOCKET socket1) {
		if (socket1 != NULL) {
			std::string msg1;
			if (pl1.set_efektsira(0, pl1.set_efektoynat(0, false), false) != 11) {
				msg1 = " " + std::to_string(pl1.set_efektsira(0, pl1.set_efektoynat(0, false), false)) + " nin efekti aktiflessin mi?" + "\0";
			}
			else {
				msg1 = " " + std::to_string(pl1.set_efektsira(0, pl1.set_efektoynat(0, false), false)) + " nin efekti aktiflessin mi? " + std::to_string(pl1.set_valemill_kart(0, false)) + " millendi" + "\0";
			}
			unsigned char ggsin1[16];
			ZeroMemory(ggsin1, 16);
			uint16_t s = 2;
			ggsin1[0] = s;
			unsigned char gp = s;
			int uz = msg1.length();
			char text1[4096];
			//ZeroMemory(text1, 4096);
			memset(text1, 0x00, 4096);/*
			std::stringstream ss;
			ss << msg1;
			ss >> text1;
			*/
			strcpy_s(text1, msg1.c_str());
			text1[0] = gp;
			//send(cstop1[oy1].sock, (char*)&ggsin1, 1, 0);
			send(socket1, text1,uz+2, 0);
			//std::cout << text1 << std::endl;
		}
	}

	void onayg_p2(SOCKET socket1) {
		if (socket1 != NULL) {
			std::string msg1;
			if (pl2.set_efektsira(0, pl2.set_efektoynat(0, false), false) != 11) {
				msg1 = " " + std::to_string(pl2.set_efektsira(0, pl2.set_efektoynat(0, false), false)) + " nin efekti aktiflessin mi?" +"\0";
			}
			else {
				msg1 = " " + std::to_string(pl2.set_efektsira(0, pl2.set_efektoynat(0, false), false)) + " nin efekti aktiflessin mi? " + std::to_string(pl2.set_valemill_kart(0, false)) + " millendi" + "\0";
			}
			unsigned char ggsin1[16];
			ZeroMemory(ggsin1, 16);
			uint16_t s = 2;
			ggsin1[0] = s;
			unsigned char gp = s;
			int uz = msg1.length();
			char text1[4096];
			ZeroMemory(text1, 4096);
			//std::stringstream ss;
			//ss  << gp << msg1;
			//ss >> text1;
			//send(cstop1[oy1].sock, (char*)&ggsin1, 1, 0);
			strcpy_s(text1, msg1.c_str());
			text1[0] = gp;
			send(socket1, text1, uz + 2, 0);
			//std::cout << text1 << std::endl;
		}
	}



	void tur_hesapla(c_player o1, c_player o2, c_s_top cstop1[], int oy1, int oy2) {
		std::cout << tk.get_tur() << " " << tk.get_tur_durum() << std::endl;
		//OYUNDUR KONTROL
		//std::cout << "semenddeeri " << std::endl;
		if (o1.set_oyunturu() == 3) {
			pl1.s_onayg(false);
			//pl1
			int dandun = 0;
			for (int i = 0; i < 10; i++) {
				if (pl1.set_efektsira(0, i, false) != 0)dandun++; break;
			}
			if (dandun != 0) {
				tk.get_oyun_dur(1, true);
			}
			else {
				tk.get_oyun_dur(0, true);
			}
			//BUNA �S�M KOYMAMI�IM AGA B
			if (tk.get_oyun_dur() == 1) {
				if (pl1.set_efektoynat() > -1) {
					if (pl1.set_efektsira(0, pl1.set_efektoynat(), false) == 0) {
						pl1.set_efektonay(0, pl1.set_efektoynat(), true);
						if (pl1.set_efektoynat() > 0)pl1.set_efektoynat(pl1.set_efektoynat() - 1, true);
					}
					else {
						if (pl1.set_efektonay(0, pl1.set_efektoynat(), false) == 0) {
							s_efektler_p1(pl1.set_efektsira(0, pl1.set_efektoynat(), false));
						}
						else {
							//cliente info g�nder
							//u8 2
							/*
							var drawtext = "";
							if(player_1.efektsira[player_1.efektoynat[ts],ts] != 11)drawtext = string(player_1.efektsira[player_1.efektoynat[ts],ts]) + " nin efekti aktiflessin mi?" else drawtext = string(player_1.efektsira[player_1.efektoynat[ts],ts]) + " nin efekti aktiflessin mi?" + string(player_1.valemill_kart[ts]) + " millendi"
							buffer_write(server_obj.buffer_serv,buffer_string,drawtext);
							*/
							pl1.s_onayg(true);


						}

					}
				}

				if (pl1.set_efektoynat() == 0 && pl1.set_efektsira(0, pl1.set_efektoynat(), false) == 0) {
					pl1.efektcheck();
					pl1.set_efektoynat(9, true);
					pl1.efektoynatcheck();
				}
			}
			//BURAYA VALE KONTROL
			if (tk.get_oyun_dur() == 0) {
				for (int i = 0; i < 3; i++) {
					if (pl1.get_field(i).f == 11) {
						pl1.set_field_joker(i, 1);
					}
					if (pl1.get_field(i).f > 13) {
						pl1.set_field_f(i, pl1.get_field(i).f % 13);
					}
				}
			}

			//�lk tur kartlar� �ek

			if (tk.get_oyun_dur() == 0) {
				if (tk.get_tur() == 1 && pl1.set_oyunturu() == 3) {
					int d = 0;
					while (deck.deck_get(d) == 0 && d < 47) d++;
					for (int i = 0; i < 5; i++) {
						pl1.set_el(deck.deck_get(d + i), i, true);
						deck.deck_set(d + i, 0);
					}
					tk.tur_arttir(1, true);

				}
				if (tk.get_tur_durum() == 1 && pl1.set_oyunturu() == 3) {
					tk.tur_arttir(1, true);

				}

				if (tk.get_tur() == 2 && pl1.set_oyunturu() == 6) {
					int d = 0;
					while (deck.deck_get(d) == 0 && d < 47) d++;
					for (int i = 0; i < 5; i++) {
						pl2.set_el(deck.deck_get(d + i), i, true);
						deck.deck_set(d + i, 0);
					}
					tk.tur_arttir(1, true);

				}
				if (tk.get_tur_durum() == 2 && pl1.set_oyunturu() == 6) {
					tk.tur_arttir(1, true);
				}

			}
	}
		if (o1.set_oyunturu() == 6) {
			//pl2
			pl2.s_onayg(false);
			int dandun = 0;
			for (int i = 0; i < 10; i++) {
				if (pl2.set_efektsira(0, i, false) != 0)dandun++; break;
			}
			if (dandun != 0) {
				tk.get_oyun_dur(1, true);
			}
			else {
				tk.get_oyun_dur(0, true);
			}
			//BUNA �S�M KOYMAMI�IM AGA B
			if (tk.get_oyun_dur() == 1) {
				if (pl2.set_efektoynat() > -1) {
					if (pl2.set_efektsira(0, pl2.set_efektoynat(), false) == 0) {
						pl2.set_efektonay(0, pl2.set_efektoynat(), true);
						if (pl2.set_efektoynat() > 0)pl2.set_efektoynat(pl2.set_efektoynat() - 1, true);
					}
					else {
						if (pl2.set_efektonay(0, pl2.set_efektoynat(), false) == 0) {
							s_efektler_p2(pl2.set_efektsira(0, pl2.set_efektoynat(), false));
						}
						else {
							//cliente info g�nder
							//u8 2
							/*
							var drawtext = "";
							if(player_1.efektsira[player_1.efektoynat[ts],ts] != 11)drawtext = string(player_1.efektsira[player_1.efektoynat[ts],ts]) + " nin efekti aktiflessin mi?" else drawtext = string(player_1.efektsira[player_1.efektoynat[ts],ts]) + " nin efekti aktiflessin mi?" + string(player_1.valemill_kart[ts]) + " millendi"
							buffer_write(server_obj.buffer_serv,buffer_string,drawtext);
							*/
							pl2.s_onayg(true);
							/*
							if (cstop1[oy1].sock != NULL) {
								std::string msg1;
								if (pl2.set_efektsira(0, pl2.set_efektoynat(0, false), false) != 11) {
									msg1 = std::to_string(pl2.set_efektsira(0, pl2.set_efektoynat(0, false), false)) + " nin efekti aktiflessin mi?";
								}
								else {
									msg1 = std::to_string(pl2.set_efektsira(0, pl2.set_efektoynat(0, false), false)) + " nin efekti aktiflessin mi? " + std::to_string(pl2.set_valemill_kart(0, false)) + " millendi";
								}

								//std::cout << " TEWXT1: " << text1 << std::endl;

								unsigned char ggsin1[16];
								ZeroMemory(ggsin1, 16);
								uint16_t s = 2;
								ggsin1[0] = s;
								unsigned char gp = s;
								int uz = msg1.length();
								char text1[4096];
								ZeroMemory(text1, 4096);
								std::stringstream ss;
								ss << gp <<msg1;
								ss >> text1;
								//send(cstop1[oy1].sock, (char*)&ggsin1, 1, 0);
								send(cstop1[oy1].sock, text1, uz + 2, 0);
								
							}*/

						}

					}
				}

				if (pl2.set_efektoynat() == 0 && pl2.set_efektsira(0, pl2.set_efektoynat(), false) == 0) {
					pl2.efektcheck();
					pl2.set_efektoynat(9, true);
					pl2.efektoynatcheck();
				}
			}
			//BURAYA VALE KONTROL
			if (tk.get_oyun_dur() == 0) {
				for (int i = 0; i < 3; i++) {
					if (pl2.get_field(i).f == 11) {
						pl2.set_field_joker(i, 1);
					}
					if (pl2.get_field(i).f > 13) {
						pl2.set_field_f(i, pl2.get_field(i).f % 13);
					}
				}
			}

			//�lk tur kartlar� �ek

			if (tk.get_oyun_dur() == 0) {
				if (tk.get_tur() == 1 && pl2.set_oyunturu() == 3) {
					int d = 0;
					while (deck.deck_get(d) == 0 && d < 47) d++;
					for (int i = 0; i < 5; i++) {
						pl1.set_el(deck.deck_get(d + i), i, true);
						deck.deck_set(d + i, 0);
					}
					tk.tur_arttir(1, true);

				}
				if (tk.get_tur_durum() == 1 && pl2.set_oyunturu() == 3) {
					tk.tur_arttir(1, true);

				}

				if (tk.get_tur() == 2 && pl2.set_oyunturu() == 6) {
					int d = 0;
					while (deck.deck_get(d) == 0 && d < 47) d++;
					for (int i = 0; i < 5; i++) {
						pl2.set_el(deck.deck_get(d + i), i, true);
						deck.deck_set(d + i, 0);
					}
					tk.tur_arttir(1, true);

				}
				if (tk.get_tur_durum() == 2 && pl2.set_oyunturu() == 6) {
					tk.tur_arttir(1, true);
				}

			}
		}


		if (tk.r_oyit() != 0) {
			tk.get_oyun_dur(1, true);
		}

		//TUR A�AMASI (ALLAH YARDIMCIM OLSUN)
		player_tur_asamasi(o1, o2, cstop1, oy1, oy2);

		//sava� turu (sikerler bu sadece p1 �aka �aka p2 de var xd)
		player_savas_asamasi(o1, o2, cstop1, oy1, oy2);

		//end tur kontrol
		if (tk.get_oyun_dur() == 0) {
			if (o1.set_oyunturu() == 3) {
				//pl1
				int siq = 0;
				if (pl1.set_oyunturu(0, false) == 3) siq = 5;
				if (pl1.set_oyunturu(0, false) == 6) siq = 8;
				if (tk.get_tur_durum() == siq) {
					int savdene = 0;
					int savdene1 = 0;
					for (int i = 0; i < 3; i++) {
						if (pl2.get_field(i).f != 0) savdene++;
						if (pl1.get_field(i).f != 0 && pl1.get_field(i).saldir == 0) savdene1++;
					}
					if (savdene == 0 && tk.get_tur() != 5) {
						pl2.set_can(pl2.set_can(0, false) - savdene1, true);
						if (pl2.set_can(0, false) < 0) pl2.set_can(0, true);
					}
					pl1.set_battlesec(0, true);
					pl1.set_field_saldir(0, 0);
					pl1.set_field_saldir(1, 0);
					pl1.set_field_saldir(2, 0);
					pl1.set_battlesecrakip(0, true);
					for (int i = 0; i < 3; i++) {
						if (pl1.get_field(i).joker == 1) {
							pl1.set_field_joker(i, 0);
							pl1.set_field_f(i, 11);
						}
						if (pl2.get_field(i).joker == 1) {
							pl2.set_field_joker(i, 0);
							pl2.set_field_f(i, 11);
						}
						int shufdene = 0;
						if (pl1.get_field(i).kapali == 1) {
							deck.deck_set(0, pl1.get_field(i).f);
							pl1.set_field_f(i, 0);
							pl1.set_field_kapali(i, 0);
							shufdene++;
						}
						if (shufdene != 0)deck.shuffle();
					}
					int destedene = 0;
					for (int i = 0; i < 10; i++) {
						if (pl1.set_el(0, i, false) != 0) {
							destedene++;
							int destebos = 51;
							while (deck.deck_get(destebos) != 0 && destebos > 0) {
								destebos--;
							}
							deck.deck_set(destebos, pl1.set_el(0, i, false));
							pl1.set_el(0, i, true);
						}
					}
					if (destedene != 0) {
						destedene = 0;
						deck.shuffle();
					}
					int d = 0;
					while (deck.deck_get(d) == 0 && d < 51) d++;
					if (d == 51 && deck.deck_get(d) == 0) {
						//kart kalmad� adsasd
					}
					if (d > 47) d = 47;
					for (int i = 0; i < 5; i++) {
						pl1.set_el(deck.deck_get(i + d), i, true);
						deck.deck_set(d + i, 0);
					}
					tk.tur_arttir(1, true);
					pl1.set_beskullan(0, true);
					pl1.set_sekizkullan(0, true);
					pl1.set_askral(false, true);
				}
			}
			if (o1.set_oyunturu() == 6) {
				//pl2
				int siq = 0;
				if (pl2.set_oyunturu(0, false) == 3) siq = 5;
				if (pl2.set_oyunturu(0, false) == 6) siq = 8;
				if (tk.get_tur_durum() == siq) {
					int savdene = 0;
					int savdene1 = 0;
					for (int i = 0; i < 3; i++) {
						if (pl1.get_field(i).f != 0) savdene++;
						if (pl2.get_field(i).f != 0 && pl2.get_field(i).saldir == 0) savdene1++;
					}
					if (savdene == 0 && tk.get_tur() != 5) {
						pl1.set_can(pl1.set_can(0, false) - savdene1, true);
						if (pl1.set_can(0, false) < 0) pl1.set_can(0, true);
					}
					pl2.set_battlesec(0, true);
					pl2.set_field_saldir(0, 0);
					pl2.set_field_saldir(1, 0);
					pl2.set_field_saldir(2, 0);
					pl2.set_battlesecrakip(0, true);
					for (int i = 0; i < 3; i++) {
						if (pl2.get_field(i).joker == 1) {
							pl2.set_field_joker(i, 0);
							pl2.set_field_f(i, 11);
						}
						if (pl1.get_field(i).joker == 1) {
							pl1.set_field_joker(i, 0);
							pl1.set_field_f(i, 11);
						}
						int shufdene = 0;
						if (pl2.get_field(i).kapali == 1) {
							deck.deck_set(0, pl2.get_field(i).f);
							pl2.set_field_f(i, 0);
							pl2.set_field_kapali(i, 0);
							shufdene++;
						}
						if (shufdene != 0)deck.shuffle();
					}
					int destedene = 0;
					for (int i = 0; i < 10; i++) {
						if (pl2.set_el(0, i, false) != 0) {
							destedene++;
							int destebos = 51;
							while (deck.deck_get(destebos) != 0 && destebos > 0) {
								destebos--;
							}
							deck.deck_set(destebos, pl2.set_el(0, i, false));
							pl2.set_el(0, i, true);
						}
					}
					if (destedene != 0) {
						destedene = 0;
						deck.shuffle();
					}
					int d = 0;
					while (deck.deck_get(d) == 0 && d < 51) d++;
					if (d == 51 && deck.deck_get(d) == 0) {
						//kart kalmad� adsasd
					}
					if (d > 47) d = 47;
					for (int i = 0; i < 5; i++) {
						pl2.set_el(deck.deck_get(i + d), i, true);
						deck.deck_set(d + i, 0);
					}
					tk.tur_arttir(1, true);
					pl2.set_beskullan(0, true);
					pl2.set_sekizkullan(0, true);
					pl2.set_askral(false, true);
				}
			}
		}


		//oyun bit kontrol
		tk.s_oybit(oyunbit_kontrol(o1, o2, cstop1, oy1, oy2));
		if (tk.r_oyit() != 0) {
			std::cout << "OYUNBITIS" << std::endl;
			tk.get_oyun_dur(1, true);
			if (svc.oyuntekrarp1 == 1 && svc.oyuntekrarp2 == 1) {
				f_setup();
				int p = player1;
				player1 = player2;
				player2 = p;
					setployuntur(1, 3, true);
					setployuntur(2, 6, true);


				std::cout << "TEKRAR OYNAA" << std::endl;
			}
		}
	}


	int s_yerekoyufak_p1(int elsay, int sayi, int secili) {
		int yer = -1;
		if (pl1.get_field(secili).f == 0) {
			pl1.set_field_f(secili, sayi);
			pl1.set_el(0, elsay, true);
			pl1.set_field_saldir(secili, 0);
			yer = 0;
		}
		return yer;
	}
	int s_yerekoyufak_p2(int elsay, int sayi, int secili) {
		int yer = -1;
		if (pl2.get_field(secili).f == 0) {
			pl2.set_field_f(secili, sayi);
			pl2.set_el(0, elsay, true);
			pl2.set_field_saldir(secili, 0);
			yer = 0;
		}
		return yer;
	}
	
	int s_yerekoyvale_p1(int elsay, int sayi, int secili) {
		int yer = -1;
		if (pl1.get_field(secili).f == 0) {
			pl1.set_field_f(secili, sayi);
			yer = 0;
		}
		return yer;
	}
	int s_yerekoyvale_p2(int elsay, int sayi, int secili) {
		int yer = -1;
		if (pl2.get_field(secili).f == 0) {
			pl2.set_field_f(secili, sayi);
			yer = 0;
		}
		return yer;
	}
	
	int s_yerekoybuyuk_p1(int elsay, int sayi, int secili) {
		int yer = -1;
		if (pl1.get_field(secili).f != 0) {
			int r = -1;
			for (int t = 0; t < 52; t++) {
				if (r == -1) {
					if (pl1.set_mezar(0, t, false) == 0) {
						r = t;
					}
				}
			}
			if (pl1.get_field(secili).f == 5&&pl1.set_beskullan(false,false)==false) {
				//CLIENTE EFEKT BILDIR
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].efekt = 19;
					efcontrol[i_s].p1d1 = secili;
					efcontrol[i_s].p2d1 = secili + 3;
					efcontrol[i_s].p1d2 = elsay;
				}
				

				pl1.set_beskullan(true, true);
				if (pl1.get_field(secili).joker == 1) {pl1.set_el(11, elsay, true);}else{pl1.set_el(5, elsay, true); }
				pl1.set_field_f(secili, sayi);
				pl1.set_field_joker(secili, 0);
				pl1.set_field_tributeli(secili, 1);
				pl1.set_field_kapali(secili, 0);
				pl1.set_field_saldir(secili, 0);
				yer = 0;
			}
			else {
				if (pl1.get_field(secili).f == 3) {
					pl1.efektset(3);
				}
				if (pl1.get_field(secili).f == 4) {
					pl1.efektset(4);
				}
				if (pl1.get_field(secili).f == 10&&pl1.get_field(secili).tributeli == 1) {


					int ondraw = 0;
					while (deck.deck_get(ondraw) == 0 && ondraw <51) {
						ondraw++;
					}
					int eldraw = 0;
					while (pl1.set_el(0, eldraw, false) != 0 && eldraw <9) eldraw++;
					pl1.set_el(deck.deck_get(ondraw), eldraw, true);
					deck.deck_set(ondraw, 0);

					//CLIENTE BINDI GONDER

					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].efekt = 24;
						efcontrol[i_s].p1d1 = secili;
						efcontrol[i_s].p2d1 = secili + 3;
						efcontrol[i_s].p1d2 = pl1.set_el(0, eldraw, false);
					}
					

					ondraw = 0;
					eldraw = 0;
					while (eldraw < 255) {
						ondraw = rand() % 10;
						eldraw++;
						if (pl2.set_el(0, ondraw, false) != 0) {
							eldraw = 255;
						}
					}
					eldraw = 0;
					while (pl2.set_mezar(0, eldraw, false) != 0) eldraw++;
					pl2.set_mezar(pl2.set_el(0, ondraw, false), eldraw, true);
					if (i_s < max_i_s) {
						efcontrol[i_s].p1d3 = pl2.set_el(0, ondraw, false);
						efcontrol[i_s].p2d3 = pl2.set_el(0, ondraw, false);
					}
					
					pl2.set_el(0, ondraw, true);


					
				}
				if (pl1.get_field(secili).kapali == 1) {
					if (pl1.get_field(secili).joker == 1) { deck.deck_set(0, 11); } else { deck.deck_set(0, pl1.get_field(secili).f); }
					deck.shuffle();
				}
				else {
					if (pl1.get_field(secili).joker == 1) pl1.set_mezar(11, r, true); else pl1.set_mezar(pl1.get_field(secili).f, r, true);
				}
				pl1.set_field_f(secili, sayi);
				pl1.set_field_joker(secili, 0);
				pl1.set_field_tributeli(secili, 1);
				pl1.set_field_kapali(secili, 0);
				pl1.set_field_saldir(secili, 0);
				pl1.set_el(0, elsay, true);
				yer = 0;
			}
		}
		return yer;
	}
	int s_yerekoybuyuk_p2(int elsay, int sayi, int secili) {
		int yer = -1;
		if (pl2.get_field(secili).f != 0) {
			int r = -1;
			for (int t = 0; t < 52; t++) {
				if (r == -1) {
					if (pl2.set_mezar(0, t, false) == 0) {
						r = t;
					}
				}
			}
			if (pl2.get_field(secili).f == 5 && pl2.set_beskullan(false, false) == false) {
				//CLIENTE EFEKT BILDIR
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].efekt = 19;
					efcontrol[i_s].p1d1 = secili + 3;
					efcontrol[i_s].p2d1 = secili;
					efcontrol[i_s].p2d2 = elsay;
				}

				pl2.set_beskullan(true, true);
				if (pl2.get_field(secili).joker == 1) { pl2.set_el(11, elsay, true); }
				else { pl2.set_el(5, elsay, true); }
				pl2.set_field_f(secili, sayi);
				pl2.set_field_joker(secili, 0);
				pl2.set_field_tributeli(secili, 1);
				pl2.set_field_kapali(secili, 0);
				pl2.set_field_saldir(secili, 0);
				yer = 0;
			}
			else {
				if (pl2.get_field(secili).f == 3) {
					pl2.efektset(3);
				}
				if (pl2.get_field(secili).f == 4) {
					pl2.efektset(4);
				}
				if (pl2.get_field(secili).f == 10 && pl2.get_field(secili).tributeli == 1) {
					



					int ondraw = 0;
					while (deck.deck_get(ondraw) == 0 && ondraw < 51) {
						ondraw++;
					}
					int eldraw = 0;
					while (pl2.set_el(0, eldraw, false) != 0 && eldraw < 9) eldraw++;
					pl2.set_el(deck.deck_get(ondraw), eldraw, true);
					deck.deck_set(ondraw, 0);
					//cliente info
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].efekt = 24;
						efcontrol[i_s].p1d1 = secili + 3;
						efcontrol[i_s].p2d1 = secili;
						efcontrol[i_s].p2d2 = pl2.set_el(0, eldraw, false);
					}
					ondraw = 0;
					eldraw = 0;
					while (eldraw < 255) {
						ondraw = rand() % 10;
						eldraw++;
						if (pl1.set_el(0, ondraw, false) != 0) {
							eldraw = 255;
						}
					}
					eldraw = 0;
					while (pl1.set_mezar(0, eldraw, false) != 0) eldraw++;
					pl1.set_mezar(pl1.set_el(0, ondraw, false), eldraw, true);
					if (i_s < max_i_s) {
						efcontrol[i_s].p1d3 = pl1.set_el(0, ondraw, false);
						efcontrol[i_s].p2d3 = pl1.set_el(0, ondraw, false);
					}
					pl1.set_el(0, ondraw, true);
				}
				if (pl2.get_field(secili).kapali == 1) {
					if (pl2.get_field(secili).joker == 1) { deck.deck_set(0, 11); }
					else { deck.deck_set(0, pl2.get_field(secili).f); }
					deck.shuffle();
				}
				else {
					if (pl2.get_field(secili).joker == 1) pl2.set_mezar(11, r, true); else pl2.set_mezar(pl2.get_field(secili).f, r, true);
				}
				pl2.set_field_f(secili, sayi);
				pl2.set_field_joker(secili, 0);
				pl2.set_field_tributeli(secili, 1);
				pl2.set_field_kapali(secili, 0);
				pl2.set_field_saldir(secili, 0);
				pl2.set_el(0, elsay, true);
				yer = 0;
			}
		}
		return yer;
	}

	void s_valemill_p1(int atilankart, int getfield) {
		if (atilankart != 0) {
			int i = 0;
			pl1.set_field_f(getfield, 0);
			pl1.set_field_joker(getfield, 1);
			switch (atilankart) {
			case 1:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 28;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
				}
				break;
			case 2:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 29;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
					pl1.efektset(2); }
				break;
			case 3:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 30;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
				}
				break;
			case 4:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 31;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
				}
				break;
			case 5:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 32;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
				}
				break;
			case 6:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 33;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
				}
				break;
			case 7:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 34;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
				}
				break;
			case 8:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 35;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
					if (pl1.get_field(getfield).tributeli == 1) {
						pl1.efektset(8);
					}
				}
				break;
			case 9:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 36;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
					if (pl1.get_field(getfield).tributeli == 1) {
						pl1.efektset(9);
					}
				}
				break;
			case 10:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 37;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
				}
				break;
			case 11:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {

					int mill = 0;
					while (deck.deck_get(mill) == 0 && mill < 51) mill++;
					int millmezar = 0;
					while (pl1.set_mezar(0, millmezar, false) != 0 && millmezar < 51) millmezar++;
					pl1.set_mezar(deck.deck_get(mill), millmezar, true);
					int valemill = deck.deck_get(mill);
					deck.deck_set(mill, 0);
					pl1.set_valemill_kart(valemill, true);
					pl1.efektset(11);
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 38;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
						efcontrol[i_s].p1d2 = valemill;
						efcontrol[i_s].p2d2 = valemill;
					}
				}
				break;
			case 12:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {
					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 39;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}
					pl1.efektset(12);
				}
				break;
			case 13:
				if (s_yerekoyvale_p1(i, atilankart, getfield) < 0) { break; }
				else {

					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 40;
						efcontrol[i_s].p1d1 = getfield;
						efcontrol[i_s].p2d1 = getfield + 3;
					}

					int dene = 0;
					for (int i = 0; i < 3; i++) {
						if (pl1.get_field(i).f == 1 && dene == 0) dene = 1;
					}

					if (dene == 1) {
						i_s = ef_bosbul();
						if (i_s < max_i_s) {
							efcontrol[i_s].clr();
							efcontrol[i_s].efekt = 27;
							efcontrol[i_s].p1d1 = 1;
						}
						for (int i = 0; i < 3; i++) {
							if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker == 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(pl1.get_field(i).f, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i, pl1.get_field(i).f); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
							if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker != 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(11, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i, 11); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
							if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker == 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(pl2.get_field(i).f, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, pl2.get_field(i).f); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
							if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker != 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(11, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, 11); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }

						}
						pl1.mezar_duzenle();
						pl2.mezar_duzenle();
						pl1.set_askral(1, true);
					}
				}
				break;
			}
		}
	}
	void s_valemill_p2(int atilankart, int getfield) {
		if(atilankart != 0){
		int i = 0;
		pl2.set_field_f(getfield, 0);
		pl2.set_field_joker(getfield, 1);
		switch (atilankart) {
		case 1:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 28;
					efcontrol[i_s].p1d1 = getfield + 3; 
					efcontrol[i_s].p2d1 = getfield;
				}
			}
			break;
		case 2:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 29;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
				pl2.efektset(2); }
			break;
		case 3:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 30;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
			}
			break;
		case 4:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 31;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
			}
			break;
		case 5:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 32;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
			}
			break;
		case 6:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 33;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
			}
			break;
		case 7:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 34;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
			}
			break;
		case 8:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 35;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
				if (pl2.get_field(getfield).tributeli == 1) {
					pl2.efektset(8);
				}
			}
			break;
		case 9:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 36;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
				if (pl2.get_field(getfield).tributeli == 1) {
					pl2.efektset(9);
				}
			}
			break;
		case 10:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 37;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
			}
			break;
		case 11:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				int mill = 0;
				while (deck.deck_get(mill) == 0 && mill < 51) mill++;
				int millmezar = 0;
				while (pl2.set_mezar(0, millmezar, false) != 0 && millmezar < 51) millmezar++;
				pl2.set_mezar(deck.deck_get(mill), millmezar, true);
				int valemill = deck.deck_get(mill);
				deck.deck_set(mill, 0);
				pl2.set_valemill_kart(valemill, true);
				pl2.efektset(11);

				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 38;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
					efcontrol[i_s].p1d2 = valemill;
					efcontrol[i_s].p2d2 = valemill;
				}
			}
			break;
		case 12:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {
				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 39;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}
				pl2.efektset(12);
			}
			break;
		case 13:
			if (s_yerekoyvale_p2(i, atilankart, getfield) < 0) { break; }
			else {

				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 40;
					efcontrol[i_s].p1d1 = getfield + 3;
					efcontrol[i_s].p2d1 = getfield;
				}

				int dene = 0;
				for (int i = 0; i < 3; i++) {
					if (pl2.get_field(i).f == 1 && dene == 0) dene = 1;
				}

				if (dene == 1) {

					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 27;
						efcontrol[i_s].p2d1 = 1;
					}

					for (int i = 0; i < 3; i++) {
						if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker == 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(pl2.get_field(i).f, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i, pl2.get_field(i).f); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
						if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker != 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(11, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i, 11); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
						if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker == 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(pl1.get_field(i).f, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, pl1.get_field(i).f); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
						if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker != 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(11, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, 11); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }

					}
					pl2.mezar_duzenle();
					pl1.mezar_duzenle();
					pl2.set_askral(1, true);
				}
			}
			break;
		}
	}
	}

	void s_efektler_p1(int olanefekt) {
		switch (olanefekt) {
			//22222222222222222222222222222222222222222222222222
		case 2:
			 bosyer = -1;
			 deste2 = -1;
			for (int i = 0; i < 3; i++) {
				if (pl1.get_field(i).f == 0 && bosyer == -1) {
					bosyer = i;
			}
			}
			if (bosyer >= 0) {
				for (int i = 0; i < 52; i++) {
					if (deck.deck_get(i) == 2 && deste2 == -1) {
						deste2 = i;
			}
			}
			}
			if (deste2 >= 0) {
				deck.deck_set(deste2, 0);
				pl1.set_field_f(bosyer, 2);
				//CL�ENTE B�LG� G�NDER

				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 16;
					efcontrol[i_s].p1d1 = pl1.set_field_secili(0, false);
					efcontrol[i_s].p2d1 = pl1.set_field_secili(0, false) + 3;
					efcontrol[i_s].p1d2 = bosyer;
					efcontrol[i_s].p2d2 = bosyer + 3;

				}

				deck.shuffle();
			}
			pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
			if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
			break;
			//333333333333333333333333333333333333333333333333
		case 3:
			if (pl1.set_uckullan(0, false) == 0) {
				//CL�ENTE B�LG� G�NDER

				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 17;

				}
				

				pl1.set_uckullan(1, true);
				int c1 = 0;
				int c2 = 0;
				for (int i = 0; i < 2; i++) {
					int b = -1;
					for (int v = 0; v < 10; v++) {
						if (pl1.set_el(0, v, false) == 0 && b == -1) {
							b = v;
						}
					}
					int v = 0;
					while (deck.deck_get(v) == 0 && v < 51) { v++; }
					if (deck.deck_get(v) == 0) pl1.set_birakt(pl1.set_birakt(0, false) + 1, true);
					pl1.set_el(deck.deck_get(v), b, true);
					if (i == 0) {
						c1 = deck.deck_get(v);
						if (i_s < max_i_s) {
							efcontrol[i_s].p1d3 = b;
						}
					}
					if (i == 1) {
						c2 = deck.deck_get(v);
						if (i_s < max_i_s) {
							efcontrol[i_s].p1d4 = b;
						}
					}
					deck.deck_set(v, 0);
				}
				if (i_s < max_i_s) {
					efcontrol[i_s].p1d1 = c1;
					efcontrol[i_s].p1d2 = c2;
				}


			}
				for (int i = 0; i < 10; i++) {
					pl1.set_oyna(svc.p1_elclick[i], i, true);
					if (pl1.set_oyna(0, i, false) == 1 && pl1.set_el(i, i, false) != 0) {
						//cliente geri g�nderme infosu
						i_s = ef_bosbul();
						if (i_s < max_i_s) {
							efcontrol[i_s].clr();
							efcontrol[i_s].efekt = 41;
							efcontrol[i_s].p1d1 = pl1.set_el(0, i, false);
							efcontrol[i_s].p1d2 = i;
							efcontrol[i_s].p1d3 = 1;
							efcontrol[i_s].p2d3 = 0;
						}

						if (deck.deck_get(0) == 0) deck.deck_set(0, pl1.set_el(i, i, false)); else deck.deck_set(1, pl1.set_el(i, i, false));
						pl1.set_el(0, i, true);
						if (pl1.set_birakt(0, false) < 2) pl1.set_birakt(pl1.set_birakt(0, false) + 1, true);
				}
				}
				if (pl1.set_birakt(0, false) == 2) {
					pl1.set_birakt(0, true);
					deck.shuffle();
					pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
					if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
					pl1.set_uckullan(0, true);
					
				}
			break;
			//4444444444444444444444444444444444444444444444444
		case 4:
			if (pl2.get_mezarsize() != 0) {
				pl1.set_dortefekt(true, true);
				
				int tik = svc.p1_mouserel;
				if (tik == 1) {
					/*
					int getx = svc.p1_mousex;
					int gety = svc.p1_mousey;
					int xsonuc = -1;
					int ysonuc = -1;
					int kartx = 0;
					for (int i = 0; i < 8; i++) {
						kartx = 94 + (i * 116);
						if (std::abs(getx - kartx) <= 43) {
							xsonuc = i;
			}
			}
					
					if (gety > 36 && gety < 157) ysonuc = 0;
					if (gety > 160 && gety < 281) ysonuc = 1;
					if (gety > 284 && gety <405) ysonuc = 2;
					if (gety > 408 && gety < 529) ysonuc = 3;
					int sonuc = ysonuc * 8 + xsonuc + (svc.p1_sayfa * 32);
					*/
					if (pl1.set_mezarsecili() < 53) {
						//burasi her yerde farkli
						int sonuc = pl1.set_mezarsecili();
						pl1.set_mezarsecili(55, true);
						if (pl2.set_mezar(0, sonuc, false) != 0) {
							//CL�ENTE �NFO GONDER

							i_s = ef_bosbul();
							if (i_s < max_i_s) {
								efcontrol[i_s].clr();
								efcontrol[i_s].efekt = 18;
								efcontrol[i_s].p1d1 = pl2.set_mezar(0, sonuc, false);
								efcontrol[i_s].p2d1 = pl2.set_mezar(0, sonuc, false);
								efcontrol[i_s].p1d2 = 1;
								efcontrol[i_s].p2d2 = 0;
							}

							pl1.set_mezar(pl2.set_mezar(0, sonuc, false), 51, true);
							pl2.set_mezar(0, sonuc, true);
							pl1.mezar_duzenle();
							pl2.mezar_duzenle();
							pl1.set_dortefekt(0, true);
							pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
							if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
							
						}
					}
				}
			}
			else {
				pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
				if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
			}
			break;
			//6666666666666666666666666666666666666666666666666666666
		case 6:
			if (pl1.get_mezarsize() != 0) { //FARKLI
				pl1.set_altiefekt(true, true); //FARKLI
				int tik = svc.p1_mouserel;
				if (tik == 1) {
					/*
					int getx = svc.p1_mousex;
					int gety = svc.p1_mousey;
					int xsonuc = -1;
					int ysonuc = -1;
					int kartx = 0;
					for (int i = 0; i < 8; i++) {
						kartx = 94 + (i * 116);
						if (std::abs(getx - kartx) <= 43) {
							xsonuc = i;
			}
			}

					if (gety > 36 && gety < 157) ysonuc = 0;
					if (gety > 160 && gety < 281) ysonuc = 1;
					if (gety > 284 && gety <405) ysonuc = 2;
					if (gety > 408 && gety < 529) ysonuc = 3;
					int sonuc = ysonuc * 8 + xsonuc + (svc.p1_sayfa * 32);
					*/
					if (pl1.set_mezarsecili() < 53) {
						//burasi her yerde farkli
						int sonuc = pl1.set_mezarsecili();
						pl1.set_mezarsecili(55, true);
						//burasi her yerde farkli
						if (pl1.set_mezar(0, sonuc, false) != 0) {
							//CL�ENTE B�LG� G�NDER

							i_s = ef_bosbul();
							if (i_s < max_i_s) {
								efcontrol[i_s].clr();
								efcontrol[i_s].efekt = 20;
								efcontrol[i_s].p1d1 = pl1.set_mezar(0, sonuc, false);
								efcontrol[i_s].p2d1 = pl1.set_mezar(0, sonuc, false);
								efcontrol[i_s].p1d2 = 1;
								efcontrol[i_s].p2d2 = 0;
								efcontrol[i_s].p1d3 = pl1.set_altisec(0, false);
							}

							pl1.set_el(pl1.set_mezar(0, sonuc, false), pl1.set_altisec(0, false), true);
							pl1.set_mezar(6, sonuc, true);
							pl1.set_altiefekt(0, true);
							pl1.set_altisec(0, true);
							pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
							if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
						}
					}
				}
			}
			else {
				pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
				if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
			}
			break;
			//7777777777777777777777777777777777777777
		case 7:
			if (pl2.get_mezarsize() != 0) { //FARKLI
				pl1.set_yediefekt(true, true); //FARKLI
				int tik = svc.p1_mouserel;
				if (tik == 1) {
					/*
					int getx = svc.p1_mousex;
					int gety = svc.p1_mousey;
					int xsonuc = -1;
					int ysonuc = -1;
					int kartx = 0;
					for (int i = 0; i < 8; i++) {
						kartx = 94 + (i * 116);
						if (std::abs(getx - kartx) <= 43) {
							xsonuc = i;
			}
			}

					if (gety > 36 && gety < 157) ysonuc = 0;
					if (gety > 160 && gety < 281) ysonuc = 1;
					if (gety > 284 && gety <405) ysonuc = 2;
					if (gety > 408 && gety < 529) ysonuc = 3;
					int sonuc = ysonuc * 8 + xsonuc + (svc.p1_sayfa * 32);
					*/
					if (pl1.set_mezarsecili() < 53) {
						//burasi her yerde farkli
						int sonuc = pl1.set_mezarsecili();
						pl1.set_mezarsecili(55, true);
						if (pl2.set_mezar(0, sonuc, false) != 0) {
							//CL�ENTE B�LG� G�NDER


							i_s = ef_bosbul();
							if (i_s < max_i_s) {
								efcontrol[i_s].clr();
								efcontrol[i_s].efekt = 21;
								efcontrol[i_s].p1d1 = pl2.set_mezar(0, sonuc, false);
								efcontrol[i_s].p2d1 = pl2.set_mezar(0, sonuc, false);
								efcontrol[i_s].p1d2 = 1;
								efcontrol[i_s].p2d2 = 0;
								efcontrol[i_s].p1d3 = pl1.set_yedisec(0, false);
							}



							pl1.set_el(pl2.set_mezar(0,sonuc,false), pl1.set_yedisec(0, false), true);
							pl2.set_mezar(7, sonuc, true);
							pl1.set_yediefekt(false, true);
							pl1.set_yedisec(0, true);
							pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
							if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);

						}
					}
				}
			}
			else {
				pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
				if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
			}
			break;
			//888888888888888888888888
		case 8:
			if (pl1.set_sekizkullan(false, false) == false) {
				pl1.set_sekizkullan(true,true);
				int fs = pl1.set_field_secili(0, false);
				int v1 = pl1.get_field(fs).tributeli;
				int v2 = pl1.get_field(fs).kapali;
				int v3 = pl1.get_field(fs).joker;
				pl1.set_field_tributeli(fs, pl2.get_field(fs).tributeli);
				pl1.set_field_kapali(fs, pl2.get_field(fs).kapali);
				pl1.set_field_joker(fs, pl2.get_field(fs).joker);
				pl1.set_field_f(fs, pl2.get_field(fs).f);
				pl1.set_field_saldir(fs, 1);

				pl2.set_field_tributeli(fs,v1);
				pl2.set_field_kapali(fs,v2);
				pl2.set_field_joker(fs,v3);
				pl2.set_field_f(fs,8);
				//cliente info ? 


				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 22;
					efcontrol[i_s].p1d1 = pl1.set_field_secili(0, false);
					efcontrol[i_s].p2d1 = pl1.set_field_secili(0, false) + 3;
					efcontrol[i_s].p1d2 = 1;
					efcontrol[i_s].p2d2 = 0;
					efcontrol[i_s].p1d3 = pl1.get_field(fs).f;
					efcontrol[i_s].p2d3 = pl1.get_field(fs).f;
				}


				pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
				if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);

			}
			else {
				pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
				if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
			}
			break;
			//999999999999999999999999999999999999BU �OK UZ�CAK AQ
		case 9:
			if (pl1.get_mezarsize() != 0) {
				if (pl1.set_dokuzefekt(false, false) == false) {
					for (int i = 0; i < 3; i++) {
						if (pl1.get_field(i).f == 0) {
							pl1.set_dokuzsummon(i, true);
							break;
						
						}
					}
					if (pl1.set_dokuzsummon(0, false) != -1) {
						pl1.set_dokuzefekt(true, true);
					}
					else {
						pl1.set_dokuzefekt(false, true);
						pl1.set_dokuzsummon(-1, true);
						pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
						if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
					}
				}
				if (pl1.set_dokuzefekt(0, false) == true) {
					int tik = svc.p1_mouserel;
					if (tik == 1) {
						/*
						int getx = svc.p1_mousex;
						int gety = svc.p1_mousey;
						int xsonuc = -1;
						int ysonuc = -1;
						int kartx = 0;
						for (int i = 0; i < 8; i++) {
							kartx = 94 + (i * 116);
							if (std::abs(getx - kartx) <= 43) {
								xsonuc = i;
				}
				}

						if (gety > 36 && gety < 157) ysonuc = 0;
						if (gety > 160 && gety < 281) ysonuc = 1;
						if (gety > 284 && gety <405) ysonuc = 2;
						if (gety > 408 && gety < 529) ysonuc = 3;
						int sonuc = ysonuc * 8 + xsonuc + (svc.p1_sayfa * 32);
						*/
						if (pl1.set_mezarsecili() < 53) {
							//burasi her yerde farkli
							int sonuc = pl1.set_mezarsecili();
							pl1.set_mezarsecili(55, true);
							if (pl1.set_mezar(0, sonuc, false) != 0) {
								//CL�ENTE B�LG� G�NDER

								i_s = ef_bosbul();
								if (i_s < max_i_s) {
									efcontrol[i_s].clr();
									efcontrol[i_s].efekt = 23;
									efcontrol[i_s].p1d1 = pl1.set_field_secili(0, false);
									efcontrol[i_s].p2d1 = pl1.set_field_secili(0, false) + 3;
									efcontrol[i_s].p1d2 = 1;
									efcontrol[i_s].p2d2 = 0;
									efcontrol[i_s].p1d3 = pl1.set_dokuzsummon(0, false);
									efcontrol[i_s].p2d3 = pl1.set_dokuzsummon(0, false);
									efcontrol[i_s].p1d4 = pl1.set_mezar(0, sonuc, false);
									efcontrol[i_s].p2d4 = pl1.set_mezar(0, sonuc, false);
								}


								pl1.set_field_f(pl1.set_dokuzsummon(0,false),pl1.set_mezar(0, sonuc, false));
								pl1.set_field_kapali(pl1.set_dokuzsummon(0, false),1);
								pl1.set_field_secili(pl1.set_dokuzsummon(0, false), true);
								std::cout << "line 1574" << std::endl;
								int dkval = pl1.set_mezar(0, sonuc, false);
								pl1.set_mezar(0, sonuc, true);
								pl1.set_dokuzefekt(false, true);
								pl1.set_dokuzsummon(-1, true);
								pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
								if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
								if (dkval == 2) {
									pl1.efektset(2);
								}
								if (dkval == 13) {
									bool dene = false;
									for (int i = 0; i < 3; i++) {
										if (pl1.get_field(i).f == 14) pl1.set_field_f(1, i);
										if (pl1.get_field(i).f == 1) {
											dene = true;
											break;
										}
									}
									if (dene == true) {

										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 27;
											efcontrol[i_s].p1d1 = 1;
										}

										for (int i = 0; i < 3; i++) {
											if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker == 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(pl1.get_field(i).f, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i, pl1.get_field(i).f); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
											if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker != 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(11, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i, 11); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
											if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker == 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(pl2.get_field(i).f, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, pl2.get_field(i).f); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
											if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker != 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(11, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, 11); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }

										}
										pl1.mezar_duzenle();
										pl2.mezar_duzenle();
										pl1.set_askral(1, true);
									}
								}
								if (dkval == 12) {
									pl1.efektset(12);
								}
								if (dkval == 11) {
									int mill = 0;
									while (deck.deck_get(mill) == 0 && mill < 51) mill++;
									int millmezar = 0;
									while (pl1.set_mezar(0, millmezar, false) != 0)millmezar++;
									pl1.set_mezar(deck.deck_get(mill), millmezar, true);
									int valemill = deck.deck_get(mill);
									deck.deck_set(mill, 0);
									pl1.set_valemill_kart(valemill, true);
									pl1.efektset(11);
								}
							}
						}
					}
				}
			}
						else {
						pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
						if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
			}
			break;
			//11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11
			case 11:
				//cliente info g�nder

				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 25;
					efcontrol[i_s].p1d1 = pl1.set_field_secili(0, false);
					efcontrol[i_s].p2d1 = pl1.set_field_secili(0, false) + 3;
					efcontrol[i_s].p1d2 = pl1.set_valemill_kart(0, false);
					efcontrol[i_s].p2d2 = pl1.set_valemill_kart(0, false);
				}



				pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
				if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
				s_valemill_p1(pl1.set_valemill_kart(0, false), pl1.set_field_secili(0, false));
				break;
			case 12:
				 bosyer = -1;
				 deste2 = -1;
				for (int t = 0; t < 3; t++) {
					if (pl1.get_field(t).f == 0) {
						bosyer = t;
						break;
					}
				}
				if (bosyer >= 0) {
					for (int t = 0; t < 52; t++) {
						if (deck.deck_get(t) == 11) {
							deste2 = t;
							break;
						}
					}
				}
				if (deste2 >= 0) {
					//CL�ENTE �NFO G�NDER 

					i_s = ef_bosbul();
					if (i_s < max_i_s) {
						efcontrol[i_s].clr();
						efcontrol[i_s].efekt = 26;
						efcontrol[i_s].p1d1 = pl1.set_field_secili(0, false);
						efcontrol[i_s].p2d1 = pl1.set_field_secili(0, false) + 3;
						efcontrol[i_s].p1d2 = bosyer;
						efcontrol[i_s].p2d2 = bosyer + 3;
					}


					

					deck.deck_set(deste2, 0);
					pl1.set_field_f(bosyer, 11);
					pl1.set_field_secili(bosyer, true);
					std::cout << "line 1679" << std::endl;
					deck.shuffle();

					int mill = 0;
					while (deck.deck_get(mill) == 0 && mill < 51) mill++;
					int millmezar = 0;
					while (pl1.set_mezar(0, millmezar, false) != 0)millmezar++;
					pl1.set_mezar(deck.deck_get(mill), millmezar, true);
					int valemill = deck.deck_get(mill);
					deck.deck_set(mill, 0);
					pl1.set_valemill_kart(valemill, true);
					pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
					if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
					pl1.efektset(11);
					

					if (i_s < max_i_s) {
						efcontrol[i_s].p1d3 = valemill;
						efcontrol[i_s].p2d3 = valemill;
					}
					
					
				}
				else {
					pl1.set_efektsira(0, pl1.set_efektoynat(0, false), true);
					if (pl1.set_efektoynat(0, false) > 0)pl1.set_efektoynat(pl1.set_efektoynat(0, false) - 1, true);
				}
				break;
}
}
	void s_efektler_p2(int olanefekt) {
	switch (olanefekt) {
		//22222222222222222222222222222222222222222222222222
	case 2:
		 bosyer = -1;
		 deste2 = -1;
		for (int i = 0; i < 3; i++) {
			if (pl2.get_field(i).f == 0 && bosyer == -1) {
				bosyer = i;
			}
		}
		if (bosyer >= 0) {
			for (int i = 0; i < 52; i++) {
				if (deck.deck_get(i) == 2 && deste2 == -1) {
					deste2 = i;
				}
			}
		}
		if (deste2 >= 0) {
			deck.deck_set(deste2, 0);
			pl2.set_field_f(bosyer, 2);
			//CL�ENTE B�LG� G�NDER

			i_s = ef_bosbul();
			if (i_s < max_i_s) {
				efcontrol[i_s].clr();
				efcontrol[i_s].efekt = 16;
				efcontrol[i_s].p1d1 = pl2.set_field_secili(0, false) + 3;
				efcontrol[i_s].p2d1 = pl2.set_field_secili(0, false);
				efcontrol[i_s].p1d2 = bosyer + 3;
				efcontrol[i_s].p2d2 = bosyer;

			}


			deck.shuffle();
		}
		pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
		if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
		break;
		//333333333333333333333333333333333333333333333333
	case 3:
		if (pl2.set_uckullan(0, false) == 0) {
			//CL�ENTE B�LG� G�NDER

			i_s = ef_bosbul();
			if (i_s < max_i_s) {
				efcontrol[i_s].clr();
				efcontrol[i_s].efekt = 17;

			}

			pl2.set_uckullan(1, true);
			int c1 = 0;
			int c2 = 0;
			for (int i = 0; i < 2; i++) {
				int b = -1;
				for (int v = 0; v < 10; v++) {
					if (pl2.set_el(0, v, false) == 0 && b == -1) {
						b = v;
					}
				}
				int v = 0;
				while (deck.deck_get(v) == 0 && v < 51) { v++; }
				if (deck.deck_get(v) == 0) pl2.set_birakt(pl2.set_birakt(0, false) + 1, true);
				pl2.set_el(deck.deck_get(v), b, true);
				if (i == 0) {
					c1 = deck.deck_get(v);
					if (i_s < max_i_s) {
						efcontrol[i_s].p2d3 = b;
					}
				}
				if (i == 1) {
					c2 = deck.deck_get(v);
					if (i_s < max_i_s) {
						efcontrol[i_s].p2d4 = b;
					}
				}
				deck.deck_set(v, 0);
			}
			if (i_s < max_i_s) {
				efcontrol[i_s].p2d1 = c1;
				efcontrol[i_s].p2d2 = c2;
			}
		}
		for (int i = 0; i < 10; i++) {
			pl2.set_oyna(svc.p2_elclick[i], i, true);
			if (pl2.set_oyna(0, i, false) == 1 && pl2.set_el(i, i, false) != 0) {
				//cliente geri verme infosu

				i_s = ef_bosbul();
				if (i_s < max_i_s) {
					efcontrol[i_s].clr();
					efcontrol[i_s].efekt = 41;
					efcontrol[i_s].p2d1 = pl2.set_el(0, i, false);
					efcontrol[i_s].p2d2 = i;
					efcontrol[i_s].p2d3 = 1;
					efcontrol[i_s].p1d3 = 0;
				}

				if (deck.deck_get(0) == 0) deck.deck_set(0, pl2.set_el(i, i, false)); else deck.deck_set(1, pl2.set_el(i, i, false));
				pl2.set_el(0, i, true);
				if (pl2.set_birakt(0, false) < 2) pl2.set_birakt(pl2.set_birakt(0, false) + 1, true);
			}
		}
		if (pl2.set_birakt(0, false) == 2) {
			pl2.set_birakt(0, true);
			deck.shuffle();
			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
			pl2.set_uckullan(0, true);
		}
		break;
		//4444444444444444444444444444444444444444444444444
	case 4:
		if (pl1.get_mezarsize() != 0) {
			pl2.set_dortefekt(true, true);
			int tik = svc.p2_mouserel;
			if (tik == 1) {
				if (pl2.set_mezarsecili() < 53) {
					//burasi her yerde farkli
					int sonuc = pl2.set_mezarsecili();
					pl2.set_mezarsecili(55, true);
					//burasi her yerde farkli
					if (pl1.set_mezar(0, sonuc, false) != 0) {
						//CL�ENTE �NFO GONDER

						i_s = ef_bosbul();
						if (i_s < max_i_s) {
							efcontrol[i_s].clr();
							efcontrol[i_s].efekt = 18;
							efcontrol[i_s].p2d1 = pl1.set_mezar(0, sonuc, false);
							efcontrol[i_s].p1d1 = pl1.set_mezar(0, sonuc, false);
							efcontrol[i_s].p2d2 = 1;
							efcontrol[i_s].p1d2 = 0;
						}

						pl2.set_mezar(pl1.set_mezar(0, sonuc, false), 51, true);
						pl1.set_mezar(0, sonuc, true);
						pl2.mezar_duzenle();
						pl1.mezar_duzenle();
						pl2.set_dortefekt(0, true);
						pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
						if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);

					}
				}
			}
		}
		else {
			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
		}
		break;
		//6666666666666666666666666666666666666666666666666666666
	case 6:
		if (pl2.get_mezarsize() != 0) { //FARKLI
			pl2.set_altiefekt(true, true); //FARKLI
			int tik = svc.p2_mouserel;
			if (tik == 1) {
				if (pl2.set_mezarsecili() < 53) {
					//burasi her yerde farkli
					int sonuc = pl2.set_mezarsecili();
					pl2.set_mezarsecili(55, true);
					//burasi her yerde farkli
					if (pl2.set_mezar(0,sonuc, false) != 0) {
						//CL�ENTE B�LG� G�NDER

						i_s = ef_bosbul();
						if (i_s < max_i_s) {
							efcontrol[i_s].clr();
							efcontrol[i_s].efekt = 20;
							efcontrol[i_s].p2d1 = pl2.set_mezar(0, sonuc, false);
							efcontrol[i_s].p1d1 = pl2.set_mezar(0, sonuc, false);
							efcontrol[i_s].p2d2 = 1;
							efcontrol[i_s].p1d2 = 0;
							efcontrol[i_s].p2d3 = pl2.set_altisec(0, false);
						}

						pl2.set_el(pl2.set_mezar(0, sonuc, false), pl2.set_altisec(0, false), true);
						pl2.set_mezar(6, sonuc, true);
						pl2.set_altiefekt(0, true);
						pl2.set_altisec(0, true);
						pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
						if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
					}
				}
			}
		}
		else {
			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
		}
		break;
		//7777777777777777777777777777777777777777
	case 7:
		if (pl1.get_mezarsize() != 0) { //FARKLI
			pl2.set_yediefekt(true, true); //FARKLI
			int tik = svc.p2_mouserel;
			if (tik == 1) {
				if (pl2.set_mezarsecili() < 53) {
					//burasi her yerde farkli
					int sonuc = pl2.set_mezarsecili();
					pl2.set_mezarsecili(55, true);
					if (pl1.set_mezar(0, sonuc, false) != 0) {
						//CL�ENTE B�LG� G�NDER

						i_s = ef_bosbul();
						if (i_s < max_i_s) {
							efcontrol[i_s].clr();
							efcontrol[i_s].efekt = 21;
							efcontrol[i_s].p2d1 = pl1.set_mezar(0, sonuc, false);
							efcontrol[i_s].p1d1 = pl1.set_mezar(0, sonuc, false);
							efcontrol[i_s].p2d2 = 1;
							efcontrol[i_s].p1d2 = 0;
							efcontrol[i_s].p2d3 = pl2.set_yedisec(0, false);
						}

						pl2.set_el(pl1.set_mezar(0, sonuc, false), pl2.set_yedisec(0, false), true);
						pl1.set_mezar(7, sonuc, true);
						pl2.set_yediefekt(false, true);
						pl2.set_yedisec(0, true);
						pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
						if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);

					}
				}
			}
		}
		else {
			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
		}
		break;
		//888888888888888888888888
	case 8:
		if (pl2.set_sekizkullan(false, false) == false) {
			pl2.set_sekizkullan(true, true);
			int fs = pl2.set_field_secili(0, false);
			int v1 = pl2.get_field(fs).tributeli;
			int v2 = pl2.get_field(fs).kapali;
			int v3 = pl2.get_field(fs).joker;
			pl2.set_field_tributeli(fs, pl1.get_field(fs).tributeli);
			pl2.set_field_kapali(fs, pl1.get_field(fs).kapali);
			pl2.set_field_joker(fs, pl1.get_field(fs).joker);
			pl2.set_field_f(fs, pl1.get_field(fs).f);
			pl2.set_field_saldir(fs, 1);

			pl1.set_field_tributeli(fs,v1);
			pl1.set_field_kapali(fs,v2);
			pl1.set_field_joker(fs,v3);
			pl1.set_field_f(fs,8);
			//cliente info?

			i_s = ef_bosbul();
			if (i_s < max_i_s) {
				efcontrol[i_s].clr();
				efcontrol[i_s].efekt = 22;
				efcontrol[i_s].p2d1 = pl2.set_field_secili(0, false);
				efcontrol[i_s].p1d1 = pl2.set_field_secili(0, false) + 3;
				efcontrol[i_s].p2d2 = 1;
				efcontrol[i_s].p1d2 = 0;
				efcontrol[i_s].p2d3 = pl2.get_field(fs).f;
				efcontrol[i_s].p1d3 = pl2.get_field(fs).f;
			}

			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);

		}
		else {
			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
		}
		break;
		//999999999999999999999999999999999999BU �OK UZ�CAK AQ
	case 9:
		if (pl2.get_mezarsize() != 0) {
			if (pl2.set_dokuzefekt(false, false) == false) {
				for (int i = 0; i < 3; i++) {
					if (pl2.get_field(i).f == 0) {
						pl2.set_dokuzsummon(i, true);
						break;

					}
				}
				if (pl2.set_dokuzsummon(0, false) != -1) {
					pl2.set_dokuzefekt(true, true);
				}
				else {
					pl2.set_dokuzefekt(false, true);
					pl2.set_dokuzsummon(-1, true);
					pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
					if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
				}
			}
			if (pl2.set_dokuzefekt(0, false) == true) {
				int tik = svc.p2_mouserel;
				if (tik == 1) {
					if (pl2.set_mezarsecili() < 53) {
						//burasi her yerde farkli
						int sonuc = pl2.set_mezarsecili();
						pl2.set_mezarsecili(55, true);
						if (pl2.set_mezar(0, sonuc, false) != 0) {
							//CL�ENTE B�LG� G�NDER

							i_s = ef_bosbul();
							if (i_s < max_i_s) {
								efcontrol[i_s].clr();
								efcontrol[i_s].efekt = 23;
								efcontrol[i_s].p2d1 = pl2.set_field_secili(0, false);
								efcontrol[i_s].p1d1 = pl2.set_field_secili(0, false) + 3;
								efcontrol[i_s].p2d2 = 1;
								efcontrol[i_s].p1d2 = 0;
								efcontrol[i_s].p2d3 = pl2.set_dokuzsummon(0, false);
								efcontrol[i_s].p1d3 = pl2.set_dokuzsummon(0, false);
								efcontrol[i_s].p2d4 = pl2.set_mezar(0, sonuc, false);
								efcontrol[i_s].p1d4 = pl2.set_mezar(0, sonuc, false);
							}

							pl2.set_field_f(pl2.set_dokuzsummon(0, false), pl2.set_mezar(0, sonuc, false));
							pl2.set_field_kapali(pl2.set_dokuzsummon(0, false), 1);
							pl2.set_field_secili(pl2.set_dokuzsummon(0, false), true);
							std::cout << "line 1964" << std::endl;
							int dkval = pl2.set_mezar(0, sonuc, false);
							pl2.set_mezar(0, sonuc, true);
							pl2.set_dokuzefekt(false, true);
							pl2.set_dokuzsummon(-1, true);
							pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
							if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
							if (dkval == 2) {
								pl2.efektset(2);
							}
							if (dkval == 13) {
								bool dene = false;
								for (int i = 0; i < 3; i++) {
									if (pl2.get_field(i).f == 14) pl2.set_field_f(1, i);
									if (pl2.get_field(i).f == 1) {
										dene = true;
										break;
									}
								}
									//CL�ENTE B�LG� G�NDER
									//KRAL ���N
									if (dene == true) {
										i_s = ef_bosbul();
										if (i_s < max_i_s) {
											efcontrol[i_s].clr();
											efcontrol[i_s].efekt = 27;
											efcontrol[i_s].p2d1 = 1;
										}

										for (int i = 0; i < 3; i++) {
											if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker == 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(pl2.get_field(i).f, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i, pl2.get_field(i).f); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
											if (pl2.get_field(i).f != 13 && pl2.get_field(i).joker != 0) { if (pl2.get_field(i).kapali == 0) { pl2.set_mezar(11, 51 - i, true); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); } else { deck.deck_set(i, 11); pl2.set_field_f(i, 0); pl2.set_field_tributeli(i, 0); pl2.set_field_joker(i, 0); pl2.set_field_kapali(i, 0); deck.shuffle(); } }
											if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker == 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(pl1.get_field(i).f, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, pl1.get_field(i).f); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }
											if (pl1.get_field(i).f != 13 && pl1.get_field(i).joker != 0) { if (pl1.get_field(i).kapali == 0) { pl1.set_mezar(11, 51 - i, true); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); } else { deck.deck_set(i + 3, 11); pl1.set_field_f(i, 0); pl1.set_field_tributeli(i, 0); pl1.set_field_joker(i, 0); pl1.set_field_kapali(i, 0); deck.shuffle(); } }

										}
										pl2.mezar_duzenle();
										pl1.mezar_duzenle();
										pl2.set_askral(1, true);
									}
							}
							if (dkval == 12) {
								pl2.efektset(12);
							}
							if (dkval == 11) {
								int mill = 0;
								while (deck.deck_get(mill) == 0 && mill < 51) mill++;
								int millmezar = 0;
								while (pl2.set_mezar(0, millmezar, false) != 0)millmezar++;
								pl2.set_mezar(deck.deck_get(mill), millmezar, true);
								int valemill = deck.deck_get(mill);
								deck.deck_set(mill, 0);
								pl2.set_valemill_kart(valemill, true);
								pl2.efektset(11);
							}
						}
					}
				}
			}
		}
		else {
			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
		}
		break;
		//11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11 11
	case 11:
		//cliente info g�nder
		
		i_s = ef_bosbul();
		if (i_s < max_i_s) {
			efcontrol[i_s].clr();
			efcontrol[i_s].efekt = 25;
			efcontrol[i_s].p2d1 = pl2.set_field_secili(0, false);
			efcontrol[i_s].p1d1 = pl2.set_field_secili(0, false) + 3;
			efcontrol[i_s].p2d2 = pl2.set_valemill_kart(0, false);
			efcontrol[i_s].p1d2 = pl2.set_valemill_kart(0, false);
		}

		pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
		if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
		s_valemill_p2(pl2.set_valemill_kart(0, false), pl2.set_field_secili(0, false));
		break;
	case 12:
		 bosyer = -1;
		 deste2 = -1;
		for (int t = 0; t < 3; t++) {
			if (pl2.get_field(t).f == 0) {
				bosyer = t;
				break;
			}
		}
		if (bosyer >= 0) {
			for (int t = 0; t < 52; t++) {
				if (deck.deck_get(t) == 11) {
					deste2 = t;
					break;
				}
			}
		}
		if (deste2 >= 0) {
			//CL�ENTE �NFO G�NDER 

			i_s = ef_bosbul();
			if (i_s < max_i_s) {
				efcontrol[i_s].clr();
				efcontrol[i_s].efekt = 26;
				efcontrol[i_s].p2d1 = pl2.set_field_secili(0, false);
				efcontrol[i_s].p1d1 = pl2.set_field_secili(0, false) + 3;
				efcontrol[i_s].p2d2 = bosyer;
				efcontrol[i_s].p1d2 = bosyer + 3;
			}

			deck.deck_set(deste2, 0);
			pl2.set_field_f(bosyer, 11);
			pl2.set_field_secili(bosyer, true);
			std::cout << "line 2069" << std::endl;
			deck.shuffle();

			int mill = 0;
			while (deck.deck_get(mill) == 0 && mill < 51) mill++;
			int millmezar = 0;
			while (pl2.set_mezar(0, millmezar, false) != 0)millmezar++;
			pl2.set_mezar(deck.deck_get(mill), millmezar, true);
			int valemill = deck.deck_get(mill);
			deck.deck_set(mill, 0);
			pl2.set_valemill_kart(valemill, true);
			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
			pl2.efektset(11);
			if (i_s < max_i_s) {
				efcontrol[i_s].p1d3 = valemill;
				efcontrol[i_s].p2d3 = valemill;
			}
		}
		else {
			pl2.set_efektsira(0, pl2.set_efektoynat(0, false), true);
			if (pl2.set_efektoynat(0, false) > 0)pl2.set_efektoynat(pl2.set_efektoynat(0, false) - 1, true);
		}
		break;
	}
}
	c_serverkontrol returnsvc() {
	return svc;
	}

c_player getpl1() {
	return pl1;
}

c_player getpl2() {
	return pl2;
}
	c_serverkontrol svc;

	char* bufgonder(c_player o1, c_player o2, c_s_top cstop1[], int oy1, int oy2,int sa,int pk) {
		unsigned char ret[4096];
		ZeroMemory(ret, 4096);
		ret[0] = 1;
		ret[1] = tk.get_tur();
		ret[2] = tk.get_tur_durum();
		ret[3] = tk.r_oyit();
		ret[4] = o1.set_can(0, false);
		ret[5] = o1.set_battlesec(0, false);
		ret[6] = o1.set_battlesecrakip(0, false);
		ret[7] = o1.set_dokuzefekt(0,false);
		ret[8] = o1.set_dortefekt(0, false);
		ret[9] = o1.set_yediefekt(0, false);
		ret[10] = o1.set_altiefekt(0, false);
		int p = 11;
		for (int i = 0; i < 3; i++) {
			ret[p] = o1.get_field(i).f;
			p++;
		}
		for (int i = 0; i < 3; i++) {
			ret[p] = o1.get_field(i).kapali;
			p++;
		}
		for (int i = 0; i < 3; i++) {
			ret[p] = o1.get_field(i).tributeli;
			p++;
		}
		for (int i = 0; i < 3; i++) {
			ret[p] = o1.get_field(i).joker;
			p++;
		}
		for (int i = 0; i < 3; i++) {
			ret[p] = o1.get_field(i).saldir;
			p++;
		}
		ret[p] = o1.set_field_secili(0, false);
		p++;
		for (int i = 0; i < 3; i++) {
			ret[p] = o2.get_field(i).f;
			p++;
		}
		for (int i = 0; i < 3; i++) {
			ret[p] = o2.get_field(i).kapali;
			p++;
		}
		for (int i = 0; i < 3; i++) {
			ret[p] = o2.get_field(i).tributeli;
			p++;
		}
		for (int i = 0; i < 3; i++) {
			ret[p] = o2.get_field(i).joker;
			p++;
		}
		for (int i = 0; i < 3; i++) {
			ret[p] = o2.get_field(i).saldir;
			p++;
		}

	//**************************************************************
		for (int i = 0; i < 10; i++) {
			ret[p] = o1.set_el(0, i, false);
			p++;
		}
		for (int i = 0; i < 52; i++) {
			while (o1.set_mezar(0, i, false) > 13) { o1.set_mezar(o1.set_mezar(0, i, false) - 13, i, true); }
			ret[p] = o1.set_mezar(0, i, false);
			p++;
		}
		for (int i = 0; i < 52; i++) {
			while (o2.set_mezar(0, i, false) > 13) { o2.set_mezar(o2.set_mezar(0, i, false) - 13, i, true); }
			ret[p] = o2.set_mezar(0, i, false);
			p++;
		}
		int els = 0;
		for (int i = 0; i < 10; i++) {
			if (o2.set_el(0, i, false) != 0) els++;
		}
		ret[p] = els;
		p++;
		ret[p] = o2.set_battlesec(0, false);
		p++;
		ret[p] = o2.set_battlesecrakip(0, false);
		p++;
		ret[p] = o2.set_can(0, false);
		p++;
		ret[p] = sa;
		p++;
		int dsize = 0;
		for (int i = 0; i < 52; i++) {
			if (deck.deck_get(i) != 0) {
				dsize++;
			}
		}
		ret[p] = dsize;
		p++;
		for (int i = 0; i < max_i_s; i++) {
			if (pk == 0) {
				ret[p] = efcontrol[i].efekt;
			}
			else {
				ret[p] = 0;
			}
			p++;
		}
		if (sa == 1) {
			for (int i = 0; i < max_i_s; i++) {
				if (pk == 0) {
					ret[p] = efcontrol[i].p1d1;
					p++;
					ret[p] = efcontrol[i].p1d2;
					p++;
					ret[p] = efcontrol[i].p1d3;
					p++;
					ret[p] = efcontrol[i].p1d4;
					p++;
				}
				else {
					ret[p] = 0;
					p++;
					ret[p] = 0;
					p++;
					ret[p] = 0;
					p++;
					ret[p] = 0;
					p++;
				}
			}
		}
		if (sa == 2) {
			for (int i = 0; i < max_i_s; i++) {
				if (pk == 0) {
					ret[p] = efcontrol[i].p2d1;
					p++;
					ret[p] = efcontrol[i].p2d2;
					p++;
					ret[p] = efcontrol[i].p2d3;
					p++;
					ret[p] = efcontrol[i].p2d4;
					p++;
				}
				else {
					ret[p] = 0;
					p++;
					ret[p] = 0;
					p++;
					ret[p] = 0;
					p++;
					ret[p] = 0;
					p++;
				}

			}
		}
		ret[p] = svc.karla;
		
		svc.karla = 0;

		return (char*)&ret;
	}
	int setployuntur(int player, int set = 0, bool setle = 0) {
		if (player == 1) {
			pl1.set_oyunturu(set, setle);
			return pl1.set_oyunturu();
		}
		if (player == 2) {
			pl2.set_oyunturu(set, setle);
			return pl2.set_oyunturu();
		}
	}
	void tk_tur_arttir(int arttir, bool ok) {
		tk.tur_arttir(arttir, ok);
	}

	int oyunbit_kontrol(c_player o1, c_player o2, c_s_top cstop1[], int oy1, int oy2) {
		//return : 0 = bi�i yok / 1 = pl1 sava�la kazand� / 2 = pl2 sava�la kazand� / 3 = pl1 3 as yaparak kazand� / 4 = pl2 3 az yaparak kazand�
		if (pl1.set_can(0, false) == 0) {
			return 2;
		}
		if (pl2.set_can(0, false) == 0) {
			return 1;
		}
		int sayy = 0;
		for (int i = 0; i < 52; i++) {
			if (pl1.set_mezar(0, i, false) == 1 || pl1.set_mezar(0, i, false) == 14 || pl1.set_mezar(0, i, false) == 27) {
				sayy++;
			}
		}
		if (sayy >= 3) {
			return 3;
		}


		sayy = 0;
		for (int i = 0; i < 52; i++) {
			if (pl2.set_mezar(0, i, false) == 1 || pl2.set_mezar(0, i, false) == 14 || pl2.set_mezar(0, i, false) == 27) {
				sayy++;
			}
		}
		if (sayy >= 3) {
			return 4;
		}

		return 0;
	}

	void setpl1replay(int i) {
		svc.set_oyuntekrarp1(i);
	}

	void setpl2replay(int i) {
		svc.set_oyuntekrarp2(i);
	}

protected:
	int bosyer = -1;
	int deste2 = -1;
	int player1;
	int player2;
	bool bosmu;
	c_player pl1;
	c_player pl2;
	c_deste deck;
	c_tur_kontrol tk;
	

};


void f_pair(c_s_top c1, c_s_top c2, int p1, int p2, c_oyunbilgisi oyunbil1) {
	//c1 ve c2yi e�le�tir
	c1.set_oyunda(true);
	c2.set_oyunda(true);
	std::cout << "OLD " << oyunbil1.get_bos() << std::endl;
	oyunbil1.set_bos(false);
	std::cout << "NEW " << oyunbil1.get_bos() << std::endl;
	oyunbil1.set_bos(false);
	//random
	auto sa = std::chrono::system_clock::now();
	std::time_t saakso = std::chrono::system_clock::to_time_t(sa);
	srand(saakso);
	int getr = rand() % 2;
	if (getr == 0) {
		oyunbil1.set_player(1, p1);
		oyunbil1.set_player(2, p2);
	}
	else {
		oyunbil1.set_player(1, p2);
		oyunbil1.set_player(2, p1);
	}

}


void f_pair2(c_s_top c1[], c_s_top c2[], int p1, int p2, c_oyunbilgisi oyunbil1[],int o1) {
	//c1 ve c2yi e�le�tir
	c1[p1].set_oyunda(true);
	c2[p2].set_oyunda(true);
	std::cout << "OLD " << oyunbil1[o1].get_bos() << std::endl;
	oyunbil1[o1].set_bos(false);
	oyunbil1[o1].f_setup();
	//random
	auto sa = std::chrono::system_clock::now();
	std::time_t saakso = std::chrono::system_clock::to_time_t(sa);
	srand(saakso);
	int getr = rand() % 2;
	if (getr == 0) {
		oyunbil1[o1].set_player(1, p1);
		oyunbil1[o1].set_player(2, p2);
	}
	else {
		oyunbil1[o1].set_player(1, p2);
		oyunbil1[o1].set_player(2, p1);
	}

}

bool f_fd_cikar(SOCKET soc, c_s_top cst[], c_oyunbilgisi ob1[], int arysize,fd_set* set) {
	bool sa = false;
	for (int i = 0; i < arysize; i++) {
		if (cst[i].sock == soc) {
			for (int p = 0; p < arysize; p++) {
				if (i == ob1[p].get_player(1) || i == ob1[p].get_player(2)) {
					if (i == ob1[p].get_player(1)) {
						cst[ob1[p].get_player(2)].oyunda = false;
						sa = true;
					}
					if (i == ob1[p].get_player(2)) {
						cst[ob1[p].get_player(1)].oyunda = false;
						sa = true;
					}
					ob1[p].f_clr();
					cst[i].f_fd_clr(set);
					std::cout << " OB " << p << " dagitildi" <<std::endl;
				}

			}
		}

	}
	return sa;
}
/*
class c_oyunbilgisi {
public:
	void f_karehasapla() {

	}
};*/