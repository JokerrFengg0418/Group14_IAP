#pragma once
#include "Entity.h"
#include "Enemy.h"
#include "Player.h"

class Combat
{
private:

	Entity* List[20] ;



public:

	Entity* FactoryCreateEntity(int CharacterType);

	Entity* CreatePlayer();

	Entity* CreateEnemy();

	int WinCondition();
	void TurnOrder();

	// Calculates distance between two positions using Chebyshev distance
	static int calculateDistance(const Position& a, const Position& b);

	// Handles a full turn: player attacks first, enemy retaliates if alive
	static void attack(Player& player, Enemy& enemy);

};

