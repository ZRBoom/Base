#include "Buffer.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
SockBuffer::SockBuffer()
{
	Erace();
	ERROR = 0;
	int CurDataSize = 0;
}


SockBuffer::~SockBuffer()
{
	Erace();
}


bool SockBuffer::Empty(){

		if (Data.empty() && IPdata.empty())
		{
			return true;
		}
		else
			return false;

}

void SockBuffer::Erace(){
	Data.erase(Data.begin(), Data.end());
	IPdata.erase(IPdata.begin(), IPdata.end());
}

void SockBuffer::Delete(){
	Data.pop_front();
	IPdata.pop_front();
}

int SockBuffer::Size(SockBuffer *Object){
	Object->ItData = Object->Data.begin();
	Object->CurDataSize = Object->ItData->size();
	return Object->ItData->size();
}

void SockBuffer::Print(){
	std::cout << "Data contains:\n";
	ItData = Data.begin();
	while (ItData != Data.end())
	{
		std::cout << ItData->data() << " ";
		ItData++;
	}
	std::cout << '\n';

	ItIpData = IPdata.begin();
	std::cout << "IpData contains: \n";
	while (ItIpData != IPdata.end())
	{
		std::cout << ItIpData->data() << " ";
		ItIpData++;
	}
	std::cout << '\n';
}

void SockBuffer::AddData(char* data, int sizedata){

	std::vector<char> vectlst(sizedata);

	std::copy(data, data + sizedata, vectlst.begin());

	std::cout << "vectlst contains:";
	for (std::vector<char>::iterator it = vectlst.begin(); it != vectlst.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	Data.push_back(vectlst);
}

void SockBuffer::AddIPData(char* data, int sizedata){
	/*char fst[] = { "423423" };
	char scd[] = { "312312" };*/

	std::vector<char> vectlst(sizedata);

	std::copy(data, data + sizedata, vectlst.begin());

	std::cout << "vectlst contains:";
	for (std::vector<char>::iterator it = vectlst.begin(); it != vectlst.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';

	IPdata.push_back(vectlst);

	/*std::cout << "Data contains:\n";
	ItData = Data.begin();
	while (ItData != Data.end())
	{
		std::cout << ItData->data() << " ";
		ItData++;
	}
	std::cout << '\n';

	ItIpData = IPdata.begin();
	std::cout << "IpData contains: \n";
	while (ItIpData != IPdata.end())
	{
		std::cout << ItIpData->data() << " ";
		ItIpData++;
	}
	std::cout << '\n';*/
}

void SockBuffer::Read(char *data, char *ipdata, SockBuffer *Object){
//	std::cout << "check";
	Object->ItData = Data.begin();
	for (int i = 0; i < Object->CurDataSize; i++)
	{
		data[i] = Object->ItData->data()[i];
	}
//	std::cout << "check";
	Object->Delete();
}