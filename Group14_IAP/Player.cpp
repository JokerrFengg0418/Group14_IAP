#include "Player.h"
#include <conio.h>
#include <iostream>
#include "Position.h"

void Player::move()
{
	int newX = position.row;
	int newY = position.col;

	std::cout << "Use WASD to move\n";

	char input;
	input = _getch();

	switch (input) {
	case 'w':
	case 'W':
		newY--;
		std::cout << "Move Up \n";
		break;
	case 's':
	case 'S':
		newY++;
		std::cout << "Move Down \n";
		break;
	case 'a':
	case 'A':
		newX--;
		std::cout << "Move Left \n";
		break;
	case 'd':
	case 'D':
		newX++;
		std::cout << "Move Right \n";
		break;
	default:
		std::cout << "invalid input\n";
		break;
	}
}

