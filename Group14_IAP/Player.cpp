#include "Player.h"
#include <conio.h>
#include <iostream>

void Player::move()
{

	std::cout << "Use WASD to move\n";

	char input;
	input = _getch();

	switch (input) {
	case 'w':
	case 'W':
		std::cout << "Move Up \n";
		break;
	case 's':
	case 'S':
		std::cout << "Move Down \n";
		break;
	case 'a':
	case 'A':
		std::cout << "Move Left \n";
		break;
	case 'd':
	case 'D':
		std::cout << "Move Right \n";
		break;
	default:
		std::cout << "invalid input\n";
		break;
	}
}

