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