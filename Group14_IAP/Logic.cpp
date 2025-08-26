#include "Logic.h"
#include "Combat.h"
#include "Option.h"
#include "Story.h"

Logic::Logic() {

	GlobalOrderState = 1;
	GameEndState = false;

}




void Logic::GameEndStateIncrease() {

	GameEndState += 1;

}




bool Logic::GameEndStateGet() {

	return GameEndState;

}




int Logic::GlobalOrderGet() const{

	return GlobalOrderState;

}




void Logic::GlobalOrderSet(int CurrentGlobalOrder) {

	GlobalOrderState = CurrentGlobalOrder;

}




void Logic::TurnOrder() {

	Option GameOption;
	Combat CombatHandler;
	Story story;

	while (GameEndState == false) {
		
		/*story.ShowWave(0, 0);
		story.ShowWave(1, 0);
		story.ShowWave(2, 0);
		story.ShowWave(3, 0);*/
		GameOption.runMainMenu();
		CombatHandler.startCombat('A');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);

	}

}

void Logic::SetDungeonSeed(Option* InventorySave, Combat* Combathandler)
{
	CombatLogic = Combathandler;
	Optionlogic = InventorySave;
}

Option* Logic::GetDungeonOption()
{
	return Optionlogic;
}

Combat* Logic::GetDungeonCombat()
{
	return CombatLogic;
}

