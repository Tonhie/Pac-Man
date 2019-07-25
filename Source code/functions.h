#include <windows.h>
#include <stdio.h>
#include <conio.h>

void Window_Size_Set(int x, int y) {
	char temp[26];
	sprintf(temp, "mode con cols=%d lines=%d", x << 1, y);
	system(temp);
} //Resize the Window

void Cursor_Status(bool temp) {
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(handle, &CursorInfo);
	CursorInfo.bVisible = temp;
	SetConsoleCursorInfo(handle, &CursorInfo);
} //Hide the cursor

void SetPos(int x, int y) {
	COORD setps;
	setps.X = (y << 1) - 2, setps.Y = x - 1;
	HANDLE hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hCon, setps);
} //Move the cursor

void Quit_Tip() {
	if(MessageBox(NULL, TEXT("Quit the game?"), TEXT("Tip"), MB_YESNO) == IDYES) exit(0);
} //Ask whether end the gamem