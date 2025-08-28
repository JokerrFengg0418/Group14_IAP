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
		if (status == 1) // Shop
		{
			GameOption.shopOption(GameOption.getPlayerInventory());
			choiceId = story.ShowWave(wave, 1);
		}
		if (status == 81)
		{
			GameOption.shopOption(GameOption.getPlayerInventory());
			choiceId = story.ShowWave(wave, 82);
		}
		if (status == 9)
		{
			GameOption.shopOption(GameOption.getPlayerInventory());
			choiceId = story.ShowWave(wave, 10);
		}
		if (status == 12)
		{
			GameOption.shopOption(GameOption.getPlayerInventory());
			choiceId = story.ShowWave(wave, 12);
		}
		else if (status == 2) // Dungeon
		{
			cutsceneDungeonTutorial();
			GameOption.waitForEnter();
			dungeon.dungeonOption();
			choiceId = story.ShowWave(wave, 7); // resume branch after dungeon
		}
		else if (status == 3) {
			GameEndState = true;
			return;
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

