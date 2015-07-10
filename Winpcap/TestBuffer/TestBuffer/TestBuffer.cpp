// TestBuffer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include <iterator>

#include <winsock.h>

#include <stddef.h>
#include <stdint.h>
#include <list>
using namespace std;
std::list<std::vector<char>> lst(0, std::vector<char>(10));
std::vector<char> vectlst33(6);

/*void copyVecFast(const vec<int>& original)
{
	vector<int> newVec;
	newVec.reserve(original.size());
	copy(original.begin(), original.end(), back_inserter(newVec));
}*/
void add(std::list<std::vector<char>> &lst){
	char fst[] = { "111111" };
	std::copy(fst, fst + 6, vectlst33.begin());

	list<vector<char>>::iterator p = lst.begin();
	lst.push_back(vectlst33);

	//std::copy(vectlst33.begin(), vectlst33.end(), p->end);
}

int main()
{
	char test[10];
	ZeroMemory(test, 10);
	test[0] = 'g';
	test[1] = 's';
	test[2] = 0x01;
	test[3] = 'h';
	printf("size buf: %s+\n", test);

	char buf[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };

	std::vector<char> vectbuf(9);

	std::copy(buf, buf + 9, vectbuf.begin());

	std::cout << "vectbuf contains:";
	for (std::vector<char>::iterator it = vectbuf.begin(); it != vectbuf.end(); ++it)
		std::cout << ' ' << *it;

	std::cout << '\n';

	char buf2[] = { 'i', 'g', 'k', 'l', 'm', 'n', 'o', 'p' };
	std::vector<char> vectbuf2(8);

	std::copy(buf2, buf2 + 8, vectbuf2.begin());

	std::cout << "vectbuf2 contains:";
	for (std::vector<char>::iterator it = vectbuf2.begin(); it != vectbuf2.end(); ++it)
		std::cout << ' ' << *it;

	std::cout << '\n';

	std::vector<std::vector<char>> endvec(0,std::vector<char>(10));
	endvec.push_back(vectbuf);
	endvec.push_back(vectbuf2);
	endvec.push_back(vectbuf2);

	std::cout << "endvec contains: \n";
	for (int i = 0; i < endvec.size(); i++)
	{
		printf("%d ", i);
		for (std::vector<char>::iterator it = endvec[i].begin(); it != endvec[i].end(); ++it)
		{
			std::cout << ' ' << *it;
		}
		printf("+++");
		std::cout << '\n';
	}
	std::cout << endvec[0].size();
	std::cout << '\n';
	
	buf[2] = '0';
	buf2[2] = '0';

	vectbuf[2] = '0';
	vectbuf2[2] = '0';
	printf("%c",vectbuf[2]);
	std::cout << "endvec contains after changes:";
	for (int i = 0; i < endvec.size(); i++)
	{
		for (std::vector<char>::iterator it = endvec[i].begin(); it != endvec[i].end(); ++it)
			std::cout << ' ' << *it;

	}
	

	std::cout << '\n';
	char *arr;
	arr = endvec[0].data();
	//arr[9] = '\0';
	printf("%c ", arr[0]);
	std::cout << '\n';
	printf("%c", endvec[0].data()[1]);
	std::cout << '\n';

	std::cout << "test with list\n";

	char *tst = new char[5];
	tst[0] = '1';
	tst[1] = '1';
	tst[2] = '1';
	tst[3] = '1';
	tst[4] = '1';

	char fst[] = { "423423" };
	char scd[] = { "312312" };

	std::vector<char> vectlst(6);

	std::copy(fst, fst + 6, vectlst.begin());

	std::cout << "vectlst contains:";
	for (std::vector<char>::iterator it = vectlst.begin(); it != vectlst.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';
	
	std::vector<char> vectlst2(6);

	std::copy(scd, scd + 6, vectlst2.begin());

	std::cout << "vectlst2 contains:";
	for (std::vector<char>::iterator it = vectlst2.begin(); it != vectlst2.end(); ++it)
		std::cout << ' ' << *it;
	std::cout << '\n';


	
//	list<char *> lst;
	lst.push_back(vectlst);
	lst.push_back(vectlst2);
	//lst.push_back(vectlst);
	add(lst);
	//lst.push_back(tst);
	delete []tst;
	fst[1] = 'g';
	fst[3] = 'g';
	list<vector<char>>::iterator p = lst.begin();
	
	/*char *ttt;
	p++;
	ttt = *p;
	p--;*/
	//printf("This is data of list %s\n", ttt);
	while (p != lst.end())
	{
		std::cout << p->data() << " ";
		lst.pop_front();
		p = lst.begin();
		std::cout << '\n';
	}


	while (p != lst.end()){
		std::cout << p->data() << " ";
		p++;
		std::cout << '\n';
	}
	return 0;
}

