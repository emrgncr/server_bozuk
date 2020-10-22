#include <iostream>
#include <string>
#include <sstream>
#include <WS2tcpip.h>
#include <time.h>
#include "hdene.h"
#include "oynat.h"
#pragma comment(lib,"ws2_32.lib")

/*
	KAREYÝ HESAPLA

	CLÝENTLERDEN BÝLGÝ AL

	CLÝENTLERE BÝLGÝ GÖNDER

*/




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

	fd_set master;
	FD_ZERO(&master);
	FD_SET(listening, &master);
	while (true) {
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
					closesocket(sock);
					FD_CLR(sock, &master);
				}
				else {
					//mesaj gönder
					//oku
					//std::cout << "client " << sock << " : "<<std::endl;
					okubit(13, "buffer_u8", buf, true);
					//std::cout << " " << i << " "<<socketcount <<" "<<"SA "<<std::endl;
				}

			}

		}
	
	}
	



	
	//winsoku bitir
	WSACleanup();

	system("pause");




}