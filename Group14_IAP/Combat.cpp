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
#include <limits>
#include <string>
#include <cctype>
#include <conio.h>
#include <Windows.h>

using namespace std;


void Combat::FactoryCreateEntity(int CharacterType) {

	int RandomTurf = rand() % 25;
	switch (CharacterType) {
	case 0:

		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List [i] = new Enemy(24, RandomTurf, EnemyType::Monster, 99, 99);
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

// simple local helper
static void clearCinLocal() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Helper: return a lowercase copy of a string (ASCII-safe)
static inline std::string toLowerCopy(std::string s) {
	for (auto& c : s)
		c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
	return s;
}

// Open inventory while in combat. Returns when user presses E.
static void openInventoryDuringCombatByName(Inventory* inv) {
	if (!inv) return;

	while (true) {
		std::cout << "\n=== INVENTORY (Combat) ===\n";
		inv->DrawInventory();
		std::cout << "Commands:\n";
		std::cout << "  w <name>  = equip weapon by name (partial ok)\n";
		std::cout << "  a <name>  = equip armor  by name (partial ok)\n";
		std::cout << "  uw        = unequip weapon\n";
		std::cout << "  ua        = unequip armor\n";
		std::cout << "  e         = exit\n> ";

		std::string line;
		std::getline(std::cin >> std::ws, line);
		if (line.empty()) continue;

		std::string lower = toLowerCopy(line);
		if (lower == "e") return;
		if (lower == "uw") { inv->unequipWeapon(); continue; }
		if (lower == "ua") { inv->unequipArmor();  continue; }

		// Parse "w ..." or "a ..."
		if (lower.size() > 2 && lower[0] == 'w' && lower[1] == ' ') {
			std::string name = line.substr(2); // keep original case/padding for matching
			inv->equipWeaponByName(name);
			continue;
		}
		if (lower.size() > 2 && lower[0] == 'a' && lower[1] == ' ') {
			std::string name = line.substr(2);
			inv->equipArmorByName(name);
			continue;
		}

		// Fallback: auto-route (weapon then armor)
		inv->equipByName(line);
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
			if (List[i] == nullptr) continue;
			if (List[i]->getEntityType() == 'P') { player = List[i]; break; }
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

		// Armor mitigation (flat; tweak as you like)
		int mitigation = 0;
		if (playerInv) {
			if (Item* armor = playerInv->getEquippedArmor()) {
				// Simple model: reduce by V/10 (clamped >=0); adjust to your stat model if needed
				mitigation = std::max(0, armor->GetItemValue('V') / 10);
			}
		}

		bool enemyAttacked = false;
		if (canMelee && distance <= 1) {
			int raw = enemyRef->getDamage();
			int finalDmg = std::max(0, raw - mitigation);
			std::cout << enemyRef->getTypeName() << " strikes you in close combat!\n";
			if (mitigation > 0) std::cout << "(Your armor reduces damage by " << mitigation << ")\n";
			player->takeDamage(finalDmg);
			enemyAttacked = true;
		}
		else if (canRange && distance <= 5) {
			int raw = std::max(0, enemyRef->getDamage() - 2);
			int finalDmg = std::max(0, raw - mitigation);
			std::cout << enemyRef->getTypeName() << " attacks you from range!\n";
			if (mitigation > 0) std::cout << "(Your armor reduces damage by " << mitigation << ")\n";
			player->takeDamage(finalDmg);
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

		// Player turn menu
		while (true) {
			std::cout << "\nYour turn: [A]ttack  [I]nventory  [E]nd\n";
			int key = _getch();
			key = std::tolower(key);

			if (key == 'i') {
				openInventoryDuringCombatByName(playerInv);  // lets you equip/unequip
				// After returning, let the player choose again (they might now attack)
				continue;
			}
			else if (key == 'e') {
				std::cout << "You end your turn.\n";
				std::cout << "=== Combat End ===\n";
				return;
			}
			else if (key == 'a') {
				break; // proceed to target selection + damage flow
			}
			else {
				std::cout << "Invalid key.\n";
			}
		}

		// Determine attack range from the equipped WEAPON
		int range = 1; // default melee (no weapon)
		if (Item* w = playerInv->getEquippedWeapon()) {
			range = std::max(1, w->GetNumber());   // adjust if your range stat is different
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

		// Selection UI: cycle valid candidates
		size_t sel = 0;
		bool selecting = true;
		while (selecting) {
			int idx = candidates[sel];
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
				break;
			}
		}

		int targetIndex = candidates[sel];
		Entity* target = (targetIndex >= 0 && targetIndex < 20) ? List[targetIndex] : nullptr;
		if (!target) { std::cout << "[Combat] Target went missing.\n"; std::cout << "=== Combat End ===\n"; return; }

		// Damage from WEAPON (or unarmed)
		if (Item* w = playerInv->getEquippedWeapon()) {
			std::cout << "You attacked the enemy with your " << w->GetItemWord('N') << "!\n";
			int dmg = std::max(1, w->GetItemValue('V'));   // adjust if you store damage elsewhere
			target->takeDamage(dmg);
		}
		else {
			std::cout << "Warning: No weapon equipped! You punch for 1 damage.\n";
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
				board.removeEnemy(List[i]);
				delete List[i];
				List[i] = nullptr;
				

			
			}
		}
	}

}
