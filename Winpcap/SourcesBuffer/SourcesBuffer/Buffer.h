#include <vector>
#include <list>
#include <iterator>

class SockBuffer{
public:
	SockBuffer();
	~SockBuffer();
	void AddData(char* data, int sizedata);
	void AddIPData(char *data, int sizedata);
	void Read(char *data, char *ipdata, SockBuffer *Object);
	int Size(SockBuffer *Object);
	bool Empty();
	int ERROR;
	void Print();
	void Erace();
private:
	//bool Check();
	int CurDataSize;
	
	void Delete();
	std::list<std::vector<char>> Data;
	std::list<std::vector<char>> IPdata;
	std::list<std::vector<char>>::iterator ItData = Data.begin();
	std::list<std::vector<char>>::iterator ItIpData = IPdata.begin();
};