#include "Turret.h"
#include "Combat.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Turret::Turret() : Entity(0, 0)
{
	setHealth(50);
	setDamage(0);
	setEntityType('T');
	// Turret Creation
}

Turret::Turret(int r, int c, int dmg) : Entity(r, c)
{
	setRow(r);
	setCol(c);
	setHealth(50);
	setDamage(dmg);
	setEntityType('T');
}

Turret::~Turret()
{
	// Destroy Turret
}

//void Turret::Update(Entity* enemies[], int enemyCount)
//{
//	const int AttackRange = 10;
//	int turretRow = getRow();
//	int turretCol = getCol();
//
//	for (int i = 0; i < 20; ++i)
//	{
//		Entity* enemy = enemies[i];
//		if (enemy != nullptr && enemy->getEntityType() == 'E')
//		{
//
//
//			int enemyRow = enemy->getRow();
//			int enemyCol = enemy->getCol();
//
//			int distance = std::max(std::abs(turretRow - enemyRow), std::abs(turretCol - enemyCol));
//
//			if (distance <= AttackRange)
//			{
//				enemy->takeDamage(getDamage());
//				std::cout << "Turret at (" << turretRow << ", " << turretCol
//					<< ") attacked enemy at (" << enemyRow << ", " << enemyCol
//					<< ") for " << getDamage() << " damage.\n";
//				return;
//			}
//		}
//	}
//}