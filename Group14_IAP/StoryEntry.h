#pragma once
#include <string>
#include <vector>

class StoryEntry
{

public:
    int wave;
    std::string text;

    std::vector<std::string> choicetext;
    std::vector<std::string> results;

    int choice;
    int quest;

    std::vector<int> nextChoices;

    StoryEntry(int w, const std::string& t);

    StoryEntry(int w, int choiceId, const std::vector<std::string>& c,
        const std::vector<std::string>& r, int q,
        const std::vector<int>& nextChoices);  // ✅ Default ONLY here
};