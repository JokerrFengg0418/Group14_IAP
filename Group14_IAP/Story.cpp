#include <iostream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "Story.h"
#include "StoryEntry.h"
#include "Quest.h"


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

void Story::FactoryCreateChoices(int wave, int choice, const std::string& choicetext,
    const std::string& results, int quest,
    const int* nextChoices, size_t nextSize)
{
    auto choiceList = splitHash(choicetext);
    auto resultList = splitHash(results);
    database.emplace_back(wave, choice, choiceList, resultList, quest, nextChoices, nextSize);
}

// --- Database Initialization ---
void Story::DatabaseInitialisation()
{
    while (GameActive != false)
    {
        // Wave 0
        FactoryCreateStory(0, "After a magical fallout between two major kingdoms, the world is pretty much doomed. You spawn as John, the last soldier from the war that just ended. You claw out of the rocks that buried you and fall onto the floor. Monsters start noticing you. You slowly recover your memory from head concussion. @");

        // Wave 1
        FactoryCreateStory(1, "After the tough battle, you wandered around to find resources. You found a camp. It's empty. @");
        FactoryCreateStory(1, "... @");
        FactoryCreateStory(1, "Someone approaches while you are starving. They give you food and water. @");
        FactoryCreateStory(1, "Mysterious Person: Huh, you're up... What are you doing out in a death zone? @");

        // Choice 1 — no next choices
        FactoryCreateChoices(1, 1,
            "What do you sell? (Opens Shop) # I'm fine for now. # Do you travel here often? # What happened?",
            "@Mysterious Person: That depends, what're ya looking to buy? @ # @Mysterious Person: Alright, suit yourself. Nights are cold, so stay warm. @ # @Nay, you're lucky. I travel here once in a few months. @ # @Better question: What happened to you? Nobody wakes up with no memory. @",
            0, nullptr, 0
        );

        FactoryCreateStory(1, "@Dave: I'm a wandering traveler. Name's Dave. @");

        // Wave 2
        FactoryCreateStory(2, "The sun wanes, as day turns to night. Tonight, you are not alone. Beasts growl. Hellhounds may appear. @");
        FactoryCreateStory(2, "The wind calms, dogs remain on the floor. Night falls, time to rest. @");
        FactoryCreateStory(2, "As you wake up, the sun bears down. Beasts are gone. Lifting yourself, it's a new day. What will you do? @");

        // Choice 2
        int next2[] = { 3, 3, 3 };
        FactoryCreateChoices(2, 2,
            "Wander around until you find a point of interest # Find Water # Survey the area",
            "@After walking, you find a mysterious hole by a well. You notice white carriages traveling. Seems there was civilization here, before whatever happened. @ # @After walking, you find a mysterious hole by a well. You notice white carriages traveling. Seems there was civilization here, before whatever happened. @ # @After walking, you find a mysterious hole by a well. You notice white carriages traveling. Seems there was civilization here, before whatever happened. @",
            0, next2, 3
        );

        // Choice 3
        int next3[] = { 8, 4 };
        FactoryCreateChoices(2, 3,
            "Head towards the white carriages. # Explore the hole.",
            "@Heading towards the carriages, you see a royal insignia. @ Guard: Halt! @ Player: (What the-) @ Dave: Didn't expect to meetcha. @ *Dave and Guard talk privately.* @ Guard: You may approach. @ Dave: Proposal coming up...",
            0, next3, 2
        );

        int next31[] = { 5, 6 };
        FactoryCreateChoices(2, 8,
            "Sure, I need the gold. # I got better things to do.",
            "",
            0, next31, 2
        );

        // Choice 4
        int next4[] = { 7 };
        FactoryCreateChoices(2, 4,
            "Explore the hole.",
            "@You went into the mysterious hole. Saw a door in the cave and opened it. {Dungeon crawler mode} @",
            0, next4, 1
        );

        // Choice 5 — no next choices
        FactoryCreateChoices(2, 5,
            "Sure, I need the gold.",
            "Then, I'll be dropping this off with ya. @ (Quest placeholder) @ Speaking of which, anything you want? @ (Opens Shop)",
            1, nullptr, 0
        );

        // Choice 6
        FactoryCreateChoices(2, 6,
            "Shame, a loss is a loss. ",
            "@ If I can't interest ya, want anything? @ Dave: See ya! The trail disappears.",
            0, nullptr, 0
        );

        // Choice 7
        int next7[] = { 3 };
        FactoryCreateChoices(2, 7,
            "Get out of the cave and return to the trail.",
            "Getting out of the cave, the caravans are still there, just resting. You head towards the trail.",
            0, next7, 1
        );

        FactoryCreateStory(3, "It's getting pretty late now, time for you to have a nice sleep, once the local wildlife stops bothering you of course. As you drift into slumber, you are jolted awake by the weird groans from afar. Standing up, you prepare yourself to fight as you see a hoard of zombies creeping towards you. @");
        FactoryCreateStory(3, "As you lower your weapon, you look at the rotten corpses that were once roaming the forests. The fatigue has set in and now you lay down in the sea of bodies. @ The line of carriages is back, along with Dave. They approach where you once laid down.@ Dave: So ya back are ya? And with so many bodies as well. @");
        while (quest1 != false)
        {
            FactoryCreateChoices(3, 9,
                "",
                "",
                0, nullptr, 0);
            quest1 = false;
        }
        FactoryCreateStory(3, "The carriage strides away, disappearing as they get further and further away. Till they blend into the background. @");
        FactoryCreateStory(4, "The sun sets once again and you start to get weary of your surroundings. As you expected, another monster rises from the shadows. This time it is a huge gang of goblins rushing towards you as you ready yourself to fight. @ You slowly let yourself down as you think to yourself about the cycle of violence that has fallen upon you and wonder when will all of these come to an end.@ The caravan from the previous day has arrived, this time, hauling an extra carriage. It was a sleek black color, standing out from the rest of the white carriages. @ Dave: New day, new start, are ya buying anything this time? @");
        while (quest2 != false)
        {
            FactoryCreateChoices(4, 10,
                "",
                "",
                0, nullptr, 0);
            quest2 = false;
        }
        FactoryCreateStory(5, "After another harsh battle, the sun sets and it is time for you to rest. However this time, you expect that another wave of monster is rushing towards you, hence you don't rest. As expected, a swarm of mutated bats rush towards you and this time you are prepared to defend yourself.@ After a long harsh battle, you can finally rest. You think to yourself if you could ever escape from this nightmare. @You wake up to Dave's foot nudging your stomach.@Dave: Well I'll be darned. How're ye still up and kickin after all this? Look 'ere, boss wants to talk to ya'.@Camilla: After careful consideration, I have confirmed your reliability and strength which has been proved to be useful time and time again. What I'm saying is, I'd like to offer you a stable job.@...@Camilla: So, what will it be?@");
        FactoryCreateChoices(5, 11,
            "Handshake hire # Reject",
            "Camilla: They said you were a soldier from the previous era. That, I mean what was left of the old records.@Camilla: What was left of that wonderful era? I haven't had the chance to see it for myself.@Camilla takes a peak at John, looking at him, with slight excitement@Camilla: Now that is a wonderful story that the elders told of long ago. That may be the key to healing these lands.@Camilla: I will be frank with you at least, the lands aren’t stable. Leylines choked with too much foreign mana are bound to expel it sooner or later. When it does, it will undoubtedly cause the biggest mana reflux the continent has seen.You have what is essentially to unblock those passages, allowing the planet to breathe.@You: That's… a big responsibility.@Camilla: No doubt, but those are the facts that I have learned.@Camilla: Maybe once this is all over, we're able to look forward to a day where we don't get attacked by Hellhounds every night@Dave: That'll be nice, for sure.@Dave: We can't dilly dally all day. Let's get back to work.#Camilla: That's a shame.@Camilla: No matter, we must go forth. May you prosper in your travels, John.@Dave: This caravan aint returning anytime soon. So farewell, this may be the last time we're seeing each other.@Dave waves his hand, you do a small wave back@The caravan disappears into the distance. @GAME END: Homestaying",
            0, nullptr, 0);
    }
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
                //check quest number//

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