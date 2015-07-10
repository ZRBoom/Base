//#include "stdafx.h"

#include <stdio.h>
#include <winsock2.h>
#include "Server.h"
#include <process.h>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

UDPServer::~UDPServer()
{
	closesocket(s);
	WSACleanup();
}

UDPServer::UDPServer(USHORT Port)
{
	port = Port;
	BUFLEN = 1400;
	slen = sizeof(si_other);

	//Initialise winsock
	printf("\nInitialising Winsock...");
	init();

	//Create a socket
	create();

	//Bind
	binding();

}

void UDPServer::init()
{
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");
}

void UDPServer::create()
{
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created on port %d.\n", port);

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
}

void UDPServer::binding()
{
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts("Bind done");
}

void UDPServer::reading(void *pParams)
{
	UDPServer *serv = (UDPServer*)pParams;
	while (1)
	{

		printf("Waiting for data...");
		fflush(stdout);

		//clear the buffer by filling null, it might have previously received data
		memset(serv->buf, '\0', serv->BUFLEN);

		//try to receive some data, this is a blocking call
		if ((serv->recv_len = recvfrom(serv->s, serv->buf, serv->BUFLEN, 0, (struct sockaddr *) &serv->si_other, &serv->slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(serv->si_other.sin_addr), ntohs(serv->si_other.sin_port));
		printf("Data: %s\n", serv->buf);
		printf("Receive len %d\n", serv->recv_len);

		//now reply the client with the same data
		/*if (sendto(s, buf, recv_len, 0, (struct sockaddr*) &si_other, slen) == SOCKET_ERROR)
		{
		printf("sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
		}*/
	}
}

void UDPServer::StartThread()
{
	_beginthread(reading, 0, (void *) this);
}