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
	setEntityType('P');
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

void Player::move(Entity* List[20])
{
	int newPos = 0;
	char A;
	std::cout << "Use WASD to move\n";

	char input;
	input = _getch();

	switch (input) {
	case 'w':
	case 'W':
		newPos = getCol() - 1;
		A = 'A';
		std::cout << "Move Up \n";
		break;
	case 's':
	case 'S':
		newPos = getCol() + 1;
		A = 'A';
		std::cout << "Move Down \n";
		break;
	case 'a':
	case 'A':
		newPos = getRow() - 1;
		A = 'B';
		std::cout << "Move Left \n";
		break;
	case 'd':
	case 'D':
		newPos = getRow() + 1;
		A = 'B';
		std::cout << "Move Right \n";
		break;
	default:
		std::cout << "invalid input\n";
		break;
	}

	if (newPos < 0 || newPos > 39 || newPos < 0 || newPos > 39)
	{
		return;
	}
	if (A == 'B')
	{
		setCol(newPos);
	}
	else
	{
		setRow(newPos);
	}
	for (int i = 0; i < 20; i++)
	{
		if (List[i] != nullptr)
		{
			int otherx = List[i]->getPosition().getRow();
			int othery = List[i]->getPosition().getCol();
			char othertype = List[i]->getEntityType();
			for (int j = i + 1; j < 20; j++)
			{
				if (List[j] != nullptr && j != i)
				{
					int otherx1 = List[j]->getPosition().getRow();
					int othery1 = List[j]->getPosition().getCol();
					char othertype1 = List[j]->getEntityType();
					if (otherx == otherx1 && othery == othery1)
					{
						if (otherx == otherx1 && othery == othery1)
						{
							if (othertype == 'P' && othertype1 == 'E')
							{
								return;
							}
						}
					}
				}
			}
		}
	}
}








