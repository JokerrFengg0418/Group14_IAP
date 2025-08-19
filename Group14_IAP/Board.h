#pragma once
class Board
{
public:
	//Constructors//
	Board();

	//Destructors
	~Board();
	//Getters//

	//Setters//
	
	//Functions//
	void drawBoard();
	void drawDungeon();
private:
	char board [40][40];
	char dungeon[4][4];
};


