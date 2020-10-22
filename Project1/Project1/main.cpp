#include <iostream>
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#include <Windows.h>
#include <time.h>
#include "hdene.h"
#include "oynat.h"
#pragma comment(lib,"ws2_32.lib")

/*
	KAREYÝ HESAPLA

	CLÝENTLERDEN BÝLGÝ AL

	CLÝENTLERE BÝLGÝ GÖNDER

*/


const int maxtop = 20;
c_s_top cstop[maxtop];
c_oyunbilgisi oyunbil[maxtop];
void main() {
	int end = 0;

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, 15);
	for (int i = 0; i < maxtop; i++) {
		oyunbil[i].set_player(1, -1);
		oyunbil[i].set_player(2, -1);
		oyunbil[i].getpl1().set_oyunturu(3, true);
		oyunbil[i].getpl2().set_oyunturu(6, true);
		oyunbil[i].set_bos(true);
	}
	//winsoku baþlat
	WSADATA wsdata;
	WORD ver = MAKEWORD(2, 2);
	int wsok = WSAStartup(ver,&wsdata);
	if (wsok != 0) {
		std::cerr << "winsock basalatilamiyor. Cikiliyor" << std::endl;
		return;
	}

	//socket oluþtur
	SOCKET listening = socket(AF_INET,SOCK_STREAM,0);
	if (listening == INVALID_SOCKET) {
		std::cerr << "socket oluþturulamadý. Cikiliyor" << std::endl;
		return;
	}


	//socketi ip adresine ve porta baðla
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening,(sockaddr*)&hint,sizeof(hint));

	//winsocka socketin dinlediðini söyle
	listen(listening, SOMAXCONN);

	fd_set master;
	FD_ZERO(&master);
	FD_SET(listening, &master);

	int p1ge = 0;
	int p2ge = 0;

	while (true) { //burayý true de yapabilirim ya da end == 0

		/*
		BURDA OYUNCU KONTROLÜ YAPICAZ
		*/
		int boscsay[2];
		boscsay[0] = -1;
		boscsay[1] = -1;
		int boscsay1 = 0;
		int boscsay2 = -1;
		for (int i = 0; i < maxtop; i++) {
			if (cstop[i].is_oyunda() == false && cstop[i].sock != NULL) {
				if (boscsay1 == 1) {
					boscsay1 = 2;
					boscsay[1] = i;
				}
				if (boscsay1 == 0) {
					boscsay1 = 1;
					boscsay[0] = i;
				}

			}
			if (boscsay2 == -1 && oyunbil[i].get_bos()==true) {
				boscsay2 = i;

			}
	}
		if (boscsay1 == 2 && boscsay2 != -1) {
			//std::cout << "OLD2 " << oyunbil[0].get_bos() << std::endl;
			f_pair2(cstop, cstop, boscsay[0], boscsay[1], oyunbil,boscsay2);
			oyunbil[boscsay2].f_setup();
			std::cout << " CLIENT " << boscsay[0] << " ve " << boscsay[1] << " oyunbil " << boscsay2 << " ile eslestirildi" << std::endl;
			std::cout << " CLIENT " << boscsay[0] << " SOCKET : " << cstop[oyunbil[boscsay2].get_player(1)].sock << " CLIENT " << boscsay[1] << " SOCKET : " << cstop[oyunbil[boscsay2].get_player(2)].sock << std::endl;
			//oyunbil[boscsay2].getpl1().set_oyunturu(3, true);
			oyunbil[boscsay2].setployuntur(1, 3, true);
			std::cout<<"ne baoktan is aq  "<< oyunbil[boscsay2].setployuntur(1)<<std::endl;
			oyunbil[boscsay2].setployuntur(2, 6, true);
		}
		unsigned char ggsin[16];
		ZeroMemory(ggsin, 16);
		if (oyunbil[0].get_bos() == false) {
			uint16_t s = 4;
			ggsin[0] = s;
			//send(cstop[oyunbil[0].get_player(1)].sock, (char*)&ggsin, 1, 0);
			s = 5;
			ggsin[0] = s;
			//send(cstop[oyunbil[0].get_player(2)].sock, (char*)&ggsin, 1, 0);
		}
		//
		//std::cout << "NEW2 " << oyunbil[0].get_bos() << std::endl;
		//oyunbil[0].set_bos(false);
		//std::cout << "NEW3 " << oyunbil[0].get_bos() << std::endl;
		//std::cout << "CLIENT 0 oynda = " << cstop[0].is_oyunda() << std::endl;
		/*
		YAPTIK
		*/
		
		/*
		BURDA KAREYÝ HESAPLÝCAZ
		
		*/
		//std::cout << " OYUN: 1 OYUNCU: 1 MOUSEX: " << oyunbil[1].svc.p1_mousex << std::endl;
		for (int k = 0; k < maxtop; k++){
			if (oyunbil[k].get_bos() == false) {
				if (oyunbil[k].get_player(1) != -1 && oyunbil[k].get_player(2) != -1 && oyunbil[k].get_tk().get_tur() == 0) {
					oyunbil[k].tk_tur_arttir(1, true);
					std::cout << "NE SIKTEN IS BU BOYLE AQ " << std::endl;
				}
				oyunbil[k].f_karehasapla(cstop);
				for (int i = 0; i < 52; i++) {
					//std::cout << oyunbil[k].g_deck().deck_get(i)<<" ";
				}
				//std::cout << std::endl;
				SetConsoleTextAttribute(hConsole, 14);
				// std::cout << " TUR: "<< oyunbil[k].get_tk().get_tur()<<" " << oyunbil[k].get_tk().get_tur_durum() << std::endl;
				SetConsoleTextAttribute(hConsole, 15);
				if (oyunbil[k].get_tk().get_tur() != 0 && oyunbil[k].get_tk().get_tur_durum() == 0) {
					oyunbil[k].tk_tur_arttir(1, true);
				}
			}
		}

		


		/*
		
		*/


		fd_set copy = master;
		timeval tval;
		tval.tv_sec = 5;
		tval.tv_usec = 0;
		int socketcount = select(0, &copy, nullptr, nullptr, &tval);
		
		for (int i = 0; i < socketcount; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == listening) {
				//yeni baðlantý kabul et
				sockaddr_in csize;
				int csizesz = sizeof(csize);
				SOCKET client = accept(listening, (sockaddr*)&csize, &csizesz);
				//f_s_bosbul(20, cstop, true).setsock(client); //BUNA GEREK YOK Ý DAHA ÝÝ ÇALIÞIYO
				cstop[f_s_ibosbul(maxtop, cstop)].setsock(client);
				std::cout <<"client " <<client << std::endl;
				//listeye yeni client ekle
				FD_SET(client, &master);
				
				//

			}
			else 
			{
				//yeni mesaj kabul et
				char buf[4096];
				ZeroMemory(buf, 4096);
				int bytesin = recv(sock, buf, 4096,0);
				if (bytesin <= 0) {
					//drop the client
					end = 1;
					if (f_fd_cikar(sock, cstop, oyunbil, maxtop, &master)) {
						std::cout << "SAAA";
					}
					else {
						std::cout << "FAK";
						/*closesocket(sock);
						FD_CLR(sock, &master);*/
						for (int i = 0; i < maxtop; i++) {
							if (cstop[i].sock == sock) {
								cstop[i].f_fd_clr(&master);
								break;
							}
						}
					}
				}
				else {
					bool oyundami = false;
					int i1 = -1;
					int i2 = -1;
					for (int i = 0; i < maxtop; i++) {
						if (cstop[oyunbil[i].get_player(1)].sock == sock) {
							i1 = 1;
							i2 = i;
							break;
						}
						if (cstop[oyunbil[i].get_player(2)].sock == sock) {
							i1 = 2;
							i2 = i;
							break;
						}
					}
					int chtr = -1;
					if (i1 == 1 && i2 != -1 && oyunbil[i2].get_player(2) != -1) {//player 1 için al
						oyundami = true;
						okub = 13;
						//std::cout << " ANNEN " << std::endl;
						//std::cout << i1 << " " << i2 << std::endl;
						//std::cout << std::endl << ":::::" << std::endl;
						int msgid = okubit(okub, "buffer_u8", buf, false).gelenint;
						//std::cout << std::endl;
						int sendm = 0;
						int s = 0;

						switch (msgid) {
						case 1:
							//std::cout << std::endl;// << "sayda: ";
							oyunbil[i2].svc.p1_sayfa = okubit(okub, "buffer_u8", buf, false).gelenint;
							s = okubit(okub, "buffer_u8", buf, false).gelenint;
							if (s == 1) {
								p1ge = 1;
							}
							s = okubit(okub, "buffer_u8", buf, false).gelenint;
							if (oyunbil[i2].get_tk().get_tur_durum() == 4 && oyunbil[i2].get_tk().get_tur() != 4) {

								oyunbil[i2].settksaldirkey(s);
							}

							s = okubit(okub, "buffer_u8", buf, false).gelenint;
							oyunbil[i2].setpl1replay(s);


							s = okubit(okub, "buffer_u8", buf, false).gelenint;
							if (oyunbil[i2].getpl1().set_altiefekt(false, false) == true || oyunbil[i2].getpl1().set_dortefekt(false, false) == true || oyunbil[i2].getpl1().set_yediefekt(false, false) == true || oyunbil[i2].getpl1().set_dokuzefekt(false, false) == true) {
								oyunbil[i2].setplayer_mezarsecili(1, s);
							}


							//std::cout << " X: ";
							oyunbil[i2].svc.p1_mousex = okubit(okub, "buffer_u32", buf, false).gelenint;
							//std::cout << " PL1 MOUSE X " << oyunbil[i2].svc.p1_mousex << std::endl;
							//std::cout << " Y: ";

							oyunbil[i2].svc.p1_mousey = okubit(okub, "buffer_u32", buf, false).gelenint;
							//std::cout << " CLICK: ";
							okub--;
							oyunbil[i2].svc.p1_mouserel = okubit(okub, "buffer_bool", buf, false).gelenbool;

							for (int i = 0; i < 10; i++) {
								//std::cout << " el "<<i<<": ";
								oyunbil[i2].svc.p1_elclick[i] = okubit(okub, "buffer_bool", buf, false).gelenbool;

							}

							oyunbil[i2].svc.p1_fclick[0] = okubit(okub, "buffer_bool", buf, false).gelenbool;
							oyunbil[i2].svc.p1_fclick[1] = okubit(okub, "buffer_bool", buf, false).gelenbool;
							oyunbil[i2].svc.p1_fclick[2] = okubit(okub, "buffer_bool", buf, false).gelenbool;


							//std::cout << " rakip CLICK0: ";
							oyunbil[i2].svc.p1_rfclick[0] = okubit(okub, "buffer_bool", buf, false).gelenbool;

							//std::cout << " rakip CLICK1: ";
							oyunbil[i2].svc.p1_rfclick[1] = okubit(okub, "buffer_bool", buf, false).gelenbool;

							//std::cout << " rakip CLICK2: ";
							oyunbil[i2].svc.p1_rfclick[2] = okubit(okub, "buffer_bool", buf, false).gelenbool;


							if (oyunbil[i2].get_tk().get_tur_durum() != 7 && oyunbil[i2].get_tk().get_tur_durum() != 4) {

								oyunbil[i2].settksaldirkey(0);
								//std::cout << "<c>" << std::endl;
							}


							break;

						case 2:
							okub = 14;
							chtr = okubit(okub, "buffer_u8", buf, false).gelenint;
							SetConsoleTextAttribute(hConsole, 9);
							std::cout << " TIKLADI " << std::endl;
							SetConsoleTextAttribute(hConsole, 15);
							if (chtr == 1) {
								oyunbil[i2].setpl1efektonay(0, oyunbil[i2].getpl1().set_efektoynat(0, false), true);
								SetConsoleTextAttribute(hConsole, 10);
								std::cout << " EVET " << std::endl;
								SetConsoleTextAttribute(hConsole, 15);

							}
							if (chtr == 0) {
								SetConsoleTextAttribute(hConsole, 12);
								std::cout << " HAYIR " << std::endl;
								SetConsoleTextAttribute(hConsole, 15);
								oyunbil[i2].msg2dopl1();
							}
							sendm = 1;
							//Sleep(60);
							break;
						case 3:
							if (oyunbil[i2].get_tk().get_tur_durum() == 3 || oyunbil[i2].get_tk().get_tur_durum() == 4) {
								if (oyunbil[i2].get_tk().get_oyun_dur() == 0) {
									//oyunbil[i2].get_tk().tur_arttir(1, true);
									oyunbil[i2].tk_tur_arttir(1, true);
								}
							}
							break;
						case 4:
							if (oyunbil[i2].get_tk().get_tur_durum() == 4 && oyunbil[i2].get_tk().get_tur() != 4) {
								oyunbil[i2].settksaldirkey(1);
							}
							break;
						case 5:
							okub = 13;
							oyunbil[i2].returnsvc().set_oyuntekrarp1(okubit(okub, "buffer_u8", buf, false).gelenint);
							break;
						case 6:

							if (f_fd_cikar(sock, cstop, oyunbil, maxtop, &master)) {
								std::cout << "SAAA";
							}
							else {
								std::cout << "FAK";
								/*closesocket(sock);
								FD_CLR(sock, &master);*/
								for (int i = 0; i < maxtop; i++) {
									if (cstop[i].sock == sock) {
										cstop[i].f_fd_clr(&master);
										break;
									}
								}
							}

							break;
						}


						//player 1 için gönder

						char* gond;
						gond = oyunbil[i2].bufgonder(oyunbil[i2].getpl1(), oyunbil[i2].getpl2(), cstop, oyunbil[i2].get_player(1), oyunbil[i2].get_player(2), 1,p1ge);
						send(sock, gond, 4096, 0);
						//p1ge = 1;


						if (sendm == 1) {
							SetConsoleTextAttribute(hConsole, 13);
							std::cout << "NOLUYO AQ" << std::endl;
							SetConsoleTextAttribute(hConsole, 15);
							Sleep(60);
							unsigned char gond[16];
							ZeroMemory(gond, 16);
							uint16_t s;
							s = 7;
							gond[0] = s;
							s = 0;
							gond[1] = s;
							send(sock, (char*)&gond, 3, 0);
						}
						else {

							if (oyunbil[i2].getpl1().g_onayg() == true) {
								Sleep(60);
								oyunbil[i2].onayg_p1(sock);
							}
						}




					}
					//aynýsýnýn p2si üþendim
					if (i1 == 2 && i2 != -1 && oyunbil[i2].get_player(1) != -1) {//player 2 için
						oyundami = true;
						okub = 13;
						//std::cout << " ANNEN " << std::endl;
						//std::cout << i1 << " " << i2 << std::endl;
						//std::cout << std::endl << ":::::" << std::endl;
						int msgid = okubit(okub, "buffer_u8", buf, false).gelenint;
						//std::cout << std::endl;
						int sendm2 = 0;
						int s = 0;
						switch (msgid) {
						case 1:
							//std::cout << std::endl;// << "sayda: ";
							oyunbil[i2].svc.p2_sayfa = okubit(okub, "buffer_u8", buf, false).gelenint;
							s = okubit(okub, "buffer_u8", buf, false).gelenint;
							if (s == 1) {
								p2ge = 1;
							}
							s = okubit(okub, "buffer_u8", buf, false).gelenint;
							if (oyunbil[i2].get_tk().get_tur_durum() == 7) {

								oyunbil[i2].settksaldirkey(s);
								SetConsoleTextAttribute(hConsole, 13);
								//std::cout << oyunbil[i2].get_tk().get_saldirkey(0,false) << std::endl;
								SetConsoleTextAttribute(hConsole, 15);
							}

							s = okubit(okub, "buffer_u8", buf, false).gelenint;
							oyunbil[i2].setpl2replay(s);

							s = okubit(okub, "buffer_u8", buf, false).gelenint;

							if (oyunbil[i2].getpl2().set_altiefekt(false, false) == true || oyunbil[i2].getpl2().set_dortefekt(false, false) == true || oyunbil[i2].getpl2().set_yediefekt(false, false) == true || oyunbil[i2].getpl2().set_dokuzefekt(false, false) == true) {
								oyunbil[i2].setplayer_mezarsecili(2, s);
							}


							//std::cout << " X: ";
							oyunbil[i2].svc.p2_mousex = okubit(okub, "buffer_u32", buf, false).gelenint;
							//std::cout << " Y: ";

							oyunbil[i2].svc.p2_mousey = okubit(okub, "buffer_u32", buf, false).gelenint;
							//std::cout << " CLICK: ";
							okub--;
							oyunbil[i2].svc.p2_mouserel = okubit(okub, "buffer_bool", buf, false).gelenbool;

							for (int i = 0; i < 10; i++) {
								//std::cout << " el "<<i<<": ";
								oyunbil[i2].svc.p2_elclick[i] = okubit(okub, "buffer_bool", buf, false).gelenbool;

							}
							//std::cout << " field CLICK0: ";
							oyunbil[i2].svc.p2_fclick[0] = okubit(okub, "buffer_bool", buf, false).gelenbool;

							//std::cout << " field CLICK1: ";
							oyunbil[i2].svc.p2_fclick[1] = okubit(okub, "buffer_bool", buf, false).gelenbool;

							//std::cout << " field CLICK2: ";
							oyunbil[i2].svc.p2_fclick[2] = okubit(okub, "buffer_bool", buf, false).gelenbool;


							//std::cout << " rakip CLICK0: ";
							oyunbil[i2].svc.p2_rfclick[0] = okubit(okub, "buffer_bool", buf, false).gelenbool;

							//std::cout << " rakip CLICK1: ";
							oyunbil[i2].svc.p2_rfclick[1] = okubit(okub, "buffer_bool", buf, false).gelenbool;

							//std::cout << " rakip CLICK2: ";
							oyunbil[i2].svc.p2_rfclick[2] = okubit(okub, "buffer_bool", buf, false).gelenbool;



							if (oyunbil[i2].get_tk().get_tur_durum() != 7 && oyunbil[i2].get_tk().get_tur_durum() != 4) {

								oyunbil[i2].settksaldirkey(0);
								//std::cout << "<b>" << std::endl;
							}

							break;

						case 2:
							okub = 14;

							chtr = okubit(okub, "buffer_u8", buf, false).gelenint;
							SetConsoleTextAttribute(hConsole, 9);
							std::cout << " TIKLADI " << std::endl;
							SetConsoleTextAttribute(hConsole, 15);
							if (chtr == 1) {
								oyunbil[i2].setpl2efektonay(0, oyunbil[i2].getpl2().set_efektoynat(0, false), true);
								SetConsoleTextAttribute(hConsole, 10);
								std::cout << " EVET " << std::endl;
								SetConsoleTextAttribute(hConsole, 15);
							}
							if (chtr == 0) {
								oyunbil[i2].msg2dopl2();
								SetConsoleTextAttribute(hConsole, 12);
								std::cout << " HAYIR " << std::endl;
								SetConsoleTextAttribute(hConsole, 15);
							}
							sendm2 = 1;
							break;
						case 3:
							if (oyunbil[i2].get_tk().get_tur_durum() == 6 || oyunbil[i2].get_tk().get_tur_durum() == 7) {
								if (oyunbil[i2].get_tk().get_oyun_dur() == 0) {
									//oyunbil[i2].get_tk().tur_arttir(1, true);
									oyunbil[i2].tk_tur_arttir(1, true);
								}
							}
							break;
						case 4:
							if (oyunbil[i2].get_tk().get_tur_durum() == 7) {
								oyunbil[i2].settksaldirkey(1);
							}
							break;
						case 5:
							okub = 13;
							oyunbil[i2].returnsvc().set_oyuntekrarp2(okubit(okub, "buffer_u8", buf, false).gelenint);
							break;
						case 6:

							if (f_fd_cikar(sock, cstop, oyunbil, maxtop, &master)) {
								std::cout << "SAAA";
							}
							else {
								std::cout << "FAK";
								/*closesocket(sock);
								FD_CLR(sock, &master);*/
								for (int i = 0; i < maxtop; i++) {
									if (cstop[i].sock == sock) {
										cstop[i].f_fd_clr(&master);
										break;
									}
								}
							}

							break;
						}


						//player 2 için gönder
						char* gond;
						gond = oyunbil[i2].bufgonder(oyunbil[i2].getpl2(), oyunbil[i2].getpl1(), cstop, oyunbil[i2].get_player(2), oyunbil[i2].get_player(1), 2,p2ge);
						send(sock, gond, 4096, 0);
						//p2ge = 1;

						if (sendm2 == 1) {
							Sleep(60);
							unsigned char gond[16];
							ZeroMemory(gond, 16);
							uint16_t s;
							s = 7;
							gond[0] = s;
							s = 0;
							gond[1] = s;
							send(sock, (char*)&gond, 3, 0);
						}
						else {
							if (oyunbil[i2].getpl2().g_onayg() == true) {
								Sleep(60);
								oyunbil[i2].onayg_p2(sock);
							}
						}
					}

					if (oyundami == false) {
						unsigned char ret2[2];
						ZeroMemory(ret2, 2);
						ret2[0] = 15;
						char* gond2 = (char*)&ret2;
						send(sock, gond2, 2, 0);
					}
					if (p1ge == 1 && p2ge == 1) {
						for (int i = 0; i < oyunbil[i2].max_i_s; i++) {
							oyunbil[i2].efcontrol[i].clr();
						}
						p1ge = 0;
						p2ge = 0;
					}
					//mesaj gönder
					//std::cout << "SEMENDERPIPI" << std::endl;
					/*
					std::string msg1;
					msg1 = "SEMENDER007sex";
					int uz = msg1.length();
					char text1[4096];
					ZeroMemory(text1,4096);
					std::stringstream ss;
					ss << msg1;
					ss >> text1;
					std::cout << " TEWXT1: " << text1 << std::endl;

					unsigned char ggsin1[16];
					ZeroMemory(ggsin, 16);
						uint16_t s = 4;
						ggsin[0] = s;
						send(sock, (char*)&ggsin, 1, 0);
					    send(sock,text1, uz, 0);
					*/


					//oku
					//std::cout << "client " << f_s_sbul(sock, maxtop, cstop) << " : "<<std::endl;
					//std::cout << "SOCK "<<sock << std::endl;
					//okubit(13, "buffer_u8", buf, false);
					//std::cout << " " << i << " "<<socketcount <<" "<<"SA "<<std::endl;
				}

			}

		}

		
	
	}
	



	
	//winsoku bitir
	WSACleanup();

	system("pause");




}




