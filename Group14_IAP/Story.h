#pragma once
#include <vector>
#include <string>
#include "StoryEntry.h"
#include "Inventory.h"
#include "Quest.h"

struct Node {
    std::string Instruction;
    int wave;
    int ChoiceID;
    int Choice;
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
    void ShowWave(int wave, int choiceId = -1); // print stories for one wave
    
    void QuestHandler( int Wave, int ChoiceID, int Choice);
    void StartQuest(int Wave, int ChoiceID);
    void CompleteQuest(int Wave, int ChoiceID);
    void ForceEndQuest(int Wave, int ChoiceID);
    void CreateNode(std::string Instructions, int Wave, int ChoiceID, int Choice);
	void FactoryCreateQuest(int Wave, int ID, std::string& Name, std::string& ItemName, int ItemCount);
    

};


