#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"
#include <vector>
#include <string>

class Combat
{
private:

	Entity* List[20] ;
	int firstTurn;
	bool spawn;
	bool playerTurn;


public:

	Entity* FactoryCreateEntity(int CharacterType);

	Entity* CreatePlayer();

	Entity* CreateEnemy();

	int WinCondition();
	void TurnOrder();

	void startCombat(Player& player, Enemy& enemy);

	// Calculates distance between two positions using Chebyshev distance
	static int calculateDistance(const Position& a, const Position& b);

	// Handles a full turn: player attacks first, enemy retaliates if alive
	static void attack(Player& player, Enemy& enemy);

	void move();

	// Inventory Functions
	void addItem(const std::string& item);
	const std::vector<std::string>& getInventory() const;

	// Reward
	void earnGold(int amount);
	int getGold() const;

};

