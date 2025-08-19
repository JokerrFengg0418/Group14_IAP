#include <iostream>
#include "Board.h"
Board::Board()
{
	std::cout << "Map created";
}
Board::~Board()
{

}
void Board::drawBoard()
{
	std::cout << "+---------------------------------------+";
	std::cout << std::endl;
	for (int i = 0; i < 40; i++) {
		for (int j = 0; j < 40; j++) {
			std::cout << '|' << Board::board[i][j];
		}
		std::cout << '|';
		std::cout << std::endl;
	}
	std::cout << "+----------------------------------------+";
	std::cout << std::endl;


	for (int row = 0; row < 40; row++)
	{
		for (int col = 0; col < 40; col++)
		{
			board[row][col] = ' ';
		}
	}
}

void Board::drawDungeon()
{
	std::cout << std::endl;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			std::cout << '|' << Board::board[i][j];
		}
		std::cout << '|';
		std::cout << std::endl;
	}


	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			board[row][col] = ' ';
		}
	}
}