#include "Server.h"
#include <process.h>
#include <stdio.h>

int a[5];

/*void Thread(void* pParams)
{
	int i, num = 0;

	while (1)
	{
		for (i = 0; i < 5; i++) a[i] = num;
		num++;
	}
}*/
UDPServer *Server1, *Server2;

int main()
{
	Server1 = new UDPServer(5555);
	Server1->StartThread();

	Server2 = new UDPServer(6666);
	Server2->StartThread();
	while (1)
	{

	}
	//_beginthread(Server.reading, 0, NULL);

	
	//void(UDPServer::*p)(void*) = &UDPServer::reading;
	//_beginthread(Server.*p, 0, NULL);
	//(Server.*p)();
	//Server = new UDPServer(5555);
	//_beginthread(Server.*pt, 0, NULL);


		
	return 0;
}
