#include "Combat.h"
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
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

// External list of enemies
extern Entity* List[20];

int Combat::calculateDistance(const Position& a, const Position& b) {
    int dx = abs(a.getRow() - b.getRow());
    int dy = abs(a.getCol() - b.getCol());
    return max(dx, dy); // Chebyshev distance
}

void Combat::attack(Player& player, Enemy& enemy) {
    int distance = calculateDistance(player.getPosition(), enemy.getPosition());

    cout << "\n=== Combat Start ===\n";
    cout << "Distance between you and " << enemy.getTypeName() << ": " << distance << "\n";

    bool attacked = false;

    // Player Turn
    for (const string& item : player.getInventory()) {
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
    }

    if (!attacked) {
        cout << "You are either unarmed or out of range to attack.\n";
    }

    if (!enemy.isAlive()) {
        cout << enemy.getTypeName() << " has been defeated!\n";
        player.earnGold(20);
        return;
    }

    // Enemy Turn
    EnemyType type = enemy.getType();
    bool enemyCanMelee = (type == EnemyType::Monster || type == EnemyType::Hellhound ||
        type == EnemyType::Zombie || type == EnemyType::Goblin ||
        type == EnemyType::Bat || type == EnemyType::Skeleton ||
        type == EnemyType::Boss);

    bool enemyCanRange = (type == EnemyType::Gargoyle || type == EnemyType::Boss);

    bool enemyAttacked = false;

    if (enemyCanMelee && distance <= 1) {
        cout << enemy.getTypeName() << " strikes you in close combat!\n";
        player.setHealth(player.getHealth() - enemy.getDamage());
        enemyAttacked = true;
    }
    else if (enemyCanRange && distance <= 5) {
        cout << enemy.getTypeName() << " attacks you from range!\n";
        player.setHealth(player.getHealth() - (enemy.getDamage() - 2));
        enemyAttacked = true;
    }

    if (!enemyAttacked) {
        cout << enemy.getTypeName() << " is too far to attack.\n";
    }

    cout << "=== Combat End ===\n";
}

Entity* Combat::FactoryCreateEntity(int CharacterType) {
    int spawnRow = 39;
    int spawnCol = rand() % 40;

    EnemyType type;
    switch (CharacterType) {
    case 0: type = EnemyType::Monster;   break;
    case 1: type = EnemyType::Hellhound; break;
    case 2: type = EnemyType::Zombie;    break;
    case 3: type = EnemyType::Goblin;    break;
    case 4: type = EnemyType::Bat;       break;
    case 5: type = EnemyType::Skeleton;  break;
    case 6: type = EnemyType::Gargoyle;  break;
    case 7: type = EnemyType::Boss;      break;
    default: return nullptr;
    }

    for (int i = 0; i < 20; ++i) {
        if (List[i] == nullptr) {
            Entity* newEnemy = new Enemy(spawnRow, spawnCol, type, 99, 10 + rand() % 5);
            List[i] = newEnemy;
            return newEnemy;
        }
    }

    return nullptr;
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