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

	void TurnOrder();

	
};

