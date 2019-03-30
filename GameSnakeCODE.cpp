#include<iostream>
#include<cstdlib>
#include<conio.h>
#include<unistd.h>
#include <windows.h>
#include<ctime>

using namespace std;

bool gameOver;
const int width = 20;										
const int height = 20;
char NextMap[width][height];	
char Map[width][height];
int x, y, fruitX, fruitY, score = 0, move = 0;
int tailX[100], tailY[100];
int nTail;
enum eDirection{ STOP = 0, LEFT, RIGHT, UP, DOWN, ENDING};
eDirection dir;
unsigned int microseconds = 250000;

void optimize() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
}

int irand(int a, int b);

void setCursorPosition(int x, int y)
{
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

void GameOver(){
	sleep(2);
	system("cls");
	setCursorPosition(11, 5);
	cout << "GAME";
	setCursorPosition(12, 6);
	cout << "IS";
	setCursorPosition(11, 7);
	cout << "OVER";
	setCursorPosition(7, 10);
	cout << "Your score: " << score;
	setCursorPosition(3, 13);
	cout << "PRESS 0 to end the game";
	setCursorPosition(100, 100);
}

void Setup()
{
	gameOver = false;
	dir = STOP;
	x = width / 2 - 1;
	y = height / 2 - 1;
	fruitX = irand(0, 20) % (width-2) + 1;
	fruitY = irand(0, 20) % (height-2) + 1;
	score = 0;
	
	for(int i = 0; i < width; i++)
	{
		NextMap[i][0] = '#';
		NextMap[i][height-1] = '#';
	}
	
	for(int i = 1; i < height-1; i++)
	{
		for(int j = 0; j < width; j++)
		{
			Map[j][i] = ' ';
			if((j == 0) || (j == width-1))
				NextMap[j][i] = '#';
			else
				NextMap[j][i] = ' ';
		}
	}
	
	NextMap[fruitX][fruitY] = 'F';
	NextMap[x][y] = '0';
}

void Draw(){
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			if(Map[j][i] != NextMap[j][i])
			{
				Map[j][i] = NextMap[j][i];
				setCursorPosition(j, i);
				cout << Map[j][i];
			}
		}
	}
	setCursorPosition(0, height+1);
	cout << "Your score: " << score;
	
	setCursorPosition(100, 21);
	usleep(microseconds);
}

void Input()
{
	if(_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			dir = LEFT;
			break;
		case 'd':
			dir = RIGHT;
			break;
		case 's':
			dir = DOWN;
			break;
		case 'w':
			dir = UP;
			break;
		case 'x':
			gameOver = true;
			break;
		case  '0':
			dir = ENDING;
			break;
		}
	}
}

void Logic()
{
	int prevX = tailX[0];
	int prevY = tailY[0];
	int prev2X, prev2Y;
	tailX[0] = x;
	tailY[0] = y;
	for(int i = 1; i < nTail; i++)		
	{
		prev2X = tailX[i];				
		prev2Y = tailY[i];			
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	
	
	switch(dir){ 
		case LEFT:
			if(move != 1)
			{
				x--;
				move = 2;
			}
			else
				x++;
			break; 
			
		case RIGHT:
			if(move != 2)
			{
				x++;
				move = 1;
			}
			else
				x--;
			break; 
			
		case DOWN:
			if(move != 3)
			{
				y++;
				move = 4;
			}
			else
				y--;
			break; 
			
		case UP:
			if(move != 4)
			{
				y--;
				move = 3;
			}
			else
				y++; 
			break; 
	}
	
	if(NextMap[x][y] == '#')
		gameOver = true;
		
	for(int i = 0; i < nTail; i++)			
		if((tailX[i] == x) && (tailY[i] == y))
			gameOver = true;
		
	
	if((x == fruitX) && (y == fruitY))
	{			
		score += 10;
		
		fruitX = irand(0, 20) % (width-2) + 1;
		fruitY = irand(0, 20) % (height-2) + 1;
		
		nTail++;
		microseconds -= 10000;	
	}
	
	for(int i = 1; i < height-1; i++)
	{
		for(int j = 1; j < width-1; j++)
		{
			NextMap[j][i] = ' ';
			
			for(int nn = 0; nn < nTail; nn++)
				if((i == tailY[nn]) && (j == tailX[nn]))
					NextMap[j][i] = 'o';
		}
	}
	
	NextMap[x][y] = '0';
	NextMap[fruitX][fruitY] = 'F';
}

int main(void)
{
	srand(time(NULL));
	
	optimize();
	
	Setup();
	while(dir != ENDING){
		while(!gameOver)
 		{
			Draw();
			Input();
			Logic();
		}
			GameOver();
		
		dir = STOP;
		while(dir == STOP)
			Input();
	}
	
	return 0;
}

int irand(int a, int b)
{
	return a + rand() % (b - a + 1);
}
