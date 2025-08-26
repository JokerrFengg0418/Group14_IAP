#pragma once
#include "Story.h"
#include <string>

enum class QuestState { NotStarted, InProgress, Completed };

struct QuestRequirement {
	std::string ItemName;
	int Count;
};

class Quest
{
private:

	QuestRequirement Requirement;
	QuestState State;
	std::string Name;
	int ID;
	int Wave;
	
public:

	Quest(int Wave, int ID, std::string& Name, std::string& ItemName, int ItemCount);

	int GetWave() const;

	void SetWave(int Wave);

	int GetQuestID() const;

	void SetQuestID(int ID);

	QuestRequirement GetQuestRequirement() const;

	void SetQuestRequirement(std::string& ItemName, int req);

	void ChangeQuestState(int newState);

	int CheckQuestState() const;
};
 
