#pragma once
#include "Option.h"
#include "Combat.h"
class Logic
{
private:
	

	//Determines what progression the Player is at//
	int GlobalOrderState;

	//End Condition of the Game//
	bool GameEndState;

	Combat* CombatLogic;
	Option* Optionlogic;
	
public:

	//Constructor//
	Logic();

	// Game End State increases//
	void GameEndStateIncrease();

	//Game End State Get//
	bool GameEndStateGet();

	//Global Order Get//
	int GlobalOrderGet() const;

	//Global Order Set//
	void GlobalOrderSet(int CurrentGlobalOrder);

	//Turn Order//
	void TurnOrder();

	void SetDungeonSeed(Option* InventorySave, Combat* Combathandler);

	Option* GetDungeonOption();

	Combat* GetDungeonCombat();

};

