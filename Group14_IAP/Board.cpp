#include <iostream>
#include "Board.h"
#include "Enemy.h"

// Define ANSI color codes
const char* const YELLOW = "\x1b[33m";
const char* const RESET = "\x1b[0m";

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

	std::cout << "Map created\n";
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

void Board::printBoardCellColor(int row, int col)
{
	bool isSelected = false;

	// Check if the current enemy at this position is the one selected
	for (int k = 0; k < enemyCount; ++k) {
		if (enemies[k]->getRow() == row && enemies[k]->getCol() == col) {
			isSelected = true;
			break;
		}
	}

	if (isSelected) {
		
		system("cls");
		std::cout << "+-------------------------------------------------------------------------------+ \n";

		for (int i = 0; i < 40; i++) {
			for (int j = 0; j < 40; j++) {
				std::cout << "|";
				if (i == row && j == col){
					std::cout << YELLOW << board[row][col] << RESET;
				}
				else {
					std::cout << board[i][j];
				}
			}
			std::cout << '|';
			std::cout << '\n';
		}
		std::cout << "+-------------------------------------------------------------------------------+ \n";

		
	}
	else {
		std::cout << board[row][col];
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

	// Place entities on the board using their coordinates
	if (Player) {
		board[Player->getRow()][Player->getCol()] = 'P'; // P = Player
	}

	for (int i = 0; i < enemyCount; ++i) {
		
		if (enemies[i]) {
			board[enemies[i]->getRow()][enemies[i]->getCol()] = static_cast<Enemy*>(enemies[i])->getTypeName();
		}
	}

	std::string Template;
	// Print out Board
	Template.append("+-------------------------------------------------------------------------------+ \n");

	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			Template.append("|"); 
			Template.append(1, board[i][j]);
		}
		Template.append(1,'|');
		Template.append(1, '\n');
	}
	Template.append("+-------------------------------------------------------------------------------+ \n");
	std::cout << Template;
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

void Board::clearBoard()
{
	for (int row = 0; row < 40; row++)
	{
		for (int col = 0; col < 40; col++)
		{
			board[row][col] = ' ';
		}
	}
	delete Player;
	Player = nullptr;       
	delete selectedEnemy;
	selectedEnemy = nullptr;    

	for (int i = 0; i < enemyCount; i++)
	{
		delete enemies[i];
		enemies[i] = nullptr;   
	}

	enemyCount = 0;
}