#include <iostream>
#include "Board.h"
#include "Enemy.h"
#include <Windows.h>


#define FOREGROUND_YELLOW (FOREGROUND_RED | FOREGROUND_GREEN)
#define FOREGROUND_WHITE (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)


Board::Board() : Player(nullptr), enemyCount(0), selectedEnemy(nullptr)
{
	for (int i = 0; i < enemyCount; i++) 
	{
		enemies[i] = nullptr; // Initialize enemy pointers to nullptr
	}

	for (int r = 0; r < 40; r++)
	{
		for (int c = 0; c < 40; c++)
		{
			board[r][c] = ' '; // Initialize the board with empty spaces
		}
	}

	std::cout << "Map created";
}
Board::~Board()
{
	for (int i = 0; i < enemyCount; i++)
	{
		delete enemies[i]; // Delete each enemy entity
	}
	delete Player; // Delete the player entity if it exists
}

void Board::selectEnemy(Entity* e) {
	selectedEnemy = e;
}

void Board::addPlayer(Entity* p)
{
	Player = p;
}

void Board::addEnemy(Entity* e)
{
	if (enemyCount < maxEnemies) // Check if there's space for a new enemy
	{
		enemies[enemyCount++] = e; // Add the enemy and increment the count
	}
	else
	{
		std::cout << "Max enemies reached." << std::endl;
	}
}

void Board::drawBoard()
{
	// Clear Board
	for (int row = 0; row < 40; row++)
	{
		for (int col = 0; col < 40; col++)
		{
			board[row][col] = ' ';
		}
	}

	// Get the handle to the console output
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	// Place entities on the board using their coordinates
	if (Player) {
		board[Player->getRow()][Player->getCol()] = 'P'; // P = Player
	}

	for (int i = 0; i < enemyCount; ++i) {
		if (enemies[i] == selectedEnemy) {
			SetConsoleTextAttribute(hConsole, FOREGROUND_YELLOW);
		}
		else {
			SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE); // Set default color for other enemies
        }

		board[enemies[i]->getRow()][enemies[i]->getCol()] = static_cast<Enemy*>(enemies[i])->getTypeName();
	}

	SetConsoleTextAttribute(hConsole, FOREGROUND_WHITE);

	// Print out Board
	std::cout << "+---------------------------------------+";
	std::cout << std::endl;
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			std::cout << '|' << board[i][j];
		}
		std::cout << '|';
		std::cout << std::endl;
	}
	std::cout << "+----------------------------------------+";
	std::cout << std::endl;
}

// Function to draw Dungeon Layout
void Board::drawDungeon()
{
	std::cout << std::endl;
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			std::cout << '|' << Board::board[i][j];
		}
		std::cout << '|';
		std::cout << std::endl;
	}


	for (int row = 0; row < 5; row++)
	{
		for (int col = 0; col < 5; col++)
		{
			board[row][col] = ' ';
		}
	}
}

char Board::GetBoard() const
{
	return board[40][40];
}