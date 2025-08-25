#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Story.h"
#include "StoryEntry.h"

// Constructor: choice line
//StoryEntry::StoryEntry(int w, int choiceId, const std::vector<std::string>& c, const std::vector<std::string>& r, int q, const std::vector<int>& next)
//    : wave(w), choice(choiceId), choicetext(c), results(r), quest(q), nextChoices(next) {
//}

Story::Story() 
{
    DatabaseInitialisation(); 
}
Story::~Story()
{

}

void Story::FactoryCreateStory(int wave, const std::string& story)
{
    database.emplace_back(wave, story);
}

// Helper: split string by '#' into trimmed vector
static std::vector<std::string> splitHash(const std::string& s)
{
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string seg;
    while (std::getline(ss, seg, '#'))
    {
        seg.erase(seg.begin(),
            std::find_if(seg.begin(), seg.end(), [](unsigned char ch) { return !std::isspace(ch); }));
        seg.erase(std::find_if(seg.rbegin(), seg.rend(), [](unsigned char ch) { return !std::isspace(ch); }).base(), seg.end());
        if (!seg.empty()) out.push_back(seg);
    }
    return out;
}

void Story::FactoryCreateChoices(int wave, int choice, const std::string& choicetext, const std::string& results, int quest, std::vector<int> nextChoices)
{
    auto choiceList = splitHash(choicetext);
    auto resultList = splitHash(results);
    database.emplace_back(wave, choice, choiceList, resultList, quest, nextChoices);
}

// --- Database Initialization ---
void Story::DatabaseInitialisation()
{
    // Wave 0
   // Wave 0
    FactoryCreateStory(0, "After a magical fallout between two major kingdoms, the world is pretty much doomed. You spawn as John, the last soldier from the war that just ended. You claw out of the rocks that buried you and fall onto the floor. Monsters start noticing you. You slowly recover your memory from head concussion. @");

    // Wave 1
    FactoryCreateStory(1, "After the tough battle, you wandered around to find resources. You found a camp. It's empty. @");
    FactoryCreateStory(1, "... @");
    FactoryCreateStory(1, "Someone approaches while you are starving. They give you food and water. @");
    FactoryCreateStory(1, "Mysterious Person: Huh, you're up... What are you doing out in a death zone? @");

    // Choice 1
    FactoryCreateChoices(1, 1,
        "What do you sell? (Opens Shop) # I'm fine for now. # Do you travel here often? # What happened?",
        "@Mysterious Person: That depends, what're ya looking to buy? @ # @Mysterious Person: Alright, suit yourself. Nights are cold, so stay warm. @ # @Nay, you're lucky. I travel here once in a few months. @ # @Better question: What happened to you? Nobody wakes up with no memory. @",
        0, {}
    );

    FactoryCreateStory(1, "@Dave: I'm a wandering traveler. Name's Dave. @");

    // Wave 2
    FactoryCreateStory(2, "The sun wanes, as day turns to night. Tonight, you are not alone. Beasts growl. Hellhounds may appear. @");
    FactoryCreateStory(2, "The wind calms, dogs remain on the floor. Night falls, time to rest. @");
    FactoryCreateStory(2, "As you wake up, the sun bears down. Beasts are gone. Lifting yourself, it's a new day. What will you do? @");

    // Choice 2: point of interest
    FactoryCreateChoices(2, 2,
        "Wander around until you find a point of interest # Find Water # Survey the area",
        "@After walking, you find a mysterious hole by a well. You notice white carriages traveling. Seems there was civilization here, before whatever happened. @",
        0, {3,3,3}
    );

    // Choice 3: Carriages vs Hole
    FactoryCreateChoices(2, 3,
        "Head towards the white carriages. # Explore the hole.",
        "@Heading towards the carriages, you see a royal insignia. @ Guard: Halt! @ Player: (What the-) @ Dave: Didn't expect to meetcha. @ *Dave and Guard talk privately.* @ Guard: You may approach. @ Dave: Proposal coming up...",
        0,
        { 5, 4 } // nested after carriages
    );

    FactoryCreateChoices(2, 4,
        "Explore the hole.",
        "@You went into the mysterious hole. Saw a door in the cave and opened it. {Dungeon crawler mode} @",
        0,
        { 7, 8 } // nested inside cave
    );
  
    // Nested Choices after Carriages
    FactoryCreateChoices(2, 5,
        "Sure, I need the gold.",
        "Then, I'll be dropping this off with ya. @ (Quest placeholder) @ Speaking of which, anything you want? @ (Opens Shop)",
        1, {}
    );

    FactoryCreateChoices(2, 6,
        "I got better things to do.",
        "Shame, a loss is a loss. @ If I can't interest ya, want anything? @ Dave: See ya! The trail disappears.",
        0, {}
    );

    // Nested Choices after Cave
    FactoryCreateChoices(2, 7,
        "Investigate deeper into the cave.",
        "You move deeper into the cave, hearing dripping water and faint echoes...",
        0, {}
    );

    FactoryCreateChoices(2, 8,
        "Get out of the cave and return to the trail.",
        "Getting out of the cave, the caravans are still there, just resting. You head towards the trail.",
        0, {}
    );
}

// --- ShowWave ---
void Story::ShowWave(int wave, int choiceId) const
{
    for (const auto& entry : database)
    {
        if (entry.wave == wave && (choiceId == 0 || entry.choice == choiceId))
        {
            // Story text
            if (!entry.text.empty())
            {
                std::stringstream ss(entry.text);
                std::string segment;
                while (std::getline(ss, segment, '@'))
                {
                    if (!segment.empty())
                    {
                        std::cout << segment << "\n";
                        if (!ss.eof())
                        {
                            std::cout << "Press Enter to continue...\n";
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        }
                    }
                }
            }

            // Choices
            if (!entry.choicetext.empty())
            {
                std::cout << "You are faced with choices:\n";
                for (size_t i = 0; i < entry.choicetext.size(); i++)
                    std::cout << i + 1 << ". " << entry.choicetext[i] << "\n";

                int pick;
                std::cout << "Enter choice number: ";
                std::cin >> pick;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (pick > 0 && pick <= entry.results.size())
                {
                    std::string resultText = entry.results[pick - 1];
                    std::stringstream ss(resultText);
                    std::string segment;
                    while (std::getline(ss, segment, '@'))
                    {
                        if (!segment.empty())
                        {
                            std::cout << segment << "\n";
                            if (!ss.eof())
                            {
                                std::cout << "Press Enter to continue...\n";
                                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            }
                        }
                    }
                }

                if (pick > 0 && pick <= entry.nextChoices.size())
                {
                    int nextChoiceId = entry.nextChoices[pick - 1];
                    ShowWave(wave, nextChoiceId); // recurse into next choice
                    return;
                }
            }
        }
    }
}