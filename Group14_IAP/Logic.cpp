#include "Logic.h"
#include "Combat.h"
#include "Option.h"
#include "Board.h"
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
	Dungeon dungeon(GameOption.getPlayerDungeon(), GameOption.getPlayerInventory());


	while (GameEndState == false) {
		
		
		story.ShowWave(0, 0);
		GameOption.waitForEnter();
		cutsceneCombatTutorial();
		CombatHandler.startCombat('A');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		int status = story.ShowWave(1, 0);
		if (status == 1) 
		{ 
			GameOption.shopOption(GameOption.getPlayerInventory());
			story.ShowWave(1, 1);
			status = 0;
		}
		CombatHandler.startCombat('B');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		int status1 = story.ShowWave(2, 0);
		if (status1 == 1)
		{
			GameOption.shopOption(GameOption.getPlayerInventory());
			story.ShowWave(2, 71);
			status1 = 0;
		}
		else if (status1 == 2)
		{
			cutsceneDungeonTutorial();
			dungeon.dungeonOption();
			story.ShowWave(2, 7);
			status1 = 0;
		}
		CombatHandler.startCombat('C');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		int status2 = story.ShowWave(3, 0);
		if (status2 == 1)
		{
			GameOption.shopOption(GameOption.getPlayerInventory());
			story.ShowWave(3, 82);
			status2 = 0;
		}
		CombatHandler.startCombat('D');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);
		int status3 = story.ShowWave(4, 0);
		if (status3 == 1)
		{
			GameOption.shopOption(GameOption.getPlayerInventory());
			story.ShowWave(4, 10);
			status3 = 0;
		}
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
		story.ShowWave(8, 0);
		GameOption.runMainMenu();
		CombatHandler.startCombat('H');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);

	}

}

void Logic::cutsceneDungeonTutorial()
{
	std::cout << "The X are random events \n";
	std::cout << "They could give you a random item, or bring you into a dungeon room \n";
	std::cout << "Once all the monster dies in a room, you will be able to leave \n";
	std::cout << "Step on a X tile to automatically collect or go into a room \n";
	std::cout << "The dungeon provides extra resources \n";
	std::cout << "A red ruby is hidden in one of the X \n";
}

void Logic::cutsceneCombatTutorial(Combat& combat)
{
	std::cout << "Welcome to the Combat Tutorial!";
	std::cout << "Fighting is really easy! Just be within 2 tiles of the enemy";
	std::cout << "In order for you to initiate the attack!";
	std::cout << "You can also, cycle the attack if there are multiple enemies\n nearby you and you want to choose who to attack! you can do this with A and D keys";
}

