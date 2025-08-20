#include "Combat.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Inventory.h"
#include <iostream>
#include <cmath>
#include <cstdlib>

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
	}
}

// External enemy list if used elsewhere
extern Entity* List[20];

int Combat::calculateDistance(const Position& a, const Position& b) {
    int dx = abs(a.getRow() - b.getRow());
    int dy = abs(a.getCol() - b.getCol());
    return max(dx, dy); // Chebyshev distance (tile-based)
}

void Combat::attack(Player& player, Enemy& enemy) {
    int distance = calculateDistance(player.getPosition(), enemy.getPosition());

    cout << "\n=== Combat Start ===\n";
    cout << "Distance between you and " << enemy.getTypeName() << ": " << distance << "\n";

    // Show player's inventory
    cout << "Your inventory: ";
    for (const std::string& item : player.getInventory()) {
        cout << item << " ";
    }
    cout << "\n";

    // Player's Turn
    bool attacked = false;
    const vector<string>& inventory = player.getInventory();

    for (const string& item : inventory) {
        if (item == "Sword" && distance <= 1) {
            cout << "You slash the enemy with your Sword!\n";
            enemy.takeDamage(20);
            attacked = true;
            break;
        }
        else if (item == "Bow" && distance <= 5) {
            cout << "You shoot an arrow at the enemy!\n";
            enemy.takeDamage(15);
            attacked = true;
            break;
        }
        else if (item == "Bomb" && distance <= 2) {
            cout << "You throw a bomb at the enemy!\n";
            enemy.takeDamage(30);
            attacked = true;
            break;
        }
    }

    if (!attacked) {
        cout << "You are either unarmed or out of range to attack.\n";
    }

    // Enemy defeated?
    if (!enemy.isAlive()) {
        cout << enemy.getTypeName() << " has been defeated!\n";
        player.earnGold(20);
        return;
    }

    // Enemy's Turn
    EnemyType type = enemy.getType();
    bool enemyCanMelee = (
        type == EnemyType::Monster ||
        type == EnemyType::Hellhound ||
        type == EnemyType::Zombie ||
        type == EnemyType::Goblin ||
        type == EnemyType::Bat ||
        type == EnemyType::Skeleton ||
        type == EnemyType::Boss
        );

    bool enemyCanRange = (
        type == EnemyType::Gargoyle ||
        type == EnemyType::Boss
        );

    bool enemyAttacked = false;

    if (enemyCanMelee && distance <= 1) {
        cout << enemy.getTypeName() << " strikes you in close combat!\n";
        player.takeDamage(enemy.getDamage());
        enemyAttacked = true;
    }
    else if (enemyCanRange && distance <= 5) {
        cout << enemy.getTypeName() << " attacks you from range!\n";
        player.takeDamage(enemy.getDamage() - 2); // range deals less
        enemyAttacked = true;
    }

    if (!enemyAttacked) {
        cout << enemy.getTypeName() << " is too far to attack.\n";
    }

    cout << "=== Combat End ===\n";
}

int Combat::WinCondition()
{
	for (int i = 0; i < 20; i++)
	{
		if (List[i] != nullptr)
		{
			return 0;
		}
	}
	return 1;
}

void Combat::TurnOrder()
{
    firstTurn = 1;
    while (WinCondition() == 0)
    {
        for (int i = 0; i < 20; i++)
        {
            board.drawBoard();
            List[i]->move();
            List[i]->attack();
            WinCondition();
        }
        firstTurn++;
        std::cout << "Turn Number: " << firstTurn;
    }
}