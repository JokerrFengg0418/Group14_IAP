#include "Combat.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Inventory.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "Option.h"

using namespace std;


Entity* Combat::FactoryCreateEntity(int CharacterType) {

	int RandomTurf = rand() % 40;
	switch (CharacterType) {
	case 0:

		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Monster, 99, 99);

			}
		}
	case 1:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Hellhound, 99, 99);

			}
		}
	case 2:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Zombie, 99, 99);

			}
		}
	case 3:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Goblin, 99, 99);

			}
		}
	case 4:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Bat, 99, 99);

			}
		}
	case 5:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Skeleton, 99, 99);

			}
		}
	case 6:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Gargoyle, 99, 99);

			}
		}
	case 7:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Boss, 99, 99);

			}
		}
	case 8:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Player(0, 0, 100, 50);

			}
		}
	}
}

// External enemy list if used elsewhere
extern Entity* List[20];

int Combat::calculateDistance(const Position& a, const Position& b) {
    int dx = abs(a.getRow() - b.getRow());
    int dy = abs(a.getCol() - b.getCol());
    return max(dx, dy);  // Chebyshev distance
}

void Combat::attack(Entity* Entity1, Inventory* PlayerInventory) {
	std::cout << "\n=== Combat Start ===\n";

	int distance;
	Player* player;

	if (Entity1->getEntityType() == 'E') {

		for (int i = 0; i < 20; i++) {
			if (List[i]->getEntityType() == 'P') {
				distance = calculateDistance(Entity1->getPosition(), List[i]->getPosition());
				player = dynamic_cast<Player*>(List[i]);

			}
		}


		// ============ Enemy's Turn ============
		Enemy* enemyReference = dynamic_cast<Enemy*>(Entity1);
		EnemyType type = enemyReference->getType();
		bool enemyCanMelee = (type == EnemyType::Monster || type == EnemyType::Hellhound ||
			type == EnemyType::Zombie || type == EnemyType::Goblin ||
			type == EnemyType::Bat || type == EnemyType::Skeleton ||
			type == EnemyType::Boss);

		bool enemyCanRange = (type == EnemyType::Gargoyle || type == EnemyType::Boss);

		bool enemyAttacked = false;

		if (enemyCanMelee && distance <= 1) {
			std::cout << enemyReference->getTypeName() << " strikes you in close combat!\n";
			player->takeDamage(enemyReference->getDamage());
			enemyAttacked = true;
		}
		else if (enemyCanRange && distance <= 5) {
			std::cout << enemyReference->getTypeName() << " attacks you from range!\n";
			player->takeDamage(enemyReference->getDamage() - 2);
			enemyAttacked = true;
		}

		if (!enemyAttacked) {
			std::cout << enemyReference->getTypeName() << " is too far to attack.\n";
		}

		// Check if player died after enemy's turn
		if (player->getHealth() <= 0) {
			std::cout << "You have been defeated...\n";

		}

	}

	// ============ Player's Turn ============
	bool attacked = false;
	Inventory* inventory = PlayerInventory;

	if (inventory->getInventory("Sword") != nullptr && distance <= 1) {
		std::cout << "You slash the enemy with your Sword!\n";
		enemy.takeDamage(20);
		attacked = true;
	}
	else if (inventory->getInventory("Bow") != nullptr && distance <= 5) {
		std::cout << "You shoot an arrow at the enemy!\n";
		enemy.takeDamage(15);
		attacked = true;
	}
	else if (inventory->getInventory("Bomb") != nullptr && distance <= 2) {
		std::cout << "You throw a bomb at the enemy!\n";
		enemy.takeDamage(30);
		attacked = true;
	}

	if (!attacked) {
		std::cout << "You are either unarmed or out of range to attack.\n";
	}

	// Check if enemy died after player's turn
	if (enemy.getHealth() <= 0) {
		std::cout << "You defeated the " << enemy.getTypeName() << "!\n";
		
	}

	
	

	std::cout << "=== Combat End ===\n";
}

int Combat::WinCondition()
{
	bool PlayerCheck = false;
	bool EnemyCheck = false;
	for (int i = 0; i < 20; i++)
	{
		if (List[i]->getEntityType() == 'E') {
			EnemyCheck = true;
		}
		if (List[i]->getEntityType() == 'P') {
			PlayerCheck = true;
		}
		if (PlayerCheck == true && EnemyCheck == true) {
			return 0;
		}
		else {
			return 1;
		}

		
	}
	
}

void Combat::TurnOrder()
{
    firstTurn = 1;
	for (int i = 0; i < 20; i++)
	{
        board.drawBoard();
		List[i]->move();
		List[i]->attack();
		if (WinCondition() == 1) {

			std::cout << "Combat Ended \n";

			return;

		}
		firstTurn++;
	}
	std::cout << "Turn Number: " << firstTurn;
}

void Combat::startCombat(char CombatScenario) {

	switch (CombatScenario) {
	case 'A':
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);
		FactoryCreateEntity(0);
		board.addEnemy(List[1]);
		FactoryCreateEntity(0);
		board.addEnemy(List[2]);
		FactoryCreateEntity(0);
		board.addEnemy(List[3]);
		break;
	}


}

void Combat::earnGold(int amount, Inventory* Inventory) {

	int CurrentAmount = Inventory->getCurrency();
	CurrentAmount + amount;
	Inventory->setCurrency(CurrentAmount);

}

int Combat::getGold(Inventory* PlayerInventory) const {

	return PlayerInventory->getCurrency();

}