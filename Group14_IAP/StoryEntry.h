#pragma once
#include <string>
#include <vector>

class StoryEntry
{
public:
    int wave;
    std::string text;

    // For choices
    std::vector<std::string> choicetext;
    std::vector<std::string> results;

    int choice;
    int quest;

    // Normal line
    StoryEntry(int w, const std::string& t);

    // Choice line
    StoryEntry(int w, int choice, const std::vector<std::string>& c, const std::vector<std::string>& r, int  quest);
};