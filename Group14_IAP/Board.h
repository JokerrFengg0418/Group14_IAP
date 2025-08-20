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

	// Function to add Entity
	void addPlayer(Entity* p);
	void addEnemy(Entity* e);

private:
	char board [40][40];
	char dungeon[5][5];
	Entity* Player; // Pointer to player entity

	Entity* selectedEnemy;
	void printBoardCellColor(int row, int col);

	static const int maxEnemies = 20; // Maximum number of enemies
	Entity* enemies[maxEnemies]; // Array of enemy pointers
	int enemyCount; // Current number of enemies on the board
};


