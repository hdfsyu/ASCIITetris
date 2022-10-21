#include <iostream>
#include <thread>
#include <vector>
#include <stdio.h>
#include <Windows.h>
using namespace std;
wstring block[7];
int fwidth = 12;
int fheight = 18;
int swidth = 120;
int sheight = 30;
unsigned char* field = nullptr;
int Rotate(int px, int py, int r) {
	switch (r % 4) {
		case 0: return py * 4 + px;
		case 1: return 12 + py - (px * 4);
		case 2: return 15 - (py * 4) - px;
		case 3: return 3 - py + (px * 4);
	}
	return 0;
}
bool DoesPieceFit(int nBlock, int rotation, int posX, int posY) {
	for (int px = 0; px < 4; px++)
		for (int py = 0; py < 4; py++) {
			int pi = Rotate(px, py, rotation);
			int fi = (posY + py) * fwidth + (posX + px);
			if (posX + px >= 0 && posX + px < fwidth) {
				if (posY + py >= 0 && posY + py < fheight) {
					if (block[nBlock][pi] == L'X' && field[fi] != 0)
						return false;
				}
			}
		}
	return true;
}
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
	field = new unsigned char[fwidth * fheight];
	for (int x = 0; x < fwidth; x++)
		for (int y = 0; y < fheight; y++)
			field[y * fwidth + x] = (x == 0 || x == fwidth - 1 || y == fheight - 1) ? 9 : 0;
	wchar_t* screen = new wchar_t[swidth * sheight];
	for (int i = 0; i < swidth * sheight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;
	bool gameOver = false;
	int cp = 0; // current pos
	int cr = 0; // current rotation
	int cx = fwidth / 2; // current x pos
	int cy = 0; // current y pos
	bool key[4];
	bool rHold = false; // is rotation held
	int speed = 20;
	int speedc = 0; // speed counter
	bool fd = false; // force piece down check
	int pc = 0; // piece counter
	int score = 0;
	vector<int> lines;
	HWND hwnd = GetConsoleWindow();
	if (hwnd != NULL)
	{
		MoveWindow(hwnd, 200, 200, 680, 400, TRUE);
	}
	while (!gameOver) {
		std::this_thread::sleep_for(50ms);
		speedc++;
		fd = (speedc == speed);
		for (int k = 0; k < 4; k++)
			key[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;
		cx += (key[0] && DoesPieceFit(cp, cr, cx + 1, cy)) ? 1 : 0;
		cx -= (key[1] && DoesPieceFit(cp, cr, cx - 1, cy)) ? 1 : 0;
		cy += (key[2] && DoesPieceFit(cp, cr, cx, cy + 1)) ? 1 : 0;
		if (key[3]) {
			cr += (!rHold && DoesPieceFit(cp, cr + 1, cx, cy)) ? 1 : 0;
			rHold = true;
		}
		else {
			rHold = false;
		}
		if (fd) {
			if (DoesPieceFit(cp, cr, cx, cy + 1)) {
				cy++;
			}
			else {
				for (int px = 0; px < 4; px++)
					for (int py = 0; py < 4; py++)
						if (block[cp][Rotate(px, py, cr)] == L'X')
							field[(cy + py) * fwidth + (cx + px)] = cp + 1;
				pc++;
				if (pc % 10 == 0)
					if (speed >= 10) speed--;
				for(int py = 0; py < 4; py++)
					if (cy + py < fheight - 1) {
						bool line = true;
						for (int px = 1; px < fwidth - 1; px++)
							line &= (field[(cy + py) * fwidth + px]) != 0;
						if (line) {
							for (int px = 1; px < fwidth - 1; px++)
								field[(cy + py) * fwidth + px] = 8;
							lines.push_back(cy + py);
						}
					}
				score += 25;
				if (!lines.empty()) score += (1 << lines.size() - 1) * 100;
				cx = fwidth / 2;
				cy = 0;
				cr = 0;
				cp = rand() % 7;
				gameOver = !DoesPieceFit(cp, cr, cx, cy);
			}
			speedc = 0;
		}
		for (int x = 0; x < fwidth; x++)
			for (int y = 0; y < fheight; y++)
				screen[(y + 2) * swidth + (x + 2)] = L" ABCDEFG-M"[field[y * fwidth + x]];
		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
				if (block[cp][Rotate(px, py, cr)] == L'X')
					screen[(cy + py + 2) * swidth + (cx + px + 2)] = cp + 65;
		swprintf_s(&screen[2 * swidth + fwidth + 6], 16, L"SCORE: %8d", score);
		if (!lines.empty()) {
			WriteConsoleOutputCharacter(hConsole, screen, swidth * sheight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms);
			for(auto &v : lines)
				for (int px = 1; px < fwidth - 1; px++) {
					for (int py = v; py > 0; py--)
						field[py * fwidth + px] = field[(py - 1) * fwidth + px];
					field[px] = 0;
				}
			lines.clear();
		}
		WriteConsoleOutputCharacter(hConsole, screen, swidth * sheight, { 0,0 }, &dwBytesWritten);
	}
	CloseHandle(hConsole);
	cout << "Game Over... Your score:" << score << endl;
	system("pause");
	return 0;
}