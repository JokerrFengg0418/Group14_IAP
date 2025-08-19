#include <iostream>
#include "Board.h"
Board::Board()
{

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


	for (int row = 0; row < 20; row++)
	{
		for (int col = 0; col < 20; col++)
		{
			board[row][col] = ' ';
		}
	}
}