#include<iostream>
#include<cstdlib>
#include<conio.h>
#include<unistd.h>
#include <windows.h>
#include<ctime>

using namespace std;

bool gameOver1;
bool gameOver2;
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
unsigned int microseconds = 280000;

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

void MapCreation(){
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
	
	for(int i = 0; i < 4; i++){
		NextMap[i+2][2] = '#';
		NextMap[i+15][2] = '#';
		NextMap[i+2][18] = '#';
		NextMap[i+15][18] = '#';
	}
	for(int i = 0; i < 3; i++){
		NextMap[4][i+5] = '#';
		NextMap[16][i+5] = '#';
		NextMap[4][i+13] = '#';
		NextMap[16][i+13] = '#';
	}
	for(int i = 0; i < 2; i++){
		NextMap[8][i+1] = '#';
		NextMap[12][i+1] = '#';
		NextMap[8][i+18] = '#';
		NextMap[12][i+18] = '#';
	}
	for(int i = 0; i < 7; i++){
		NextMap[i+7][5] = '#';
		NextMap[i+7][15] = '#';
	}
	for(int i = 0; i < 5; i++){
		NextMap[i+3][10] = '#';
		NextMap[i+13][10] = '#';
	}
	for(int i = 0; i < 3; i++){
		NextMap[10][i+7] = '#';
		NextMap[10][i+11] = '#';
	}
		
	NextMap[1][5]= '#';
	NextMap[19][5] = '#';
	NextMap[1][15] = '#';
	NextMap[19][15] = '#';
	
	NextMap[fruitX1][fruitY1] = 'F';
	NextMap[fruitX2][fruitY2] = 'F';
	NextMap[x1][y1] = '0';
	NextMap[x2][y2] = '0';
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
	setCursorPosition(8, 9);
	
	
	if(gameOver1 == gameOver2){
		if(score1 == score2)
			cout << "DRAW MATCH";
		else
			if(score1 > score2)
				cout << "Player1 wins!";
			else
				cout << "Player2 wins!";
	}
	else{
		if(gameOver2)
			cout << "Player1 wins!";
		if(gameOver1)
			cout << "Player2 wins!";
	}
	
	setCursorPosition(4, 10);
	cout << "PRESS 0 to end the game";
	setCursorPosition(100, 100);
}

void randFruit(int& fruitX, int& fruitY){
	tryAgain:
		
	fruitX = irand(0, 1000) % (width-2) + 1;
	fruitY = irand(0, 1000) % (height-2) + 1;

	if(NextMap[fruitX][fruitY] == '#')
		goto tryAgain;
}

void Moving(int a, int& x, int& y, int& move){
	switch(KeyBoard[a]){ 
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
}

void tailInfo(int *tailX, int *tailY, int& nTail, int& x, int& y){
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
}

void FruitEating(int x, int y, int& nTail, int& score){
	if((x == fruitX1) && (y == fruitY1))
	{			
		score += 10;
		
		randFruit(fruitX1, fruitY1);	
		
		nTail++;	
	}
	if((x == fruitX2) && (y == fruitY2))
	{			
		score += 10;
		
		randFruit(fruitX2, fruitY2);	
		
		nTail++;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////Main logical functions//////////////////////////////////////////

void Setup(){
	MapCreation();
	
	gameOver1 = false;
	gameOver2 = false;
	/////////////////////////////////////////
	KeyBoard[0] = STOP;
	KeyBoard[1] = STOP;
	/////////////////////////////////////////
	x1 = width/2;
	y1 = 2;
	x2 = width/2;
	y2 = height-3;
	/////////////////////////////////////////
	randFruit(fruitX1, fruitY1);
	randFruit(fruitX2, fruitY2);
	/////////////////////////////////////////
	score1 = 0;
	score2 = 0;
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
			gameOver1 = true;
			gameOver2 = true;
			break;		
		}
	}
	
}

void Logic(){
	tailInfo(tailX1, tailY1, nTail1, x1, y1);
	tailInfo(tailX2, tailY2, nTail2, x2, y2);
	
	Moving(0, x1, y1, move1);
	Moving(1, x2, y2, move2);
	
	/////////////////////////////// Game ending ////////////////////////////////////////
	
	if(NextMap[x1][y1] == '#')
		gameOver1 = true;
	if(NextMap[x2][y2] == '#')
		gameOver2 = true;
		
	if((x1 == x2) && (y1 == y2)){
		gameOver1 = true;
		gameOver2 = true;
	}
	
	for(int i = 0; i < nTail1; i++)
	{				
		if(((tailX1[i] == x1) && (tailY1[i] == y1)) || ((tailX1[i] == x2) && (tailY1[i] == y2)))
			gameOver1 = true;
	}
	for(int i = 0; i < nTail2; i++)
	{				
		if(((tailX2[i] == x1) && (tailY2[i] == y1)) || ((tailX2[i] == x2) && (tailY2[i] == y2)))
			gameOver2 = true;
	}
	
	if(score1 == 250)
		gameOver2 = true;
	if(score2 == 250)
		gameOver1 = true;
	
/////////////////////////////////////////////////////////////////////////////////////////////////
	
	FruitEating(x1, y1, nTail1, score1);
	FruitEating(x2, y2, nTail2, score2);
	
////////////////////////////////////////Map drawing//////////////////////////////////////////////
	
	for(int i = 1; i < height-1; i++)
	{
		for(int j = 1; j < width-1; j++)
		{
			if(NextMap[j][i] != '#')
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
		while(!gameOver1 && !gameOver2)
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
