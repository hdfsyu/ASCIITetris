#include <iostream>
using namespace std;
wstring block[7];
int main() {
	//block 0
	block[0].append(L"..X.");
	block[0].append(L"..X.");
	block[0].append(L"..X.");
	block[0].append(L"..X.");
	//block 1
	block[1].append(L"..X.");
	block[1].append(L".XX.");
	block[1].append(L".X..");
	block[1].append(L"....");
	//block 2
	block[2].append(L".X..");
	block[2].append(L".XX.");
	block[2].append(L"..X.");
	block[2].append(L"....");
	//block 3
	block[3].append(L"....");
	block[3].append(L".XX.");
	block[3].append(L".XX.");
	block[3].append(L"....");
	//block 4
	block[4].append(L"..X.");
	block[4].append(L".XX.");
	block[4].append(L"..X.");
	block[4].append(L"....");
	//block 5
	block[5].append(L"....");
	block[5].append(L".XX.");
	block[5].append(L"..X.");
	block[5].append(L"..X.");
	//block 6
	block[6].append(L"....");
	block[6].append(L".XX.");
	block[6].append(L".X..");
	block[6].append(L".X..");
}