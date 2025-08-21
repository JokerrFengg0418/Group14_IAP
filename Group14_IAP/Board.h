#pragma once
#include "Player.h"
class Board
{
public:
	//Constructors//
	Board();

	//Destructors
	~Board();
	void selectEnemy(Entity* e);
	//Getters//
	char GetBoard() const;
	//Setters//
	
	//Functions
	void drawBoard();
	void drawDungeon();
	void clearBoard();

	// Function to add Entity
	void addPlayer(Entity* p);
	void addEnemy(Entity* e);
	void printBoardCellColor(int row, int col);

private:
	char board [40][40];
	char dungeon[5][5];
	Entity* Player; // Pointer to player entity

	Entity* selectedEnemy;
	

	static const int maxEnemies = 20; // Maximum number of enemies
	Entity* enemies[maxEnemies]; // Array of enemy pointers
	int enemyCount; // Current number of enemies on the board
};


