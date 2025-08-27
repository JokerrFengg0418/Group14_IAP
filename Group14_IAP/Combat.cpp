#define NOMINMAX
#include "Combat.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Inventory.h"
#include "Option.h"
#include "Logic.h"
#include "Turret.h"
#include <iostream>
#include <cmath>
#include <algorithm>  
#include <cstdlib>
#include <vector>
#include <limits>
#include <sstream>
#include <string>
#include <cctype>
#include <conio.h>
#include <Windows.h>
#include <chrono>
#include <thread>

using namespace std;

constexpr int BOARD_ROWS = 20;
constexpr int BOARD_COLS = 20;

static bool isOccupiedCell(int r, int c, Entity* list[]) {
	for (int i = 0; i < 20; ++i) {
		if (list[i] && list[i]->getRow() == r && list[i]->getCol() == c) return true;
	}
	return false;
}

// Avoids (avoidR, avoidC) explicitly AND any occupied cell in List[]
static bool findRandomFreeCellAvoiding(int& outR, int& outC, Entity* list[],
	int rows = BOARD_ROWS, int cols = BOARD_COLS,
	int maxTries = 200,
	int avoidR = 0, int avoidC = 0)
{
	// random attempts
	for (int t = 0; t < maxTries; ++t) {
		int r = rand() % rows;
		int c = rand() % cols;
		if ((r == avoidR && c == avoidC)) continue;     // never top-left
		if (!isOccupiedCell(r, c, list)) { outR = r; outC = c; return true; }
	}
	// fallback linear scan
	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			if ((r == avoidR && c == avoidC)) continue; // never top-left
			if (!isOccupiedCell(r, c, list)) { outR = r; outC = c; return true; }
		}
	}
	return false; // grid full
}



// Map enemy type -> drop name (must match your DB exactly)
static const char* lootNameFor(EnemyType t) {
	switch (t) {
	case EnemyType::Rat:       return "    Rat Tail    ";
	case EnemyType::Hellhound: return "      Fang      ";
	case EnemyType::Zombie:    return "  Rotten Flesh  ";
	case EnemyType::Goblin:    return "   Mana Cores   ";
	case EnemyType::Bat:       return "    Bat Wing    ";
	case EnemyType::Skeleton:  return "      Bone      ";
	case EnemyType::Witch:     return "  Flying Broom  ";
	default:                   return ""; // no drop
	}
}

