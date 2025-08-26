#pragma once
#include <vector>
#include <string>
#include "StoryEntry.h"

class Story
{
private:
    std::vector<StoryEntry> database;   // holds all story lines
    bool GameActive;
public:
    Story();
    ~Story();

    void FactoryCreateStory(int wave, const std::string& story);
    void FactoryCreateChoices(int wave, int choice, const std::string& choicetext, const std::string& results, int quest, std::vector<int> nextChoices);
    void FactoryCreateChoices(int wave, int choice, const std::string& choicetext, const std::string& results, int quest, const int* nextChoices, size_t nextSize);
    void DatabaseInitialisation();
    void ShowWave(int wave, int choiceId = -1) const; // print stories for one wave

};


