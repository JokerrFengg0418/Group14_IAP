#include "Turret.h"
#include "Combat.h"
#include <iostream>
#include <cmath>
#include <algorithm>

Turret::Turret() : Entity(0, 0)
{
	setHealth(50);
	setDamage(0);
	setEntityType('T');
	// Turret Creation
}

Turret::Turret(int r, int c, int dmg) : Entity(r, c)
{
	setRow(r);
	setCol(c);
	setHealth(50);
	setDamage(dmg);
	setEntityType('T');
}

Turret::~Turret()
{
	// Destroy Turret
}
