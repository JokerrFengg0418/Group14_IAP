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
	int wave = 0;
	int choiceId = 0;

	while (GameEndState == false) {
		
		
		// --- Show the story for this wave ---
		int status = story.ShowWave(wave, choiceId);
		GameOption.waitForEnter();

		// --- Handle special return values ---
		switch (status)
		{
		case 1:
			GameOption.shopOption(GameOption.getPlayerInventory());
			choiceId = story.ShowWave(wave, 1);
			break;
		case 81:
			GameOption.shopOption(GameOption.getPlayerInventory());
			choiceId = story.ShowWave(wave, 82);
			break;
		case 9:
			GameOption.shopOption(GameOption.getPlayerInventory());
			choiceId = story.ShowWave(wave, 10);
			break;
		case 12:
			GameOption.shopOption(GameOption.getPlayerInventory());
			choiceId = story.ShowWave(wave, 12);
			break;
		case 2:
			cutsceneDungeonTutorial();
			GameOption.waitForEnter();
			dungeon.dungeonOption();
			choiceId = story.ShowWave(wave, 7);
			break;
		case 3:
			GameEndState = true;
			return;
			break;
		default:
			break;
		}
		

		// --- Run combat after each wave ---
		char combatTag = 'A' + wave; // A, B, C... for each wave
		//CombatHandler.startCombat(combatTag);
		//CombatHandler.TurnOrder(GameOption.getPlayerInventory());

		// increment global turn order + move to next wave
		GlobalOrderSet(GlobalOrderGet() + 1);
		wave++;
		//cutscene plays
		if (combatTag == 'A')
		{
			cutsceneCombatTutorial();
			CombatHandler.startCombat(combatTag);
			CombatHandler.TurnOrder(GameOption.getPlayerInventory());
			GlobalOrderSet(GlobalOrderGet() + 1);
			wave++;
		}
		// Example exit condition (adapt as needed)
		if (wave > 8)
		{
			GameOption.runMainMenu();
			CombatHandler.startCombat('H');
			CombatHandler.TurnOrder(GameOption.getPlayerInventory());
			GlobalOrderSet(GlobalOrderGet() + 1);
			GameEndState = true;
		}
	}
}

void Logic::DemoTurnOrder() {

	Option GameOption;
	Combat CombatHandler;
	Story story(GameOption.getPlayerInventory());
	Dungeon dungeon(GameOption.getPlayerDungeon(), GameOption.getPlayerInventory());
	


	story.ShowWave(0, 0);
	GameOption.runMainMenu();
	cutsceneCombatTutorial();
	CombatHandler.startCombat('A');
	CombatHandler.TurnOrder(GameOption.getPlayerInventory());
	story.ShowWave(2, 0);
	GameOption.runMainMenu();
	story.ShowWave(3, 0);
	story.ShowWave(6, 0);
	story.ShowWave(7, 0);
	story.ShowWave(8, 0);
}

void Logic::cutsceneCombatTutorial()
{
	Option GameOption;

	system("cls");
	std::cout << "Welcome to the Combat Tutorial!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "To attack, stand within range of the enemy. Weapons have different ranges,\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "and if multiple enemies are nearby, use A and D to cycle targets!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "There is other options in the combat sequence that you can also explore!";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	GameOption.waitForEnter();
	system("cls");

}
void Logic::cutsceneInventoryTutorial()
{
	Option GameOption;

	system("cls");

	std::cout << "Welcome to the Inventory Tutorial!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "You can equip, unequip, or use items.\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "Commands:\n";
	std::cout << " - equip [item]\n";
	std::cout << " - unequip [weapon/armor]\n";
	std::cout << " - use [Health Potion]\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	std::cout << "That's it! Easy, right? Now go fight, youngin'!\n";
	std::this_thread::sleep_for(std::chrono::milliseconds(2000));

	GameOption.waitForEnter();
	system("cls");
}
void Logic::cutsceneDungeonTutorial()
{
	Option GameOption;

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

	GameOption.waitForEnter();
	system("cls");
}

