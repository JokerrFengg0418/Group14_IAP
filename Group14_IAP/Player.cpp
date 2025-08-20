#include "Player.h"
#include "Enemy.h"
#include "Inventory.h"
#include <conio.h>
#include <iostream>
#include "Position.h"
#include "Shop.h"


Player::Player()
{
	Health = 100;
	std::cout << "Player Created \n";
}

Player::Player(int r, int c, int hp, int dmg)
{
	setRow(r);
	setCol(c);
	setHealth(hp);   // 
	setDamage(dmg);  // 
	std::cout << "Player Created at (" << r << ", " << c << ")" << ", health: " << getHealth() << ", damage: " << getDamage() << std::endl;
}

Player::~Player()
{
}


// Player Behaviours 
void Player::takeDamage(int amount)
{
	setHealth(getHealth() - amount);
	if (getHealth() <= 0) {
		setHealth(0);
		std::cout << " Player has been defeated!" << std::endl;
	}
}

void Player::attack(Enemy* enemy) {
	if (enemy != nullptr) {
		enemy->takeDamage(getDamage());
		std::cout << "Player attacks the enemy, dealing " << getDamage() << " damage." << std::endl;
	}
}

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







