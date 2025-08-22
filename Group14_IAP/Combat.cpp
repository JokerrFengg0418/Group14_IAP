#define NOMINMAX
#include "Combat.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Inventory.h"
#include "Option.h"
#include "Turret.h"
#include <iostream>
#include <cmath>
#include <algorithm>  
#include <cstdlib>
#include <vector>
#include <conio.h>
#include <Windows.h>

using namespace std;


void Combat::FactoryCreateEntity(int CharacterType) {

	int RandomTurf = rand() % 40;
	switch (CharacterType) {
	case 0:

		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List [i] = new Enemy(39, RandomTurf, EnemyType::Monster, 99, 99);
				return;

			}
		}
		break;
	case 1:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Hellhound, 99, 99);
				return;

			}
		}
		break;
	case 2:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0 , RandomTurf, EnemyType::Zombie, 99, 99);
				return;

			}
		}
		break;
	case 3:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Goblin, 99, 99);
				return;

			}
		}
		break;
	case 4:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Bat, 99, 99);
				return;

			}
		}
		break;
	case 5:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Skeleton, 99, 99);
				return;

			}
		}
		break;
	case 6:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Gargoyle, 99, 99);
				return;

			}
		}
		break;
	case 7:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Boss, 99, 99);
				return;

			}
		}
		break;
	case 8:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Player(0, 0, 100, 50);
				return;

			}
		}
		break;
	}
}

// External enemy list if used elsewhere
extern Entity* List[20];

int Combat::calculateDistance(const Position& a, const Position& b) {
	int dx = std::abs(a.getRow() - b.getRow());
	int dy = std::abs(a.getCol() - b.getCol());
	return std::max(dx, dy);  // Chebyshev distance
}