void Combat::FactoryCreateEntity(int CharacterType) {

	int RandomTurf = rand() % 20;
	switch (CharacterType) {
	case 0:

		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(19, RandomTurf, EnemyType::Rat, 3, 5);
				return;

			}
		}
		break;
	case 1:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(19, RandomTurf, EnemyType::Hellhound, 5, 6);
				return;

			}
		}
		break;
	case 2:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(19, RandomTurf, EnemyType::Zombie, 10, 6);
				return;

			}
		}
		break;
	case 3:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(19, RandomTurf, EnemyType::Goblin, 8, 7);
				return;

			}
		}
		break;
	case 4:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(19, RandomTurf, EnemyType::Bat, 4, 4);
				return;

			}
		}
		break;
	case 5:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(19, RandomTurf, EnemyType::Skeleton, 7, 5);
				return;

			}
		}
		break;
	case 6:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(19, RandomTurf, EnemyType::Witch, 25, 15);
				return;

			}
		}
		break;
	case 7:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Enemy(19, RandomTurf, EnemyType::Boss, 125, 30);
				return;

			}
		}
		break;
	case 8:
		for (int i = 0; i < 20; i++) {

			if (List[i] == nullptr) {

				List[i] = new Player(0, 0, 100, 5);
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
void Combat::openInventoryDuringCombatByName(Inventory* inv) {
	if (!inv) return;

	Logic logic;

	if (!inventoryTutorialPlayed) {
		logic.cutsceneInventoryTutorial();  // <-- Your function to show tutorial
		inventoryTutorialPlayed = true; // Mark as shown
	}

	while (true) {
		system("cls"); // hide board while inventory is open

		std::cout << "\n=== INVENTORY (Combat) ===\n";
		inv->DrawInventory();
		std::cout << "Commands:\n";
		std::cout << "  equip <item name>\n";
		std::cout << "  unequip <weapon|armor>\n";
		std::cout << "  use <item name>\n";
		std::cout << "  e = exit\n> ";

		std::string line;
		if (!std::getline(std::cin >> std::ws, line)) return;
		if (line.empty()) continue;

		// quick exit
		std::string lowerAll = toLowerCopy(line);
		if (lowerAll == "e") return;

		// tokenize: first word = command, rest = argument
		std::istringstream iss(line);
		std::string cmd; iss >> cmd;
		std::string arg; std::getline(iss, arg);
		if (!arg.empty()) {
			size_t p = arg.find_first_not_of(' ');
			if (p != std::string::npos) arg.erase(0, p);
			else arg.clear();
		}

		const std::string cmdLower = toLowerCopy(cmd);
		const std::string argLower = toLowerCopy(arg);

		if (cmdLower == "equip") {
			if (arg.empty()) {
				std::cout << "Usage: equip <item name>\nPress any key to continue...";
				_getch();
				continue;
			}

			// 1) Find the actual item in the bag (partial match OK)
			Item* found = inv->FindItemByName(arg);
			if (!found) {
				std::cout << "Item not found: " << arg << "\nPress any key to continue...";
				_getch();
				continue;
			}
			const std::string nm = found->GetItemWord('N');

			// 2) Route by DB membership so we don't call the wrong equip function
			if (inv->DrawDatabase('M', nm)) {
				std::cout << "Monster drop items/Quest Items cannot be equipped.\nPress any key to continue...";
				_getch();
				continue;
			}

			bool ok = false;
			if (inv->DrawDatabase('W', nm)) {
				ok = inv->equipWeaponByName(nm);
				if (ok) { std::cout << "Equipped weapon: " << nm << "\n"; }
			}
			else if (inv->DrawDatabase('A', nm)) {
				ok = inv->equipArmorByName(nm);
				if (ok) { std::cout << "Equipped armor: " << nm << "\n"; }
			}
			else {
				std::cout << "That item isn't equippable.\n";
			}

			std::cout << "Press any key to continue...";
			_getch();
			continue;
		}

		if (cmdLower == "unequip") {
			if (argLower == "weapon" || argLower == "w") {
				if (inv->getEquippedWeapon()) {
					std::cout << "Unequipped weapon: " << inv->getEquippedWeapon()->GetItemWord('N') << "\n";
				}
				inv->unequipWeapon();
				std::cout << "Press any key to continue...";
				_getch();
				continue;
			}
			if (argLower == "armor" || argLower == "a") {
				if (inv->getEquippedArmor()) {
					std::cout << "Unequipped armor: " << inv->getEquippedArmor()->GetItemWord('N') << "\n";
				}
				inv->unequipArmor();
				std::cout << "Press any key to continue...";
				_getch();
				continue;
			}
			std::cout << "Usage: unequip <weapon|armor>\nPress any key to continue...";
			_getch();
			continue;
		}

		if (cmdLower == "use") {
			if (arg.empty()) {
				std::cout << "Usage: use <item name>\nPress any key to continue...";
				_getch();
				continue;
			}

			Item* found = inv->FindItemByName(arg);
			if (!found) {
				std::cout << "Item not found: " << arg << "\nPress any key to continue...";
				_getch();
				continue;
			}
			if (inv->DrawDatabase('M', arg) || inv->DrawDatabase('W', arg) || inv->DrawDatabase('A', arg)) {

				std::cout << "Monster item, weapons and armour cannot be used.\nPress any key to continue...";
				_getch();
				continue;

			}
			if (inv->DrawDatabase('I', arg)) {
				std::cout << "Used item: " << found->GetItemWord('N') << "\n";
				int cur = found->GetItemValue('V');

				if (cur > 1) {
					found->SetItemValue('V', cur - 1);
				}

				else {
					inv->RemoveItemFromInventory(found->GetItemWord('N'), 1);
				}

				for (int i = 0; i < 20; i++)
				{
					if (!List[i]) continue;
					if (List[i]->getEntityType() == 'P') {
						List[i]->setHealth(List[i]->getHealth() + found->GetNumber());
						std::cout << "You healed " << found->GetNumber() << "HP.\n";
						break;
					}
				}
				std::cout << "Press any key to continue...";
				_getch();
				continue;
			}
			

		}

		std::cout << "Unknown command. Use: equip <name>  or  unequip <weapon|armor>\nPress any key to continue...";
		_getch();
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

	// ------------- ENEMY TURN -------------
	if (entity1->getEntityType() == 'E') {
		// Find the player in List
		Entity* player = nullptr;
		for (int i = 0; i < 20; ++i) {
			if (List[i] == nullptr) continue;
			if (List[i]->getEntityType() == 'P') { player = List[i]; break; }
		}
		if (!player) { std::cout << "[Combat] No player found.\n"; std::cout << "=== Combat End ===\n" ; return; }

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
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		system("cls");
		board.drawBoard(List, 20);
		
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
				// Clear to show inventory only
				openInventoryDuringCombatByName(playerInv);

				// When inventory closes: restore the board view
				system("cls");
				board.drawBoard(List, 20);   // use your board’s draw function signature
				// then let the user choose again
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
		const int AttackRange = 5;
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
	Item* turretItem = playerInventory->getInventory("     Turret     ");

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

	const int ROWS = 20;
	const int COLS = 20;

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
		if (newRow < 0) { newRow - 1; }
		if (newRow >= ROWS) { newRow + 1; }
		if (newCol < 0) { newCol -  1; }
		if (newCol >= COLS) { newCol + 1; }

		if (row < 0 || row >= 20 || col < 0 || col >= 20) {
			std::cout << "Invalid position for turret!" << std::endl;
			return;
		}

		board.selectTurretHighlight(newRow, newCol);

		Turret* newTurret = new Turret(newRow, newCol, turretItem->GetItemValue('V'));
		board.addTurret(newTurret);
		std::cout << "Turret placed at (" << newRow << ", " << newCol << ")!" << std::endl;

		for (int i = 0; i < 20; i++) {
			if (List[i] != nullptr &&
				List[i]->getRow() == newRow &&
				List[i]->getCol() == newCol)
			{
				std::cout << "Cannot place turret here, position is occupied!" << std::endl;
				return;
			}
		}


		}

	for (int i = 0; i < 20; i++) {
		if (List[i] == nullptr) {
			List[i] = new Turret(newRow, newCol, turretItem->GetItemValue('V'));
			playerInventory->RemoveItemFromInventory("    Turret    ", 1);
			std::cout << "Turret placed at (" << newRow << ", " << newCol << ")!" << std::endl;
			break;
		}
	}

}


void Combat::TurnOrder(Inventory* PlayerInventory)
{
	firstTurn = 1;
	board.drawBoard(List, 20);
	placeTurret(PlayerInventory, List);
	system("cls");

	while (WinCondition() == 0)
	{
		std::cout << "Turn Number: " << firstTurn << "\n";
		board.drawBoard(List, 20);


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
	case 'A': // Wave 1 ([R]ats)

		// Player Spawn
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);


		// Enemy Spawn
		FactoryCreateEntity(0);
		board.addEnemy(List[1]);
		FactoryCreateEntity(0);
		board.addEnemy(List[2]);
		FactoryCreateEntity(0);
		board.addEnemy(List[3]);
		break;

	case 'B': // Wave 2 ([H]ellhounds)

		// Player Spawn
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);


		// Enemy Spawn
		FactoryCreateEntity(1);
		board.addEnemy(List[1]);
		FactoryCreateEntity(1);
		board.addEnemy(List[2]);
		FactoryCreateEntity(1);
		board.addEnemy(List[3]);
		FactoryCreateEntity(1);
		board.addEnemy(List[4]);
		break;
	case 'C': // Wave 3 ([Z]ombies)

		// Player Spawn
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);

		// Enemy Spawn
		FactoryCreateEntity(2);
		board.addEnemy(List[1]);
		FactoryCreateEntity(2);
		board.addEnemy(List[2]);
		FactoryCreateEntity(2);
		board.addEnemy(List[3]);
		FactoryCreateEntity(2);
		board.addEnemy(List[4]);
		FactoryCreateEntity(2);
		board.addEnemy(List[5]);
		FactoryCreateEntity(2);
		board.addEnemy(List[6]);
		FactoryCreateEntity(2);
		board.addEnemy(List[7]);
		FactoryCreateEntity(2);
		board.addEnemy(List[8]);
		break;

	case 'D': // Wave 4 ([G]oblins)

		// Player Spawn
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);

		// Enemy Spawn
		FactoryCreateEntity(3);
		board.addEnemy(List[1]);
		FactoryCreateEntity(3);
		board.addEnemy(List[2]);
		FactoryCreateEntity(3);
		board.addEnemy(List[3]);
		FactoryCreateEntity(3);
		board.addEnemy(List[4]);
		FactoryCreateEntity(3);
		board.addEnemy(List[5]);
		FactoryCreateEntity(3);
		board.addEnemy(List[6]);
		FactoryCreateEntity(3);
		board.addEnemy(List[7]);
		FactoryCreateEntity(3);
		board.addEnemy(List[8]);
		FactoryCreateEntity(3);
		board.addEnemy(List[9]);
		break;

	case 'E': // Wave 5 ([B]ats)

		// Player Spawn
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);

		// Enemy Spawn
		FactoryCreateEntity(4);
		board.addEnemy(List[1]);
		FactoryCreateEntity(4);
		board.addEnemy(List[2]);
		FactoryCreateEntity(4);
		board.addEnemy(List[3]);
		FactoryCreateEntity(4);
		board.addEnemy(List[4]);
		FactoryCreateEntity(4);
		board.addEnemy(List[5]);
		FactoryCreateEntity(4);
		board.addEnemy(List[6]);
		FactoryCreateEntity(4);
		board.addEnemy(List[7]);
		FactoryCreateEntity(4);
		board.addEnemy(List[8]);
		FactoryCreateEntity(4);
		board.addEnemy(List[9]);
		FactoryCreateEntity(4);
		board.addEnemy(List[10]);
		FactoryCreateEntity(4);
		board.addEnemy(List[11]);
		FactoryCreateEntity(4);
		board.addEnemy(List[12]);
		FactoryCreateEntity(4);
		board.addEnemy(List[13]);
		FactoryCreateEntity(4);
		board.addEnemy(List[14]);
		FactoryCreateEntity(4);
		board.addEnemy(List[15]);
		FactoryCreateEntity(4);
		board.addEnemy(List[16]);
		break;


	case 'F': // Wave 6 ([S]keletons)

		// Player Spawn
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);

		// Enemy Spawn
		FactoryCreateEntity(5);
		board.addEnemy(List[1]);
		FactoryCreateEntity(5);
		board.addEnemy(List[2]);
		FactoryCreateEntity(5);
		board.addEnemy(List[3]);
		FactoryCreateEntity(5);
		board.addEnemy(List[4]);
		FactoryCreateEntity(5);
		board.addEnemy(List[5]);
		FactoryCreateEntity(5);
		board.addEnemy(List[6]);
		FactoryCreateEntity(5);
		board.addEnemy(List[7]);
		FactoryCreateEntity(5);
		board.addEnemy(List[8]);
		FactoryCreateEntity(5);
		board.addEnemy(List[9]);
		FactoryCreateEntity(5);
		board.addEnemy(List[10]);
		FactoryCreateEntity(5);
		board.addEnemy(List[11]);
		FactoryCreateEntity(5);
		board.addEnemy(List[12]);
		FactoryCreateEntity(5);
		board.addEnemy(List[13]);
		break;


	case 'G': // Wave 7 ([W]itches)

		// Player Spawn
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);

		// Enemy Spawn
		FactoryCreateEntity(6);
		board.addEnemy(List[1]);
		FactoryCreateEntity(6);
		board.addEnemy(List[2]);
		FactoryCreateEntity(6);
		board.addEnemy(List[3]);
		FactoryCreateEntity(6);
		board.addEnemy(List[4]);
		FactoryCreateEntity(6);
		board.addEnemy(List[5]);
		break;

	case 'H': // Wave 8 ([A]bomination)

		// Player Spawn
		FactoryCreateEntity(8);
		board.addPlayer(List[0]);

		// Enemy Spawn
		FactoryCreateEntity(7);
		board.addEnemy(List[1]);
		break;
	}
}

