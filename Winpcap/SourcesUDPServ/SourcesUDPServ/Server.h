//#include<stdio.h>
#include<winsock2.h>

class UDPServer{

public:
	UDPServer(USHORT Port);
	~UDPServer();
	
	//void Read();
	void StartThread();
private:
	void init();
	void create();
	void binding();
	static void reading(void *pParams);

	USHORT BUFLEN;
	USHORT port;
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[1400];
	WSADATA wsa;
};