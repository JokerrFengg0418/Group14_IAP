#include <windows.h>
#include "Logic.h"
#include "Combat.h"
#include "Option.h"
#include "Board.h"
#include "Story.h"
#include "Dungeon.h"
#include "Entity.h"
#include <cstdlib>
#include <chrono>
#include <thread>


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
		
		
		//story.ShowWave(0, 0);
		//GameOption.waitForEnter();


		//CombatHandler.startCombat('A');
		//
		//cutsceneCombatTutorial(); // cutscene combat
		//GameOption.waitForEnter(); // Enter

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
		//else if (status1 == 2)
		//{
		//	cutsceneDungeonTutorial();
		//	GameOption.waitForEnter();
		//	dungeon.dungeonOption();
		//	story.ShowWave(2, 7);
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
		//CombatHandler.startCombat('D');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		GameOption.runMainMenu();
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
		story.ShowWave(6, 0);
		//CombatHandler.startCombat('G');
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		//GlobalOrderSet(GlobalOrderGet() + 1);
		story.ShowWave(7, 0);
		story.ShowWave(8, 0);
		GameOption.runMainMenu();
		CombatHandler.startCombat('H');
		CombatHandler.TurnOrder(GameOption.getPlayerInventory());
		GlobalOrderSet(GlobalOrderGet() + 1);

	}

}

void Logic::cutsceneCombatTutorial()
{
	//Board board;
	//Entity* Entitylist[20];
	//board.drawBoard(Entitylist, 20);

	system("cls");
	std::cout << "Welcome to the Combat Tutorial!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "Fighting is really easy! Just be within 2 tiles of the enemy\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "In order for you to initiate the attack!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "Just remember that, different weapons have different ranges!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "You can also, cycle the attack if there are multiple enemies\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "nearby you and you can choose who to attack! you can do this with A and D keys";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	system("cls");

}
void Logic::cutsceneInventoryTutorial()
{
	system("cls");
	std::cout << "Welcome to the Inventory Tutorial!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "You can choose to equip, unequip or use an item!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "simply type equip [name of equipment/item]\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "or, unequip[weapon/armor name]\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "to use the health potion, use [Health Potion] to heal yourself!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "see, easy right! Now go on and fight youngin'!";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	system("cls");
}
void Logic::cutsceneDungeonTutorial()
{
	system("cls");
	std::cout << "The X are random events \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "They could give you a random item, or bring you into a dungeon room \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "Once all the monster dies in a room, you will be able to leave \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "Step on a X tile to automatically collect or go into a room \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "The dungeon provides extra resources \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	std::cout << "A red ruby is hidden in one of the X \n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));
	system("cls");
}

