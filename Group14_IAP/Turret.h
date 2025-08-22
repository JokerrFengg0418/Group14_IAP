#pragma once
#include <iostream>
#include "Entity.h"
#include "Combat.h"

class Turret : public Entity
{
public:
	// Constructor
	Turret();

	// Overloaded Con
	Turret(int r, int c, int dmg);
	
	// Destructor
	~Turret();

	void Update(Entity* enemies[], int enemyCount);

	
};

