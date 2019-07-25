//Writer: Coxe (Cloud.Xavier)
//Sponsors: One autumn leaf & The cat looking up at the starry sky
//Product: Pac-Man (on the console)
//Version: 1.0.0_base

#include <iostream>
#include <conio.h>
#include "functions.h"
#include "base_data.h"


void Drawer() {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PacMan.Color);
	SetPos(PacMan.X, PacMan.Y), _putch(Pic[PacMan.Direction]);
	//Load the Pac-Man

	for(int i  = 1; i < 5; i ++) {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), Ghost[i].locked ^ -1 ? Ghost[i].Color : 8);
		SetPos(Ghost[i].X, Ghost[i].Y), _putch('M');
	}
	//Load the Ghost

	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	//Reset the color
} // Color drawer

void Framework() {
	for(int i = 1; i <= Height; i++)
		for(int j = 1; j <= Width; j++) {
			_putch(Save_Map[i][j]);
			if(i == Height && j == Width) continue;
			_putch(j ^ Width ? ' ' : '\n');
		}
	//Load the map

	SetPos(Height + 1, 1), std::cout << "Score: " << Score;
	//Load the Score

	Drawer(); //Load the Ghost and the PacMan
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

void Move_PacMan(void) {
	COORD temp;
	temp.X = PacMan.X + Move[PacMan.Direction][0];
	temp.Y = PacMan.Y + Move[PacMan.Direction][1];
	//Get the new coord

	if(Map[temp.X][temp.Y] == '.')
		SetPos(Height + 1, 1), std::cout << "Score: " << ++Score;
	//Scorer

	if(Map[temp.X][temp.Y] == '@' ) {
		Strong_Time = 20;
		//see the name

		for(int i = 1; i < 5; i++)
			Ghost[i].locked = -1;
		//Lock the ghosts
	} //Check whether ate the strong pill

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

int Change_Direction(int x, int y, int d, int a) {
	int temp = d;

	if(x == PacMan.X) temp = PacMan.Y - y > 0 ? (!PacMan.locked ? 4 : 3) : (!PacMan.locked ? 3 : 4);
	if(y == PacMan.Y) temp = PacMan.X - x > 0 ? (!PacMan.locked ? 2 : 1) : (!PacMan.locked ? 1 : 2);
	if(temp ^ d && Check_Obstacle(x, y, temp)) return temp;
	//The algorithm of grab (or Escape form) the Pac-Manaas

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
	if(Ghost[a].locked > 0) {
		Ghost[a].locked--;
		return;
	} //If the Ghost has die and didn't revival, don't move

	Ghost[a].Direction = Change_Direction(Ghost[a].X, Ghost[a].Y, Ghost[a].Direction, a);
	//Change this ghost's direction

	SetPos(Ghost[a].X, Ghost[a].Y), _putch(Map[Ghost[a].X][Ghost[a].Y]);
	//Clean the passed place

	if(Ghost[a].X > 11 && Ghost[a].X < 15 && Ghost[a].Y == 11) Ghost[a].Direction = 1;

	COORD temp;
	temp.X = Ghost[a].X + Move[Ghost[a].Direction][0];
	temp.Y = Ghost[a].Y + Move[Ghost[a].Direction][1];

	if(temp.X == 14) {
		if(!temp.Y) temp.Y = Width;
		if(temp.Y == Width + 1) temp.Y = 1;
	}
	//Check whether the special coordinate

	Ghost[a].X = temp.X;
	Ghost[a].Y = temp.Y;
}

bool check_alive(int x, int y) {
	for(int i = 1; i < 5; i++)
		if(x == Ghost[i].X && y == Ghost[i].Y) {
			if(Ghost[i].locked > -1) return false; //If Pac-Man isn't strong, eat it

			Ghost[i].X = Save_Ghost[i].X;
			Ghost[i].Y = Save_Ghost[i].Y;
			Ghost[i].Direction = Save_Ghost[i].Direction;
			Ghost[i].locked = 10;
			//Reset this Ghost

			Score += 10;
			SetPos(Height + 1, 1), std::cout << "Score: " << Score;
		}
	return true;
} //Check whether be catched by ghosts

int main() {
	Data_Processing(); //Process the data

	Cursor_Status(false); //Hide the Cursor

	Window_Size_Set(Width, Height + 1); //Resize the window

	Framework(); //Load the map

	int timer_PacMan = 0, timer_Ghost[5]; //see the name
	memset(timer_Ghost, 0, sizeof(timer_Ghost));

	while(check_alive(PacMan.X, PacMan.Y)) { //Continue when alive
		if(_kbhit()) Direction_Controller(_getch()); //Change the PacMan.Direction
		//Move_PacMan(); //See the name

		if(timer_PacMan == 500) Move_PacMan(), timer_PacMan = 0;

		if(!check_alive(PacMan.X, PacMan.Y)) break; //Continue when alive

		Drawer(); //Draw the Ghost and the PacMan

		bool temp = false;

		for(int i = 1; i < 5; i++) {
			if(timer_Ghost[i] == (Ghost[i].locked ? 700 : 500)) {
				Move_Ghost(i); // Move the Ghost
				
				timer_Ghost[i] = 0; // Reset the timer of this ghost
				
				temp = true; // Temporary mark

				if(!Strong_Time && Ghost[i].locked == -1)
					Ghost[i].locked = 0;
				// When the efficacy is over, restore
			}

			timer_Ghost[i] += 25; //see the name
		} // About the Ghost

		Strong_Time -= Strong_Time && temp ? 1 : 0; //see the name

		Drawer(); //Draw the Ghost and the PacMan

		timer_PacMan += 25; //Waiting sum

		Sleep(1); //Step interval
	}
	_getch();
}
