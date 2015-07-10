#include "Buffer.h"
#include <iostream>
int main()
{
	SockBuffer *BUFF = new SockBuffer;
	char data[] = { "0123456789" };
	char ipdata[] = { "9876543210" };
	BUFF->AddData(data, 10);
	BUFF->AddIPData(ipdata, 10);
	BUFF->Print();
	//BUFF.Erace();
	int size = BUFF->Size(BUFF);
	char *readdata, *readipdata;
	readdata = new char[size];
	readipdata = new char[size];

	BUFF->Read(readdata, readipdata, BUFF);
	printf("data read: %s", readdata);
	delete BUFF;
	return 0;
}