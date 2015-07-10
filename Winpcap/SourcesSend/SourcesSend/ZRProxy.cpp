// SourcesSend.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "pcapsend.h"
#include <pcap.h>

int main()
{
	char devNames[100];
	ZeroMemory(devNames, 100);

	pcap_if_t *alldevs;
	pcap_if_t *d;
	int h = 0;
	char errbuf[PCAP_ERRBUF_SIZE];

	/* Retrieve the device list from the local machine */
	if (pcap_findalldevs(&alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs_ex: %s\n", errbuf);
		exit(1);
	}

	for (d = alldevs; d != NULL; d = d->next)
	{
		strcpy_s(devNames, d->name);
		printf("%d. %s", ++h, d->name);
		if (d->description)
		{
			printf(" (%s)\n", d->description);
			printf(" (%s)\n", d->addresses);
		}
		else
			printf(" (No description available)\n");

	}
	if (h == 0)
	{
		printf("\nNo interfaces found! Make sure WinPcap is installed.\n");
		return 0;
	}

	pcap_freealldevs(alldevs);

	
	char defmac[6];
	char srcmac[6];
	for (int i = 0; i < 6; i++)
	{
		defmac[i] = i;
		//srcmac[i] = i + 1;
	}
	srcmac[0] = 0x74;
	srcmac[1] = 0xd4;
	srcmac[2] = 0x35;
	srcmac[3] = 0x36;
	srcmac[4] = 0x78;
	srcmac[5] = 0x53;
	wincap *Client;
	Client = new wincap(devNames, defmac, srcmac);

	char data[5];
	USHORT size = 5;
	char ipdest[4] = { 192, 168, 0, 1 };
	char ipsrc[4] = { 192, 168, 0, 100 };
	USHORT port = 55555;


	for (int i = 0; i < size; i++)
	{
		data[i] = i+60;
	}

	bool ifsent = false;
	if (Client->openstatus)
	{
		ifsent = Client->sendpacket(port, ipsrc, ipdest, data, size);
		ifsent = Client->sendpacket(port, ipsrc, ipdest, data, size);
		ifsent = Client->sendpacket(port, ipsrc, ipdest, data, size);
		ifsent = Client->sendpacket(port, ipsrc, ipdest, data, size);
		ifsent = Client->sendpacket(port, ipsrc, ipdest, data, size);
	}
	delete Client;
	
	return 0;
}

