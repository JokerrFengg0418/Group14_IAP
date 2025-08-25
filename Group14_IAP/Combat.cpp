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

// Map enemy type -> drop name (must match your DB exactly)
static const char* lootNameFor(EnemyType t) {
	switch (t) {
	case EnemyType::Rat:       return "    Rat Tail    ";
	case EnemyType::Hellhound: return "      Fang      ";
	case EnemyType::Zombie:    return "  Rotten Flesh  ";
	case EnemyType::Goblin:    return "     Dagger     ", "   Mana Cores   ";
	case EnemyType::Bat:       return "    Bat Wing    ";
	case EnemyType::Skeleton:  return "      Bone      ";
	case EnemyType::Witch:     return "  Flying Broom  ";
	default:                   return ""; // no drop
	}
}

void Combat::FactoryCreateEntity(int CharacterType) {

	int RandomTurf = rand() % 25;
	switch (CharacterType) {
	case 0:

		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(24, RandomTurf, EnemyType::Rat, 3, 5);
				return;

			}
		}
		break;
	case 1:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Hellhound, 5, 6);
				return;

			}
		}
		break;
	case 2:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Zombie, 10, 6);
				return;

			}
		}
		break;
	case 3:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Goblin, 8, 7);
				return;

			}
		}
		break;
	case 4:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Bat, 4, 4);
				return;

			}
		}
		break;
	case 5:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Skeleton, 7, 5);
				return;

			}
		}
		break;
	case 6:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Witch, 25, 15);
				return;

			}
		}
		break;
	case 7:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(0, RandomTurf, EnemyType::Boss, 125, 30);
				return;

			}
		}
		break;
	case 8:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Player(0, 0, 100, 1);
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
		bool canMelee = (type == EnemyType::Rat || type == EnemyType::Hellhound ||
			type == EnemyType::Zombie || type == EnemyType::Goblin ||
			type == EnemyType::Bat || type == EnemyType::Skeleton ||
			type == EnemyType::Boss);

		bool canRange = (type == EnemyType::Witch || type == EnemyType::Boss);

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

	if (entity1->getEntityType() == 'T') {

		// Turret turn order
		// Turret Dynamic Cast
		Turret* turret = dynamic_cast<Turret*>(entity1);

		// Enemy Dynamic Cast
		

		if (!turret) return;

		// set attack range for turret
		const int AttackRange = 10;
		int turretRow = turret->getRow();
		int turretCol = turret->getCol();


		// loops through list to find enemies and attack if in range
		for (int i = 0; i < 20; ++i) {
			Entity* enemy = List[i];
			Enemy* enemy1 = dynamic_cast<Enemy*>(enemy);
			if (enemy1 != nullptr && enemy1->getEntityType() == 'E') {
				int enemyRow = enemy->getRow();
				int enemyCol = enemy->getCol();

				// calculate distance
				int distance = std::max(std::abs(turretRow - enemyRow), std::abs(turretCol - enemyCol));

				if (distance <= AttackRange) {
					enemy1->takeDamage(turret->getDamage());
					std::cout << "Turret at (" << turretRow << ", " << turretCol
						<< ") attacked enemy at (" << enemyRow << ", " << enemyCol
						<< ") for " << turret->getDamage() << " damage.\n";
					return;
				}
			}
		}
	}

	std::cout << "=== Combat End ===\n";
}

int Combat::WinCondition()
{
	bool hasPlayer = false;
	bool hasEnemy = false;

	for (int i = 0; i < 20; ++i) {
		if (!List[i]) continue;
		char t = List[i]->getEntityType();
		if (t == 'P') hasPlayer = true;
		else if (t == 'E') hasEnemy = true;

		if (hasPlayer && hasEnemy) {
			return 0; // keep fighting
		}
	}

	// If we get here, either no player or no enemies
	return 1; // combat over
}

