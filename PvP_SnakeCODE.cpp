#include<iostream>
#include<cstdlib>
#include<conio.h>
#include<unistd.h>
#include <windows.h>
#include<ctime>

#define fruits fruitX1, fruitY1, fruitX2, fruitY2
#define INTfruits int fruitX1, int fruitY1, int fruitX2, int fruitY2

using namespace std;

bool drw;
bool gameOver;
const int width = 21;										
const int height = 21;
const int STOP = 0, LEFT = 1, RIGHT = 2, UP = 3, DOWN = 4, ENDING = 5;
char NextMap[width][height];
char Map[width][height];
int x1, y1, fruitX1, fruitY1, score1 = 0, move1 = 0, nTail1;
int x2, y2, fruitX2, fruitY2, score2 = 0, move2 = 0, nTail2;
int tailX1[100], tailY1[100];
int tailX2[100], tailY2[100];
int KeyBoard[2] = {0, 0};
unsigned int microseconds = 250000;

////////////////////////////////////////System functions/////////////////////////////////////////

void optimize() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);
}

int irand(int a, int b){
	return a + rand() % (b - a + 1);
}

void setCursorPosition(int x, int y){
    static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
}

/////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////Logical events in game/////////////////////////////////////////

void GameOver(){
	sleep(2);
	system("cls");
	setCursorPosition(11, 5);
	cout << "GAME";
	setCursorPosition(12, 6);
	cout << "IS";
	setCursorPosition(11, 7);
	cout << "OVER";
	setCursorPosition(8, 9);
	if(score1 > score2)
		cout << "Player1 wins!";
	if(score1 < score2)
		cout << "Player2 wins!";
	if(score1 == score2)
		cout << "DRAW MATCH";
	setCursorPosition(4, 10);
	cout << "PRESS 0 to end the game";
	setCursorPosition(100, 100);
}

/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////Main logical functions//////////////////////////////////////////

void Setup(){
	gameOver = false;
	drw = false;
	/////////////////////////////////////////
	KeyBoard[0] = STOP;
	KeyBoard[1] = STOP;
	/////////////////////////////////////////
	x1 = width-10;
	y1 = 2;
	x2 = width-10;
	y2 = height-3;
	/////////////////////////////////////////
	fruitX1 = irand(0, 20) % (width-2) + 1;
	fruitY1 = irand(0, 20) % (height-2) + 1;	
	fruitX2 = irand(0, 20) % (width-2) + 1;
	fruitY2 = irand(0, 20) % (height-2) + 1;
	/////////////////////////////////////////
	score1 = 0;
	score2 = 0;
	
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
	
	NextMap[fruitX1][fruitY1] = 'F';
	NextMap[fruitX2][fruitY2] = 'F';
	NextMap[x1][y1] = '0';
	NextMap[x2][y2] = '0';
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
	cout << "Player1 score: " << score1;
	setCursorPosition(0, height+2);
	cout << "Player2 score: " << score2;
	
	setCursorPosition(100, 21);
	usleep(microseconds);
}

void Input(){
	if(_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			KeyBoard[0] = LEFT;
			break;
		case 'd':
			KeyBoard[0] = RIGHT;
			break;
		case 's':
			KeyBoard[0] = DOWN;
			break;
		case 'w':
			KeyBoard[0] = UP;
			break;
////////////////////////////////
		case '4':
			KeyBoard[1] = LEFT;
			break;
		case '6':
			KeyBoard[1] = RIGHT;
			break;
		case '5':
			KeyBoard[1] = DOWN;
			break;
		case '8':
			KeyBoard[1] = UP;
			break;
////////////////////////////////
		case  '0':
			KeyBoard[0] = ENDING;
			break;
		case 'x':
			gameOver = true;
			break;		
		}
	}
	if(_kbhit())
	{
		switch (_getch())
		{
		case 'a':
			KeyBoard[0] = LEFT;
			break;
		case 'd':
			KeyBoard[0] = RIGHT;
			break;
		case 's':
			KeyBoard[0] = DOWN;
			break;
		case 'w':
			KeyBoard[0] = UP;
			break;
////////////////////////////////
		case '4':
			KeyBoard[1] = LEFT;
			break;
		case '6':
			KeyBoard[1] = RIGHT;
			break;
		case '5':
			KeyBoard[1] = DOWN;
			break;
		case '8':
			KeyBoard[1] = UP;
			break;
////////////////////////////////
		case  '0':
			KeyBoard[0] = ENDING;
			break;
		case 'x':
			gameOver = true;
			break;		
		}
	}
}

