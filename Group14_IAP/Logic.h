#pragma once
class Logic
{
private:
	// Determines what part of the game to access//
	int GameOrderState;

	//Determines what progression the Player is at//
	int GlobalOrderState;

	//End Condition of the Game//
	bool GameEndState;
public:

	//Constructor//
	Logic();

	//Game Order Setter//
	void GameOrderStateSet(int CurrentGameOrder);

	//Game Order Getter//
	int GameOrderStateGet();

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

};

