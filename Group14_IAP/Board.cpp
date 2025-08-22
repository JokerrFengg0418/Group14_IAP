#include <iostream>
#include "Board.h"
#include "Enemy.h"
#include <fstream>

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

void Board::initializeDungeonXGrid() {
	for (int i = 0; i < 5; ++i)
		for (int j = 0; j < 5; ++j)
			board[i][j] = 'X';           // fill with X

	board[4][4] = ' '; // spawn cell blank (bottom-right)
}

void Board::setCellContentDungeon(int r, int c, char ch) { board[r][c] = ch; }
char Board::getCellContentDungeon(int r, int c) const { return board[r][c]; }

void Board::drawDungeon() {
	std::cout << "+---------+ \n";
	for (int i = 0; i < 5; ++i) {
		for (int j = 0; j < 5; ++j) {
			char cell = board[i][j];
			std::cout << '|' << (cell ? cell : ' ');
		}
		std::cout << "|\n";
	}
	std::cout << "+---------+ \n";
}

bool Board::save(const char* path) const {
	std::ofstream out(path, std::ios::binary);
	if (!out) return false;
	out.write(reinterpret_cast<const char*>(board), sizeof(board));
	return true;
}

bool Board::load(const char* path) {
	std::ifstream in(path, std::ios::binary);
	if (!in) return false;
	in.read(reinterpret_cast<char*>(board), sizeof(board));
	return in.good();
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

void Board::removeEnemy(Entity* e)
{
	for (int i = 0; i < enemyCount; i++)
	{
		if (enemies[i] == e)
		{
			enemies[i] = nullptr;
			enemies[i] = enemies[--enemyCount]; // Move the last enemy to this position
			enemies[enemyCount] = nullptr; // Clear the last position
			return;
		}
	}
	std::cout << "Enemy not found." << std::endl;
}