void Combat::earnGold(int amount, Inventory* inv) {
	inv->setCurrency(inv->getCurrency() + amount);
}

int Combat::getGold(Inventory* PlayerInventory) const {

	return PlayerInventory->getCurrency();

}

void Combat::startDungeonOneTypeRandom(int minEnemies, int maxEnemies, bool /*allowBoss*/)
{
	// --- sanitize inputs ---
	if (minEnemies < 1)  minEnemies = 1;
	if (maxEnemies > 19) maxEnemies = 19;   // keep a slot for player
	if (minEnemies > maxEnemies) std::swap(minEnemies, maxEnemies);

	// --- clear this combat instance's entities ---
	for (int i = 0; i < 20; ++i) {
		if (List[i]) { delete List[i]; List[i] = nullptr; }
	}

	// --- add the player using your factory (keeps normal behavior) ---
	FactoryCreateEntity(8);
	board.addPlayer(List[0]); // spawns your Player as usual

	// --- choose ONE non-boss enemy type (0..6) ---
	int typeIndex = rand() % 7; // Rat..Witch (no Boss)

	// --- how many enemies? ---
	int toSpawn = minEnemies + (rand() % (maxEnemies - minEnemies + 1));
	if (toSpawn > 19) toSpawn = 19;

	// helper: HP/DMG per type
	auto hpFor = [](int t) {
		switch (t) {
		case 0: return 3;   // Rat
		case 1: return 5;   // Hellhound
		case 2: return 10;  // Zombie
		case 3: return 8;   // Goblin
		case 4: return 4;   // Bat
		case 5: return 7;   // Skeleton
		case 6: return 25;  // Witch
		default: return 5;
		}
	};
	auto dmgFor = [](int t) {
		switch (t) {
		case 0: return 5;
		case 1: return 6;
		case 2: return 6;
		case 3: return 7;
		case 4: return 4;
		case 5: return 5;
		case 6: return 15;
		default: return 5;
		}
	};

	auto etFor = [](int t) -> EnemyType {
		switch (t) {
		case 0: return EnemyType::Rat;
		case 1: return EnemyType::Hellhound;
		case 2: return EnemyType::Zombie;
		case 3: return EnemyType::Goblin;
		case 4: return EnemyType::Bat;
		case 5: return EnemyType::Skeleton;
		case 6: return EnemyType::Witch;
		default: return EnemyType::Rat;
		}
	};

	// --- spawn that many of the chosen type at RANDOM FREE CELLS ---
	for (int k = 0; k < toSpawn; ++k) {
		int r, c;
		// avoid (0,0) where the player spawns, and avoid any occupied cell
		if (!findRandomFreeCellAvoiding(r, c, this->List, BOARD_ROWS, BOARD_COLS, 200, /*avoidR=*/0, /*avoidC=*/0)) {
			std::cout << "[Dungeon] No free cell to spawn more enemies.\n";
			break;
		}

		// find a free slot in List
		int slot = -1;
		for (int i = 0; i < 20; ++i) { if (!List[i]) { slot = i; break; } }
		if (slot == -1) { std::cout << "[Dungeon] Entity list full.\n"; break; }

		List[slot] = new Enemy(r, c, etFor(typeIndex), hpFor(typeIndex), dmgFor(typeIndex));
		board.addEnemy(List[slot]);
	}
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
					playerInv->setInventory(drop, 1);
					std::cout << "You obtained: " << drop << "!\n";
				}
				board.removeEnemy(List[i]);
			}
		}

		delete List[i];
		List[i] = nullptr;
	}
}




