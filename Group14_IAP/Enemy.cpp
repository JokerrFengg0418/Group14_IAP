#include "Enemy.h"
#include <iostream>

// Default Constructor
Enemy::Enemy()
{
	std::cout << "Enemy Created." << std::endl;
}

// Overloaded Constructor to set initial position, type, health, and damage
Enemy::Enemy(int r, int c, EnemyType type, int hp, int dmg)
{
	setRow(r);
	setCol(c);
	this->type = type;
	setEntityType('E');
	setHealth(hp);   // 
	setDamage(dmg);  // 
	std::cout << "Enemy Created at (" << r << ", " << c << ") with type: "
		<< static_cast<int>(type) << ", health: " << getHealth()
		<< ", damage: " << getDamage() << std::endl;
}

 
// Destructor
Enemy::~Enemy()
{
	std::cout << "Enemy Destroyed." << std::endl;
}

// Getters
EnemyType Enemy::getType() const
{
	return type;
}

// Returns a character representation of the enemy type defined in Enemy.h as a enum
char Enemy::getTypeName() const 
{ 
	switch (type) {
	case EnemyType::Monster:   return 'M';
	case EnemyType::Hellhound: return 'H';
	case EnemyType::Zombie:    return 'Z';
	case EnemyType::Goblin:    return 'G';
	case EnemyType::Bat:       return 'B';
	case EnemyType::Skeleton:  return 'S';
	case EnemyType::Gargoyle:  return 'G';
	case EnemyType::Boss:      return 'A';
	default:                   return 'U';
	}
}


// Enemy Behaviours 
void Enemy::takeDamage(int amount)
{
	setHealth(getHealth() - amount);
	if (getHealth() <= 0) {
		setHealth(0);
		std::cout << getTypeName() << " has been defeated!" << std::endl;
	}
}


void Enemy::move(Entity* List[20])
{
	// Example: Move randomly in one of the four directions
	int direction = rand() % 4; // 0: up, 1: down, 2: left, 3: right
	int newPos;
	char A;
	switch (direction) {
	case 0:
		newPos = getRow() - 1; // Move up
		A = 'A';
		break;
	case 1:
		newPos = getRow() + 1; // Move down
		A = 'A';
		break;
	case 2:
		newPos = getCol() - 1; // Move left
		A = 'B';
		break;
	case 3:
		newPos = getCol() + 1; // Move right
		A = 'B';
		break;
	}

	if (newPos < 0 || newPos > 39 || newPos < 0 || newPos > 39)
	{
		return;
	}
	else
	{
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
							if (othertype == 'P' && othertype1 == 'E')
							{
								return;
							}
							else if (othertype == 'E' && othertype1 == 'E')
							{
								return;
							}
						}
					}
				}
			}
		}
		if (A == 'A')
		{
			setRow(newPos);
		}
		else
		{
			setCol(newPos);
		}
	}
}

// Debug
void Enemy::printStatus() const
{
	std::cout << "Enemy Type: " << getTypeName() << ", Health: " << getHealth()
		<< ", Damage: " << getDamage() << ", Location: ("
		<< getRow() << ", " << getCol() << ")" << std::endl;
}

