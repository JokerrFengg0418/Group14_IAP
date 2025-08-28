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

	// Turrets don't move, so this function is empty
	void move(Entity* List[]) override {}

	bool turretSelect = false;

};