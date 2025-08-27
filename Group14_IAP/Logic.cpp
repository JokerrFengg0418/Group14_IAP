#include "Logic.h"
#include "Combat.h"
#include "Option.h"
#include "Story.h"
#include "Dungeon.h"


Logic::Logic() {

	GlobalOrderState = 1;
	GameEndState = false;

}




void Logic::GameEndStateIncrease() {

	GameEndState = true;

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
	Story story(GameOption.getPlayerInventory());
	Dungeon dungeon;

	while (GameEndState == false) {
		
		
		//story.ShowWave(0, 0);
		//CombatHandler.startCombat('A');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		//int status = story.ShowWave(1, 0);
		//if (status == 1) 
		//{ 
		//	GameOption.shopOption(GameOption.getPlayerInventory());
		//	story.ShowWave(1, 1);
		//	status = 0;
		//}
		//CombatHandler.startCombat('B');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		//int status1 = story.ShowWave(2, 0);
		//if (status1 == 1)
		//{
		//	GameOption.shopOption(GameOption.getPlayerInventory());
		//	story.ShowWave(2, 71);
		//	status1 = 0;
		//}
		//CombatHandler.startCombat('C');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		//int status2 = story.ShowWave(3, 0);
		//if (status2 == 1)
		//{
		//	GameOption.shopOption(GameOption.getPlayerInventory());
		//	story.ShowWave(3, 82);
		//	status2 = 0;
		//}
		//else if (status2 == 2)
		//{
		//	dungeon.dungeonOption();
		//	GameOption.waitForEnter();
		//	status2 = 0;
		//}
		//CombatHandler.startCombat('D');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		//int status3 = story.ShowWave(4, 0);
		//if (status3 == 1)
		//{
		//	GameOption.shopOption(GameOption.getPlayerInventory());
		//	story.ShowWave(4, 10);
		//	status3 = 0;
		//}
		//CombatHandler.startCombat('E');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		//story.ShowWave(5, 0);
		//CombatHandler.startCombat('F');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		//story.ShowWave(6, 0);
		//CombatHandler.startCombat('G');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		//story.ShowWave(7, 0);
		//story.ShowWave(8, 0);
		GameOption.runMainMenu();
		CombatHandler.startCombat('H');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);

	}

}

