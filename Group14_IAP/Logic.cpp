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
		CombatHandler.startCombat('A');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(1, 0);
		GameOption.shopOption(GameOption.getPlayerInventory());
		CombatHandler.startCombat('B');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(2, 0);
		CombatHandler.startCombat('C');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(3, 0);
		CombatHandler.startCombat('D');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(4, 0);
		CombatHandler.startCombat('E');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(5, 0);
		CombatHandler.startCombat('F');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(6, 0);
		CombatHandler.startCombat('G');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(7, 0);
		CombatHandler.startCombat('H');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(8, 0);*/
		GameOption.runMainMenu();
		CombatHandler.startCombat('A');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);

	}

}

