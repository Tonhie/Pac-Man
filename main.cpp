#include <iostream>
#include <conio.h>
#include "functions.h"
#include "base_data.h"

void Framework() {
	for(int i = 1; i <= Height; i++)
		for(int j = 1; j <= Width; j++) {
			_putch(Save_Map[i][j]);
			if(i == Height && j == Width) continue;
			_putch(j ^ Width ? ' ' : '\n');
		}
	//Load the map

	for(int i = 1; i < 5; i++)
		SetPos(Ghost[i].X, Ghost[i].Y), _putch('M');
	//Load the Ghost

	SetPos(Height + 1, 1), std::cout << "Score: " << Score;
	//Load the Score

	SetPos(PacMan.X, PacMan.Y), _putch(Pic[PacMan.Direction]);
	//Load the Pac-Man
}

void Direction_Controller(char temp) { 
	int record = PacMan.Direction;

	if(temp == 'p') while(_getch() ^ 'c');
	//Check the pause order

	if(temp == (char)(27)) Quit_Tip();
	//Check the quit order

	if(temp == 'w') PacMan.Direction = 1;
	if(temp == 's') PacMan.Direction = 2;
	if(temp == 'a') PacMan.Direction = 3;
	if(temp == 'd') PacMan.Direction = 4;
	//Check what the direction going to change

	if(Map[PacMan.X + Move[PacMan.Direction][0]][PacMan.Y + Move[PacMan.Direction][1]] == '#')
		PacMan.Direction = record;
	//If the next pos can't walk, then not change the direction
}

void Move_PacMan() {
	COORD temp;
	temp.X = PacMan.X + Move[PacMan.Direction][0];
	temp.Y = PacMan.Y + Move[PacMan.Direction][1];
	//Get the new coord

	if(Map[temp.X][temp.Y] == '.')
		SetPos(Height + 1, 1), std::cout << "Score: " << ++Score;
	//Scorer

	if(Map[temp.X][temp.Y] == '#') return;
	//Check whether PacMan can go, if not, then not go

	SetPos(PacMan.X, PacMan.Y), _putch(' ');
	Map[PacMan.X][PacMan.Y] = ' ';
	//Clean the passed place and the record

	if(temp.X == 14) {
		if(!temp.Y) temp.Y = Width;
		if(temp.Y == Width + 1) temp.Y = 1;
	} //Check special coordinate

	PacMan.X = temp.X, PacMan.Y = temp.Y;
	//Give the new coord to PacMan
}

bool Check_Obstacle(int x, int y, int d) {
	while(x ^ PacMan.X || y ^ PacMan.Y) {
		x += Move[d][0], y += Move[d][1];
		if(Map[x][y] == '#') return false;
	}
	return true;
}

int Change_Direction(int x, int y, int d) {
	int temp = d;

	if(!Strong_Time) {
		if(x == PacMan.X) temp = PacMan.Y - y > 0 ? 4 : 3;
		if(y == PacMan.Y) temp = PacMan.X - x > 0 ? 2 : 1;
		if(temp ^ d && Check_Obstacle(x, y, temp)) return temp;
	} //The algorithm of grab the Pac-Manaas

	if(!Turnings[x][y]) return d; //If it's not a turning, don't change the direction

	d = d ^ 1 ? (d ^ 2 ? (d ^ 3 ? 3 : 4) : 1) : 2;
	//Reverse process

	temp = (rand() & 3) + 1;
	while(Map[x + Move[temp][0]][y + Move[temp][1]] == '#' || temp == d)
		temp = (rand() & 3) + 1;
	//Find a reasonable value of "temp"

	return temp;
}

void Move_Ghost(int a) {
	Ghost[a].Direction = Change_Direction(Ghost[a].X, Ghost[a].Y, Ghost[a].Direction);
	//Change this ghost's direction

	SetPos(Ghost[a].X, Ghost[a].Y), _putch(Map[Ghost[a].X][Ghost[a].Y]);
	//Clean the passed place

	bool record = false;

	if(Ghost[a].X == 14) {
		if(!Ghost[a].Y) Ghost[a].Y = Width, record = true;
		if(Ghost[a].Y == Width + 1) Ghost[a].Y = 1, record = true;
	}
	if(Ghost[a].X > 11 && Ghost[a].X <15 && Ghost[a].Y == 11) Ghost[a].Direction = 1;
	//Check whether the special coordinate

	if(!record) {
		Ghost[a].X += Move[Ghost[a].Direction][0];
		Ghost[a].Y += Move[Ghost[a].Direction][1];
	} //If it's normal, get the new Coordinate
}

void Drawer() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Fyellow);
	SetPos(PacMan.X, PacMan.Y), _putch(Pic[PacMan.Direction]);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Fred);
	SetPos(Ghost[1].X, Ghost[1].Y), _putch('M');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_green);
	SetPos(Ghost[2].X, Ghost[2].Y), _putch('M');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_blue);
	SetPos(Ghost[3].X, Ghost[3].Y), _putch('M');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), F_purple);
	SetPos(Ghost[4].X, Ghost[4].Y), _putch('M');
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Fwhite);
}

bool check_alive(int x, int y) {
	for(int i = 1; i < 5; i++)
		if(x == Ghost[i].X && y == Ghost[i].Y && !Strong_Time) return false;
	return true;
} //Check whether be catched by ghosts

int main() {
	Data_Processing(); //Process the data
	
	Cursor_Status(false); //Hide the Cursor
	
	Window_Size_Set(Width, Height + 1); //Resize the window
	
	Framework(); //Load the map
	
	while(check_alive(PacMan.X, PacMan.Y)) { //Continue when alive
		if(_kbhit()) Direction_Controller(_getch()); //Change the PacMan.Direction
		Move_PacMan(); //See the name

		if(!check_alive(PacMan.X, PacMan.Y)) break; //Continue when alive
		
		Drawer(); //Draw the Ghost and the PacMan

		for(int i = 1; i < 5; i++)
			Move_Ghost(i);
		//See the name;
		
		Drawer(); //Draw the Ghost and the PacMan

		Sleep(350); //Step interval
	}
	_getch();
}