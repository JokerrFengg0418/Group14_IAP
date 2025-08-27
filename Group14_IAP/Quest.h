#pragma once
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
	
	
public:

	Quest(std::string& Name, std::string& ItemName, int ItemCount);

	std::string GetName() const;

	void SetName(std::string& Name);

	QuestRequirement GetQuestRequirement() const;

	void SetQuestRequirement(std::string& ItemName, int req);

	void ChangeQuestState(int newState);

	int CheckQuestState() const;


};
 
