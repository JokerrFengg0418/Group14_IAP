#pragma once
#include <vector>
#include <string>
#include "StoryEntry.h"
#include "Inventory.h"
#include "Quest.h"

struct Node {
    std::string Instruction;
    std::string QuestName;
    int Wave;
    int ChoiceID;
    int Choice;

    Node(std::string instructions, int wave, int choiceID, int choice, std::string& QuestName);
};

class Story
{
private:
    std::vector<StoryEntry> database;   // holds all story lines
    std::vector<Quest> QuestDatabase; // holds all Quests
    std::vector<Node> NodeDatabase; // holds all Nodes
    Inventory* PlayerInventoryPointer; //Pointer to Access PlayerInventory
    bool GameActive;
public:
    Story(Inventory* PlayerInventory);
    ~Story();

    void FactoryCreateStory(int wave, const std::string& story);
    void FactoryCreateChoices(int wave, int choice, const std::string& choicetext, const std::string& results, int quest, const int* nextChoices, size_t nextSize);
    void DatabaseInitialisation();
    int ShowWave(int wave, int choiceId = -1); // print stories for one wave
    
    bool QuestHandler( int Wave, int ChoiceID, int Choice);
    void StartQuest(std::string& Name);
    void CompleteQuest(std::string& Name);
    void ForceEndQuest(std::string& Name);
    void CreateNode(std::string Instructions, int Wave, int ChoiceID, int Choice, std::string QuestName);
	void FactoryCreateQuest(std::string Name, std::string ItemName, int ItemCount);
    bool QuestFlagCheck(std::string Name, int DesiredState);
	void TypeWriter(const std::string& text, int delay_ms);
    

};


