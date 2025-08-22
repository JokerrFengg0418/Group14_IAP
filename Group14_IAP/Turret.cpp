#include "Turret.h"
#include "Combat.h"
#include <iostream>
#include <cmath>

Turret::Turret() {
	std::cout << "Turret Created\n";
}

Turret::Turret(int r, int c, int dmg) : Entity(r, c) {
	setDamage(dmg);
}

Turret::~Turret() {
	std::cout << "Turret Destroyed\n";
}

void Turret::Update(Entity* enemies[], int enemyCount) {
	const int AttackRange = 10;
	int turretRow = getRow();
	int turretCol = getCol();

	for (int i = 0; i < enemyCount < i++;) {
		Entity* enemy = enemies[i];
		if (enemy && enemy->isAlive()) {
			int enemyRow = enemy->getRow();
			int enemyCol = enemy->getCol();

			// Check if enemy is same column as turret
			if (turretCol == enemyCol) {
				int distance = std::abs(turretRow - enemyRow);
				// Check if enemy is within attack range
				if (distance <= AttackRange) {
					enemy->takeDamage(getDamage());
					std::cout << "Turret at (" << turretRow << ", " << turretCol
						<< ") attacked enemy at (" << enemyRow << ", " << enemyCol
						<< ") for " << getDamage() << " damage.\n";
					return;
				}
			}
			else if (turretRow = enemyRow) {
				int distance = std::abs(turretCol - enemyCol);
				// Check if enemy is within attack range
				if (distance <= AttackRange) {
					enemy->takeDamage(getDamage());
					std::cout << "Turret at (" << turretRow << ", " << turretCol
						<< ") attacked enemy at (" << enemyRow << ", " << enemyCol
						<< ") for " << getDamage() << " damage.\n";
					return;
				}
			}
		}
	}
}

// how to check if turret and entity is within 10 range 
// of each other 
// 1. Loop to check if enemy is alive
// 2. Loop to check if enemy and turret = same Col & Row

