#include "Quest.h"
#include "Story.h"


//Initialiser, Wave determines chunk, ID determines specific choice, Item name and Item Count for requirements
Quest::Quest( std::string& Name, std::string& ItemName, int ItemCount) {

	
	this->Name = Name;
	Requirement.ItemName = ItemName;
	Requirement.Count = ItemCount;
	State = QuestState::NotStarted;
	
}


//Getters Setters

QuestRequirement Quest::GetQuestRequirement() const {

	return Requirement;
}

void Quest::SetQuestRequirement(std::string& ItemName, int req) {

	Requirement.ItemName = ItemName;
	Requirement.Count = req;
}

void Quest::ChangeQuestState(int newState) {

	switch (newState) {
	case '0':
		State = QuestState::NotStarted;
		break;
	case '1':
		State = QuestState::InProgress;
		break;
	case '2':
		State = QuestState::Completed;
		break;
	}
}

int Quest::CheckQuestState() const {
	return int(State);
}

std::string Quest::GetName() const {
	return Name;
}

void Quest::SetName(std::string& Name) {
	this->Name = Name;
}

