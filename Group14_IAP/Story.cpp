#include <iostream>
#include "Story.h"
#include <sstream>

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

static std::vector<std::string> splitHash(const std::string& s)
{
    std::vector<std::string> out;
    std::stringstream ss(s);
    std::string seg;
    while (std::getline(ss, seg, '#'))
    {
        // trim whitespace at start/end
        seg.erase(seg.begin(),
            std::find_if(seg.begin(), seg.end(), [](unsigned char ch) {
                return !std::isspace(ch);
                }));
        seg.erase(std::find_if(seg.rbegin(), seg.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(),
                seg.end());

        if (!seg.empty()) out.push_back(seg);
    }
    return out;
}

void Story::FactoryCreateChoices(int wave, int choice,
    const std::string& choicetext,
    const std::string& results,
    int quest)
{
    auto choiceList = splitHash(choicetext);
    auto resultList = splitHash(results);

    database.emplace_back(wave, choice, choiceList, resultList, quest);
}

void Story::DatabaseInitialisation()
{
    //Wave 0//
    FactoryCreateStory(0, "After a magical fallout between two major kingdoms, the world is pretty much doomed. You spawn as John, the last soldier from the war that just ended.You claw out of the rocks that buried you and you fall onto the floor, as you look around, monsters start noticing you.You started slowly recovering your memory from head concussion. @");
    //Wave 1//
    FactoryCreateStory(1, "After the tough battle, you wandered around to find resources in order to stay alive. And you came across a camp. You investigate it and it turns out to be an empty desolate camp with no living soul in sight. @");
    FactoryCreateStory(1, "... @");
    FactoryCreateStory(1, "You noticed the presence of someone walking toward your direction, while you were on the verge of starving to death. This mysterious person saw that you were quivering from hunger and thirst, so he gave you food and water to help with it. @");
    FactoryCreateStory(1, "Mysterious Person: Huh, you're up... Found you half dead in the wastelands. What're you doing here, out in a death zone? @");
    //Choice 1
    FactoryCreateChoices(1, 1, "What do you sell? (Opens Shop) # I'm fine for now. # Do you travel here often? # What Happened?", "@Mysterious Person: That Depends, what're ya looking to buy? @ # @Mysterious Person: Alright, suitcha self. But beware, the nights are cold, so keep yourselves warm and alive. @ # @Nay, You're lucky bud. It's only once in a few months that I travel here. You’d better have more luck searching for caravans to trade with than anything. @ # @Mysterious Person:  Better question is, what happened to you? Nobody just wakes in the middle of nowhere, no memory or nothing. But what I'll tell ya, is that stuff aint right in this generation. Monsters appearing outta nowhere, spawning, just aint right. I'm no magiwizard, but there’s some things that just don't follow nature. @", 0);
    FactoryCreateStory(1, "@Dave: If ya can't tell, I'm a wandering traveller.I know, it's rare... But we do what we can to get by. Name's Dave, for your convenience. @");
    //Wave 2//
    FactoryCreateStory(2, "The sun wanes, as day turns to night. Tonight, you are not alone. Beasts growl as they look and see, hellhounds, should they believe. Hunting out, they shall tonight, watching for your very fright. @");
    FactoryCreateStory(2, "The wind calms, as the dogs remain. Down on the floor, they stay twitchin. A night a day, you save your skin. Now it's time to sleep in between. @");
    FactoryCreateStory(2, "As you wake up, the sun bears down on you. The beasts from the previous day are gone, as if no trace of them had remained, only ashes and shadows remain in their wake. Lifting yourself up, it's the start of a new day. What will you do? @");
    //Choice 2
    FactoryCreateChoices(2, 2, "Wander around until you find a point of interest # Find Water # Survey the area", "@After walking around for a bit, you discover a mysterious hole in the ground, right by a well. Seems there was civilization here, before whatever happened. Not only that, you notice that there seems to be a column of white carriages travelling through the area. @ # @After walking around for a bit, you discover a mysterious hole in the ground, right by a well. Seems there was civilization here, before whatever happened. Not only that, you notice that there seems to be a column of white carriages travelling through the area. @ #@After walking around for a bit, you discover a mysterious hole in the ground, right by a well. Seems there was civilization here, before whatever happened. Not only that, you notice that there seems to be a column of white carriages travelling through the area. @ ", 0);
    //Choice 3
    FactoryCreateChoices(2, 3, "Head towards the white carriages. # Explore the hole.", "@Heading towards there, you see a weird insignia, getting closer. Bearing what seems to be a royal coat of arms, but you don't remember which. @ Guard: Halt! In the name of Calypso! @ Player: (What the-) @ Dave: Didn't expect to meetcha here. Gimme a sec.. @ *Dave and the Guard huddled up, to talk with each other in private.* @ Guard: You may approach, just this once. The guard withdraws his spear. @ Dave: Gist is, a very interesting proposal came up... #You went into the mysterious hole to explore. You saw a door in the middle of the caveand being the curious guy you are, you opened it. {Dungeon crawler mode} @ #@ Player: Sure, I need the gold. # Player: I got better things to do. @", 0);
    FactoryCreateChoices(2, 4, "Sure, I need the gold. # I got better things to do.", "Then, I'll be dropping this off with ya. @ (Quest to get 2 (Placeholder)) @ Speaking of which, is there anything you want for me? @ (Opens Shop) @ # Shame, a loss is a loss. @ If I can't interest ya in that, do ya want to get anything? @ (Opens Shop) @ Dave: Well then, see ya! We'll be making our way back from here. The trail of caravans disappear into the horizon. @ Options: Explore the Hole (2). @", 1);
}

void Story::ShowWave(int wave) const
{
    std::cout << "=== Wave " << wave << " ===\n";

    for (const auto& entry : database)
    {
        if (entry.wave == wave)
        {
            // -------------------------
            // CASE 1: Story text
            // -------------------------
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

            // -------------------------
            // CASE 2: Choices
            // -------------------------
            if (!entry.choicetext.empty())
            {
                std::cout << "You are faced with choices:\n";
                for (size_t i = 0; i < entry.choicetext.size(); i++)
                {
                    std::cout << i + 1 << ". " << entry.choicetext[i] << "\n";
                }

                // Let the player pick
                int pick;
                std::cout << "Enter choice number: ";
                std::cin >> pick;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (pick > 0 && pick <= entry.results.size())
                {
                    std::string resultText = entry.results[pick - 1];

                    // --------- Show text in paragraphs (@) ---------
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

                    // --------- Nested branching (CHOICE:) ---------
                    if (resultText.find("CHOICE:") != std::string::npos)
                    {
                        // isolate after CHOICE:
                        std::string choiceBlock = resultText.substr(resultText.find("CHOICE:") + 7);
                        auto nestedChoices = splitHash(choiceBlock);

                        if (!nestedChoices.empty())
                        {
                            std::cout << "\nYou have new options:\n";
                            for (size_t i = 0; i < nestedChoices.size(); i++)
                            {
                                std::cout << i + 1 << ". " << nestedChoices[i] << "\n";
                            }

                            int nestedPick;
                            std::cout << "Enter choice number: ";
                            std::cin >> nestedPick;
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                            if (nestedPick > 0 && nestedPick <= nestedChoices.size())
                            {
                                std::cout << ">> " << nestedChoices[nestedPick - 1] << "\n";
                            }
                        }
                    }
                }
            }
        }
    }
}

void Story::PrintWithPause(const std::string& text)
{

}
