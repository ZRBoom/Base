#include "stdafx.h"
#include "pcapsend.h"

wincap::wincap(char* DevName, char* defmac, char* srcmac){
	openstatus = false;
	
	if ((Interface = pcap_open_live(DevName, 65536, 1, 1000, errbuf)) == NULL)
	{
		openstatus = false;
	}
else
	openstatus = true;

	for (int i = 0; i < 6; i++)
	{
		Mymac[i] = srcmac[i];//Set the source MAC address of all packets
		Defmac[i] = defmac[i];//Set the destination MAC address of all packets
	}

	/*ZeroMemory(IPheader, 20);
	ZeroMemory(UDPheader, 8);
	ZeroMemory(Etherheader, 14);*/

	ZeroMemory(ipsrc, 4);
	ZeroMemory(ipdest, 4);
}

wincap::~wincap(){
	openstatus = false;
	pcap_close(Interface);
}

USHORT wincap::checksum(USHORT *buffer, int size) 
{
	unsigned long cksum = 0;
	while (size > 1){
		cksum += *buffer++;
		size -= sizeof(USHORT);
	}
	if (size){
		cksum += *(UCHAR*)buffer;
	}
	cksum = (cksum >> 16) + (cksum & 0xffff);
	cksum += (cksum >> 16);
	return (USHORT)(~cksum);
}
char wincap::CheckTypeOfService(USHORT Port)
{
	char tos = 0;;
	if (Port == 49001)
		tos = 4;
	else if (Port == 49003)
		tos = 8;
	else if (Port == 49005)
		tos = 12;
	else if (Port == 49007)
		tos = 16;
	else if (Port == 49009)
		tos = 20;
	else if (Port == 49011)
		tos = 24;
	else if (Port == 49013)
		tos = 28;
	else if (Port == 49015)
		tos = 32;
	else if (Port == 49031)
		tos = 36;
	else if (Port == 49033)
		tos = 40;
	else if (Port == 49035)
		tos = 44;
	else if (Port == 49037)
		tos = 48;
	else if (Port == 49039)
		tos = 52;
	else if (Port == 49041)
		tos = 56;
	else if (Port == 49043)
		tos = 60;
	else if (Port == 49045)
		tos = 64;
	else tos = 0;

	return tos;
}
void wincap::CreateEthHeader(char* EthHead)
{
	for (int i = 0; i<6; i++)
	{
		EthHead[i] = Defmac[i];//Dest MAC
		EthHead[i + 6] = Mymac[i];//Src MAC
	}
	EthHead[12] = 0x08;//Type of
	EthHead[13] = 0x00;//protocol
}

void wincap::CreateIPHeader(char* IPHead)
{
	//VerLen
	IPHead[0] = 0x45;
	
	//Type of service
	IPHead[1] = CheckTypeOfService(dport);
	
	//Total Length
	USHORT totallength = dsize + 28;
	IPHead[2] = (totallength >> 8) & 0xff;
	IPHead[3] = totallength & 0xff;


	//id
	IPHead[4] = 0x00;
	IPHead[5] = 0x00;
	
	//OffSet
	IPHead[6] = 0x00;
	IPHead[7] = 0x00;

	//Time to live
	IPHead[8] = 0x80;

	//Type of include protocol(only UDP)
	IPHead[9] = 17;
	
	//Set to zero checksum before calculate
	IPHead[10] = 0x00;
	IPHead[11] = 0x00;

	//Set the source and destination IP address
	for (int i = 0; i < 4; i++)
	{
		IPHead[12 + i] = ipsrc[i];
		IPHead[16 + i] = ipdest[i];
	}

	//Calculate checksum(use the same var that using for ttl for memroy saving)
	totallength = checksum((USHORT *)IPHead, 20);
	
	//Set rigth checksum of IP
	IPHead[10] = totallength & 0xff;
	IPHead[11] = (totallength >> 8) & 0xff;
}

void wincap::CreateUDPHeader(char* UDPHead, char* IPHead, char* Data)
{
	//Set source port
	UDPHead[0] = (dport >> 8) & 0xff;
	UDPHead[1] = dport & 0xff;

	
	//Set destination port
	UDPHead[2] = (dport >> 8) & 0xff;
	UDPHead[3] = dport & 0xff;


	//Set the length of UDP packet
	USHORT length = dsize + 8;
	UDPHead[4] = (length >> 8) & 0xff;
	UDPHead[5] = length & 0xff;


	//Set to zero checksum before calculate
	UDPHead[6] = 0x00;
	UDPHead[7] = 0x00;
	
	//Create array to calculate checksum udp
	int parity = dsize % 2;
	char *csar = new char[dsize + 20 + parity];
		//Create UDP pseudo header
	for (int i = 0; i < 4; i++)
	{
		csar[i] = IPHead[12 + i];
		csar[4 + i] = IPHead[16 + i];
	}
	csar[8] = 0x00;
	csar[9] = IPHead[9];

	csar[10] = UDPHead[4];
	csar[11] = UDPHead[5];

	for (int i = 0; i < 8; i++)
	{
		csar[12 + i] = UDPHead[i];
	}

	for (int i = 0; i < dsize; i++)
	{
		csar[20 + i] = Data[i];
	}

	//Add zero byte if data non parity
	if (parity == 1)
	{
		csar[dsize + 20] = 0x00;
	}

	length = checksum((USHORT *)csar, dsize + 20 + parity);
	UDPHead[6] = length & 0xff;
	UDPHead[7] = (length >> 8) & 0xff;

		
}

bool wincap::sendpacket(USHORT DestPort, char* IPsource, char* IPdest, char* Data, USHORT DataSize)
{
	bool ifSent = false;
	u_char *packet;
	packet = new u_char[DataSize + 42];//Allocate memory for new packet
	char ethhead[14];
	char iphead[20];
	char udphead[8];

	//Initialize vars
	dsize = DataSize;
	dport = DestPort;
	for (int i = 0; i < 4; i++)
	{
		ipsrc[i] = IPsource[i];
		ipdest[i] = IPdest[i];
	}

	CreateEthHeader(ethhead); //Create Eth head
	CreateIPHeader(iphead); //Create ip head
	CreateUDPHeader(udphead, iphead, Data); //Create udp head
	
	//Copy Ethernet header to packet
	for (int i = 0; i < 14; i++)
	{
		packet[i] = ethhead[i];
	}

	//Copy IP header to packet
	for (int i = 0; i < 20; i++)
	{
		packet[14 + i] = iphead[i];
	}
	
	//Copy UDP header to packet
	for (int i = 0; i < 8; i++)
	{
		packet[34 + i] = udphead[i];
	}

	//Copy Data to packet 
	for (int i = 0; i < DataSize; i++)
	{
		packet[42 + i] = Data[i];
	}
	//Send the packet
	if (openstatus == true)
	{
		if (pcap_sendpacket(Interface, packet, DataSize + 42) != 0)
			ifSent = false;
		else ifSent = true;
	}
	
	return ifSent;
}