#include "Logic.h"
#include "Combat.h"
#include "Option.h"

Logic::Logic() {

	GameOrderState = 1;
	GlobalOrderState = 1;
	GameEndState = false;

}





void Logic::GameOrderStateSet(int CurrentGameOrder) {
	
	GameOrderState = CurrentGameOrder;

}




int Logic::GameOrderStateGet() {

	return GameOrderState;

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

	while (GameEndState == false) {

		Option::displayMenu();


	}

}