void Logic(){
	int prevX1 = tailX1[0];
	int prevY1 = tailY1[0];
	int prev2X1, prev2Y1;
	tailX1[0] = x1;
	tailY1[0] = y1;
	for(int i = 1; i < nTail1; i++)		
	{
		prev2X1 = tailX1[i];				
		prev2Y1 = tailY1[i];	
		tailX1[i] = prevX1;
		tailY1[i] = prevY1;
		prevX1 = prev2X1;
		prevY1 = prev2Y1;
	}
	
	int prevX2 = tailX2[0];
	int prevY2 = tailY2[0];
	int prev2X2, prev2Y2;
	tailX2[0] = x2;
	tailY2[0] = y2;
	for(int i = 1; i < nTail2; i++)		
	{
		prev2X2 = tailX2[i];				
		prev2Y2 = tailY2[i];	
		tailX2[i] = prevX2;
		tailY2[i] = prevY2;
		prevX2 = prev2X2;
		prevY2 = prev2Y2;
	}	
	
	switch(KeyBoard[0]){ 
		case LEFT:
			if(move1 != 1)
			{
				x1--;
				move1 = 2;
			}
			else
				x1++;
			break; 
			
		case RIGHT:
			if(move1 != 2)
			{
				x1++;
				move1 = 1;
			}
			else
				x1--;
			break; 
			
		case DOWN:
			if(move1 != 3)
			{
				y1++;
				move1 = 4;
			}
			else
				y1--;
			break; 
			
		case UP:
			if(move1 != 4)
			{
				y1--;
				move1 = 3;
			}
			else
				y1++; 
			break; 
	}
	switch(KeyBoard[1]){ 
		case LEFT:
			if(move2 != 1)
			{
				x2--;
				move2 = 2;
			}
			else
				x2++;
			break; 
			
		case RIGHT:
			if(move2 != 2)
			{
				x2++;
				move2 = 1;
			}
			else
				x2--;
			break; 
			
		case DOWN:
			if(move2 != 3)
			{
				y2++;
				move2 = 4;
			}
			else
				y2--;
			break; 
			
		case UP:
			if(move2 != 4)
			{
				y2--;
				move2 = 3;
			}
			else
				y2++; 
			break; 
	}
	
	
	/////////////////////////////// GAME ENDING ////////////////////////////////////////
	if(NextMap[x1][y1] == '#')
		gameOver = true;
	if(NextMap[x2][y2] == '#')
		gameOver = true;
		
	if((x1 == x2) && (y1 == y2)){
		gameOver = true;
		drw = true;
	}
	
	for(int i = 0; i < nTail1; i++)
	{				
		if(((tailX1[i] == x1) && (tailY1[i] == y1)) || ((tailX1[i] == x2) && (tailY1[i] == y2)))
			gameOver = true;
	}
	for(int i = 0; i < nTail2; i++)
	{				
		if(((tailX2[i] == x1) && (tailY2[i] == y1)) || ((tailX2[i] == x2) && (tailY2[i] == y2)))
			gameOver = true;
	}
	///////////////////////////////////////////////////////////////////////////////////////
	
	if((x1 == fruitX1) && (y1 == fruitY1))
	{			
		score1 += 10;
		
		fruitX1 = irand(0, 20) % (width-2) + 1;
		fruitY1 = irand(0, 20) % (height-2) + 1;
		
		nTail1++;
		microseconds -= 1000;	
	}
	if((x1 == fruitX2) && (y1 == fruitY2))
	{			
		score1 += 10;
		
		fruitX2 = irand(0, 20) % (width-2) + 1;
		fruitY2 = irand(0, 20) % (height-2) + 1;
		
		nTail1++;
		microseconds -= 1000;	
	}
	
	
	
	if((x2 == fruitX1) && (y2 == fruitY1))
	{			
		score2 += 10;
		
		fruitX1 = irand(0, 20) % (width-2) + 1;
		fruitY1 = irand(0, 20) % (height-2) + 1;
		
		nTail2++;
		microseconds -= 1000;	
	}
	if((x2 == fruitX2) && (y2 == fruitY2))
	{			
		score1 += 10;
		
		fruitX2 = irand(0, 20) % (width-2) + 1;
		fruitY2 = irand(0, 20) % (height-2) + 1;
		
		nTail2++;
		microseconds -= 1000;	
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////
	
	for(int i = 1; i < height-1; i++)
	{
		for(int j = 1; j < width-1; j++)
		{
			NextMap[j][i] = ' ';
			
			for(int nn = 0; nn < nTail1; nn++)
				if((i == tailY1[nn]) && (j == tailX1[nn]))
					NextMap[j][i] = 'o';
					
			for(int nn = 0; nn < nTail2; nn++)
				if((i == tailY2[nn]) && (j == tailX2[nn]))
					NextMap[j][i] = 'o';
		}
	}
	
	NextMap[fruitX1][fruitY1] = 'F';
	NextMap[fruitX2][fruitY2] = 'F';
	NextMap[x1][y1] = '0';
	NextMap[x2][y2] = '0';
}

/////////////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////Main///////////////////////////////////////////////////

int main(void){
	srand(time(NULL));
	
	optimize();
	
	Setup();
	while(KeyBoard[0] != ENDING){
		while(!gameOver)
 		{
		 	Draw();
			Input();
			Logic();
		}
			GameOver();
			Input();
	}
	
	return 0;
}
