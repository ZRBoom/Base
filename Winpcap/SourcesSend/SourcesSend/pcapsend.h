
#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <winsock.h>
#include <stddef.h>
#include <stdint.h>

#define _CRT_SECURE_NO_WARNINGS
#pragma once
class wincap{

public:
	//wincap();//Unuse
	wincap(char* DevName, char* defmac, char* srcmac);//Constructor, that open for wark network adapter, and set def Src MAC and Dest MAC
	~wincap();
	bool sendpacket(USHORT DestPort, char* IPsource, char* IPdest, char* Data, USHORT DataSize);//Create packet and send to network
	bool openstatus;//Status of opening the network adapter
	
private:
	USHORT checksum(USHORT *buffer, int size); //Calculate checksum of pakets(UDP,IP)
	void CreateEthHeader(char* EthHead);//Create the Ethernet Header
	void CreateIPHeader(char* IPHead);//Create the IP header
	void CreateUDPHeader(char* UDPHead, char* IPHead, char* Data);//Create the UDP header
	char CheckTypeOfService(USHORT Port);//Receive tos by port

	char errbuf[PCAP_ERRBUF_SIZE]; //buffer for errors with using winpcap
	pcap_t *Interface; //the var, uses for open,close network card and send packet
	char Mymac[6]; //The default Source MAC
	char Defmac[6];// The default Destination MAC

	//Parametrs of Packet
	USHORT dsize; //Size of sending packet
	USHORT dport;//Sending udp port
	char ipsrc[4]; //IPsource in IPheader
	char ipdest[4]; //IPdest in IPHeader

};