void Combat::attack(Entity* entity1, Inventory* playerInv) {
	if (!entity1) return;
	std::cout << "\n=== Combat Start ===\n";

	// ------------- ENEMY TURN -------------
	if (entity1->getEntityType() == 'E') {
		// Find the player in List
		Entity* player = nullptr;
		for (int i = 0; i < 20; ++i) {
			if (List[i] == nullptr) { continue; }
			if (List[i]->getEntityType() == 'P') {
				player = List[i];
				break;
			}
		}
		if (!player) { std::cout << "[Combat] No player found.\n"; std::cout << "=== Combat End ===\n"; return; }

		int distance = calculateDistance(entity1->getPosition(), player->getPosition());

		Enemy* enemyRef = dynamic_cast<Enemy*>(entity1);
		if (!enemyRef) { std::cout << "[Combat] Bad enemy cast.\n"; std::cout << "=== Combat End ===\n"; return; }

		EnemyType type = enemyRef->getType();
		bool canMelee = (type == EnemyType::Monster || type == EnemyType::Hellhound ||
			type == EnemyType::Zombie || type == EnemyType::Goblin ||
			type == EnemyType::Bat || type == EnemyType::Skeleton ||
			type == EnemyType::Boss);

		bool canRange = (type == EnemyType::Gargoyle || type == EnemyType::Boss);

		bool enemyAttacked = false;
		if (canMelee && distance <= 1) {
			std::cout << enemyRef->getTypeName() << " strikes you in close combat!\n";
			player->takeDamage(enemyRef->getDamage());
			enemyAttacked = true;
		}
		else if (canRange && distance <= 5) {
			std::cout << enemyRef->getTypeName() << " attacks you from range!\n";
			player->takeDamage(std::max(0, enemyRef->getDamage() - 2));
			enemyAttacked = true;
		}
		else {
			std::cout << enemyRef->getTypeName() << " is too far to attack.\n";
		}

		if (player->getHealth() <= 0) {
			std::cout << "You have been defeated...\n";
			std::cout << "=== Combat End ===\n";
			return;
		}
	}

	// ------------- PLAYER TURN -------------
	if (entity1->getEntityType() == 'P') {
		if (!playerInv) { std::cout << "[Combat] Player inventory is null.\n"; std::cout << "=== Combat End ===\n"; return; }

		// Determine attack range
		int range = 1; // default melee
		if (Item* eq = playerInv->getEquippedItem()) {
			// If your item stores range differently, adjust here
			range = std::max(1, eq->GetNumber());
		}

		// Collect targetable enemies
		std::vector<int> candidates;
		for (int i = 0; i < 20; ++i) {
			if (!List[i] || List[i]->getEntityType() != 'E') continue;
			int d = calculateDistance(entity1->getPosition(), List[i]->getPosition());
			if (d <= range) candidates.push_back(i);
		}

		if (candidates.empty()) {
			std::cout << "No enemies in range.\n";
			std::cout << "=== Combat End ===\n";
			return;
		}

		// Selection UI: cycle through only the valid candidates
		size_t sel = 0;
		bool selecting = true;
		while (selecting) {
			int idx = candidates[sel];
			// Highlight the current candidate on your board (adapt if your API differs)
			// (guarding against null just in case)
			if (List[idx]) {
				board.printBoardCellColor(List[idx]->getPosition().getRow(),
					List[idx]->getPosition().getCol());
			}

			std::cout << "Select Enemy [A/D to cycle, ENTER to confirm]\n";
			int key = _getch();
			switch (key) {
			case 'a': case 'A':
				sel = (sel + candidates.size() - 1) % candidates.size();
				break;
			case 'd': case 'D':
				sel = (sel + 1) % candidates.size();
				break;
			case '\r': // Enter
				selecting = false;
				break;
			default:
				// ignore other keys
				break;
			}
		}

		int targetIndex = candidates[sel];
		Entity* target = (targetIndex >= 0 && targetIndex < 20) ? List[targetIndex] : nullptr;
		if (!target) { std::cout << "[Combat] Target went missing.\n"; std::cout << "=== Combat End ===\n"; return; }

		Item* equipped = playerInv->getEquippedItem();
		if (equipped) {
			std::cout << "You attacked the enemy with your " << equipped->GetItemWord('N') << "!\n";
			int dmg = std::max(1, equipped->GetItemValue('V')); // swap to your damage stat if different
			target->takeDamage(dmg);
		}
		else {
			std::cout << "Warning: No item equipped! You punch for 1 damage.\n";
			target->takeDamage(1);
		}

		if (target->getHealth() <= 0) {
			if (Enemy* e = dynamic_cast<Enemy*>(target)) {
				std::cout << "You defeated the " << e->getTypeName() << "!\n";
			}
			else {
				std::cout << "Enemy defeated!\n";
			}
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
		if (List[i] == nullptr) continue;
		if (List[i]->getEntityType() == 'E') {
			EnemyCheck = true;
		}
		if (List[i]->getEntityType() == 'P') {
			PlayerCheck = true;
		}
		if (PlayerCheck == true && EnemyCheck == true) {
			return 0;
		} 

		
	}

	for (int i = 0; i < 20; i++) {
		delete List[i];
		List[i] = nullptr;
	}
	return 1;

}

void Combat::placeTurret(Inventory* playerInventory, Entity* List[], int entityCount)
{
	Item* turretItem = playerInventory->DrawDatabase('W', "Turret");
	if (!turretItem) {
		std::cout << "You don't have a turret to place!" << std::endl;
		return;
	}

	std::cout << "Enter row and column to place the turret: ";
	int row, col;
	std::cin >> row >> col;

	if (row < 0 || row >= 40 || col < 0 || col >= 40) {
		std::cout << "Invalid position for turret!" << std::endl;
		return;
	}

	for (int i = 0; i < entityCount;) {
		if (List[i] != nullptr && List[i]->getRow() == row && List[i]->getCol() == col) {
			std::cout << "Cannot place turret here, position is occupied!" << std::endl;
			return;
		}
	}

	if (entityCount < 20) {
		List[entityCount] = new Turret(row, col, turretItem->GetItemValue('V'));
		playerInventory->RemoveItemFromInventory("    Turret    ", 1);
		std::cout << "Turret placed at (" << row << ", " << col << ")!" << std::endl;
	}
	else {
		std::cout << "Cannot place turret, maximum entities reached!" << std::endl;
	}
}

void Combat::TurnOrder(Inventory* PlayerInventory)
{
    firstTurn = 1;
	while (WinCondition() == 0)
	{
		std::cout << "Turn Number: " << firstTurn << "\n";
		board.drawBoard();
		for (int i = 0; i < 20; i++)
		{
			if (List[i] != nullptr)
			{
				
				List[i]->move(List);
				attack(List[i], PlayerInventory);
				FactoryDestructor();
				
			}
		}
		firstTurn++;
		
		Sleep(200);
		system("cls");
		
	}

	    std::cout << "Combat Ended \n";
		return;
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
		if (List[i] != nullptr) {
			if (List[i]->getHealth() <= 0) {
				std::cout << "Enemy Health:" << List[i]->getHealth() << "\n";
				delete List[i];
				List[i] = nullptr;
			}
		}
	}

}