void Combat::placeTurret(Inventory* playerInventory, Entity* List[])
{
	turretSelect = true;
	Item* turretItem = playerInventory->getInventory("    Turret    ");
	if (!turretItem) {
		std::cout << "You don't have a turret to place!" << std::endl;
		return;
	}

	// current position
	int row = 0;
	int col = 0;

	// target position starts as current
	int newRow = row;
	int newCol = col;

	const int ROWS = 25;
	const int COLS = 25;

	while (turretSelect == true) {
		const char input = _getch();
		switch (input) {
		case 'w': case 'W':
			newRow = newRow - 1;
			std::cout << "Move Up\n";
			break;
		case 's': case 'S':
			newRow = newRow + 1;
			std::cout << "Move Down\n";
			break;
		case 'a': case 'A':
			newCol = newCol - 1;
			std::cout << "Move Left\n";
			break;
		case 'd': case 'D':
			newCol = newCol + 1;
			std::cout << "Move Right\n";
			break;
		case'\r':
			turretSelect = false;
			std::cout << "Placing turret at (" << newRow << ", " << newCol << ")\n";
			break;
		default:
			std::cout << "invalid input\n";
			return; // don't move on invalid input
		}
		// bounds check (both axes)
		if (newRow < 0) { newRow + 1; }
		if (newRow >= ROWS) { newRow - 1; }
		if (newCol < 0) { newCol + 1; }
		if (newCol >= COLS) { newCol - 1; }

		if (row < 0 || row >= 25 || col < 0 || col >= 25) {
			std::cout << "Invalid position for turret!" << std::endl;
			return;
		}

		Turret* newTurret = new Turret(newRow, newCol, turretItem->GetItemValue('V'));
		board.addTurret(newTurret);
		std::cout << "Turret placed at (" << newRow << ", " << newCol << ")!" << std::endl;


		for (int i = 0; i < 20; i++) {
			if (List[i] != nullptr && List[i]->getRow() == newRow && List[i]->getCol() == newCol) {
				std::cout << "Cannot place turret here, position is occupied!" << std::endl;
				return;
			}
		}

		for (int i = 0; i < 20; i++) {
			if (List[i] == nullptr) {
				List[i] = new Turret(newRow, newCol, turretItem->GetItemValue('V'));
				playerInventory->RemoveItemFromInventory("    Turret    ", 1);
				std::cout << "Turret placed at (" << newRow << ", " << newCol << ")!" << std::endl;

				return;
			}
		}

	}
}


void Combat::TurnOrder(Inventory * PlayerInventory) {
	firstTurn = 1;
	while (WinCondition() == 0)
	{
		std::cout << "Turn Number: " << firstTurn << "\n";
		board.drawBoard(List);

		for (int i = 0; i < 20; ++i)
		{
			if (!List[i]) continue;

			List[i]->move(List);
			attack(List[i], PlayerInventory);
			FactoryDestructor(PlayerInventory); // ok to prune *dead* things here
		}

		++firstTurn;
		Sleep(200);
		system("cls");
	}

	// Final cleanup AFTER the loop ends
	for (int i = 0; i < 20; ++i) {
		if (List[i]) {
			// If it's an enemy, also remove from board’s internal list.
			if (List[i]->getEntityType() == 'E') {
				board.removeEnemy(List[i]);
			}
			delete List[i];
			List[i] = nullptr;
		}
	}

	std::cout << "Combat Ended \n";
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
	case 'T':
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);
		FactoryCreateEntity(0);
		board.addEnemy(List[1]);
		FactoryCreateEntity(0);
		board.addEnemy(List[2]);
		FactoryCreateEntity(0);
		board.addEnemy(List[3]);
		FactoryCreateEntity(0);
		board.addEnemy(List[4]);
		FactoryCreateEntity(0);
		board.addEnemy(List[5]);
	}
}

void Combat::earnGold(int amount, Inventory* inv) {
	inv->setCurrency(inv->getCurrency() + amount);
}

int Combat::getGold(Inventory* PlayerInventory) const {

	return PlayerInventory->getCurrency();

}

void Combat::FactoryDestructor(Inventory* playerInv) {
	for (int i = 0; i < 20; ++i) {
		if (!List[i]) continue;
		if (List[i]->getHealth() > 0) continue;

		// If it's an enemy, award loot before removing
		if (List[i]->getEntityType() == 'E') {
			if (Enemy* e = dynamic_cast<Enemy*>(List[i])) {
				const char* drop = lootNameFor(e->getType());
				if (playerInv && drop && drop[0] != '\0') {
					playerInv->setInventory(drop, 1); // stackable sellable items
					std::cout << "You obtained: " << drop << "!\n";
				}
				board.removeEnemy(List[i]); // keep board in sync
			}
		}

		delete List[i];
		List[i] = nullptr;
	}
}

