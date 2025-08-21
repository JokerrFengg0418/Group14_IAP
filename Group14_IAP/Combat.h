#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include "Board.h"
#include <vector>
#include <string>

class Combat
{
private:
	int firstTurn;
	bool spawn;
	bool playerTurn;
	Board board;

protected:
	Entity* List[20];
public:

	void FactoryCreateEntity(int CharacterType);

	int WinCondition();
	void TurnOrder(Inventory* PlayerInventory);

	void startCombat(char CombatScenario);

	// Calculates distance between two positions using Chebyshev distance
	static int calculateDistance(const Position& a, const Position& b);

	// Attack Function
	void attack(Entity* Entity, Inventory* PlayerInventory);

	void move();

	void FactoryDestructor();

	// Reward
	void earnGold(int amount, Inventory* Inventory);
	int getGold(Inventory* PlayerInventory) const;

};

