#include <iostream>
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#include <fstream>
#include "hdene.h"
#pragma comment(lib,"ws2_32.lib")






void main() {
	
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

	// baðlantý bekle
	sockaddr_in client;
	int clientsize = sizeof(client);
	SOCKET clientsocket = accept(listening, (sockaddr*)&client, &clientsize);

	if (clientsocket == INVALID_SOCKET) {
		std::cerr << "client socket oluþturulamadý. Cikiliyor" << std::endl;
		return;
	}


	char host[NI_MAXHOST];	//client in remote adý
	char service[NI_MAXSERV];	// clientin baðlandýðý service/port
	ZeroMemory(host,NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV,0) == 0) 
	{
		std::cout << host << " CONNECTED ON PORT : " << service << std::endl;
	}
	else 
	{
		inet_ntop(AF_INET,&client.sin_addr,host,NI_MAXHOST);
		std::cout << host << " CONNECTED2 ON PORT : " << ntohs(client.sin_port)<<std::endl;

	}
	//dinleyen socketi kapa
	closesocket(listening);

	//while : mesajý al ve geri yansýt
	char buf[4096];
	char semender[4096];
	while (true) {
		ZeroMemory(buf, 4096);
		ZeroMemory(semender, 4096);
		//clientin veri göndermesini -bekle
		int bytesrecieved = recv(clientsocket, buf, 4096,0);
		if (bytesrecieved == SOCKET_ERROR) {
			std::cerr << "recv() hatasý. Cikiliyor" << std::endl;
			break;
		}
		if (bytesrecieved == 0) {
			std::cout << "client dc" << std::endl;
			break;
		}
		/*if (bytesrecieved >= 10) {
			std::ofstream file;
			file.open("C:/Users/asus/Desktop/3dclient/baban.txt");
			file << buf;
			break;
		}*/

		//send(clientsocket, semender, bytesrecieved + 1, 0);
		unsigned char b1 = 0;
		for (int i = 0; i < 152; ++i) {
			semender[i] = b1;
		}
		b1 = 1;
		semender[0] = b1;
		b1 = 1;
		semender[1] = b1;
		semender[2] = b1;
		b1 = 3;
		semender[3] = b1;
		b1 = 0;
		semender[4] = b1;
		semender[5] = b1;
		semender[6] = b1;
		semender[7] = b1;
		semender[8] = b1;
		semender[9] = b1;
		semender[10] = b1;
		b1 = 13;
		semender[11] = b1;
		semender[12] = b1;
		b1 = 0;
		for (int i = 0; i < 4; ++i) {//13 14 15 16}
			semender[13 + i] = b1;
		}
		b1 = 1;
		semender[17] = b1;
		semender[18] = b1;
		b1 = 0;
		semender[19] = b1;
		semender[20] = b1;
		semender[21] = b1;
		b1 = 2;
		semender[22] = b1;
		b1 = 0;


		//send(clientsocket, semender, 153, 0);
		//send(clientsocket, buf, bytesrecieved + 1, 0);
		/*std::ifstream file;
		//std::string semen;
		file.open("C:/Users/asus/Desktop/3dclient/baban.txt");
		file >> semender;
		send(clientsocket, semender, 300, 0);*/
		/*int k = 0;
		for (int i = 12*8; i < (bytesrecieved) * 8; ++i) {
			if (i % 8 == 0) std::cout << " ";
			int f = std::floor(i / 8);
			bool t = buf[f] & (1 << (i%8));
			if(t) k += myPow(2, (i%8));
			if ((i + 1) % 8 == 0) { std::cout << k; 
			k = 0;
			}

		}
		k = 0;*/
		okub = 13;
		okubit(okub, "buffer_u8", buf,true);
		okubit(okub, "buffer_u8", buf, true);
		okubit(okub, "buffer_u32", buf, true);
		okubit(okub, "buffer_u32", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		okubit(okub, "buffer_bool", buf, true);
		std::cout << std::endl;
		/*for (int i = 0; i < (bytesrecieved + 1); ++i) {
			std::cout << buf[i];
		}
		std::cout << bytesrecieved + 1 << std::endl;*/
		//veriyi geri yansýt


	}

	// 



	//socketi kapa
	closesocket(clientsocket);
	
	//winsoku bitir
	WSACleanup();






}