#include "Combat.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Inventory.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include "Option.h"
#include <conio.h>

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
		break;
	case 1:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Hellhound, 99, 99);

			}
		}
		break;
	case 2:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Zombie, 99, 99);

			}
		}
		break;
	case 3:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Goblin, 99, 99);

			}
		}
		break;
	case 4:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Bat, 99, 99);

			}
		}
		break;
	case 5:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Skeleton, 99, 99);

			}
		}
		break;
	case 6:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Gargoyle, 99, 99);

			}
		}
		break;
	case 7:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Enemy(39, 0, EnemyType::Boss, 99, 99);

			}
		}
		break;
	case 8:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				return new Player;

			}
		}
		break;
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
	Entity* player = nullptr;

	if (Entity1->getEntityType() == 'E') {

		for (int i = 0; i < 20; i++) {
			if (List[i]->getEntityType() == 'P') {
				distance = calculateDistance(Entity1->getPosition(), List[i]->getPosition());
				player = List[i];

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

	if (Entity1->getEntityType() == 'P') {

		bool attacked = false;
		Inventory* inventory = PlayerInventory;
		bool Highlightlist[20];

		for (int i = 0; i < 20; i++) {
			Highlightlist[i] = false;
		}

		for (int i = 0; i < 20; i++) {
			if (List[i]->getEntityType() == 'E') {

				distance = calculateDistance(Entity1->getPosition(), List[i]->getPosition());
				if (inventory->getEquippedItem()->GetNumber() < distance) {
					Highlightlist[i] = true;
				}

			}
		}
		// ============ Player's Turn ============
		bool Selection = true;
		int Increment = 0;
		while (Selection == true) {
			if (Highlightlist[Increment] == true) {
				board.printBoardCellColor(List[Increment]->getRow(), List[Increment]->getCol());
				int input = _getch();

				switch (input) {
				case 'a':
					Increment - 1;
					break;
				case 'A':
					Increment - 1;
					break;
				case 'd':
					Increment + 1;
					break;
				case 'D':
					Increment + 1;
					break;
				default:
					break;
				case '\r':
					Selection = false;

				}

			}

		}

		Item* EquippedItem = inventory->getEquippedItem();

		if (EquippedItem != nullptr) {
			std::cout << "You attacked the enemy with your" << EquippedItem->GetItemWord('N') << "! \n";

			List[Increment]->takeDamage(EquippedItem->GetItemValue('V'));
			attacked = true;
		} else if (!attacked) {
			std::cout << "You are either unarmed or out of range to attack.\n";
		}

		// Check if enemy died after player's turn
		if (List[Increment]->getHealth() <= 0) {
			Enemy* EnemyNameReference = dynamic_cast<Enemy*>(List[Increment]);
			std::cout << "You defeated the " << EnemyNameReference->getTypeName() << "!\n";

		}


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
			
			for (int i = 0; i < 20; i++) {
				delete List[i];
				List[i] = nullptr;
			}
			return 1;

		}

		
	}
	
}

void Combat::TurnOrder(Inventory* PlayerInventory)
{
	Entity* List[20];
    firstTurn = 1;
	for (int i = 0; i < 20; i++)
	{
        board.drawBoard();
		List[i]->move();
		attack(List[i], PlayerInventory);
		FactoryDestructor();
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

void Combat::FactoryDestructor() {

	for (int i = 0; i < 20; i++) {
		if (List[i]->getHealth() <= 0) {
			delete List[i];
			List[i] = nullptr;
		}
	}

}