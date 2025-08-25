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

        FactoryCreateChoices(2, 5,
            "Sure, I need the gold.",
            "Then, I'll be dropping this off with ya. @ (Quest to get 2 Fangs) @ Speaking of which, anything you want? (Opens Shop)@",
            1, nullptr, 0
        );

        // Choice 4
        int next4[] = { 7 };
        FactoryCreateChoices(2, 4,
            "Explore the hole.",
            "@You went into the mysterious hole. Saw a door in the cave and opened it. {Dungeon crawler mode} @",
            0, next4, 1
        );

        // Choice 7
        int next7[] = { 3 };
        FactoryCreateChoices(2, 7,
            "Get out of the cave and return to the trail.",
            "Getting out of the cave, the caravans are still there, just resting. You head towards the trail.",
            0, next7, 1
        );
        FactoryCreateStory(2, "Dave: Well then, see ya! We’ll be making our way back from here. The trail of caravans disappear into the horizon.");
        FactoryCreateStory(3, "It's getting pretty late now, time for you to have a nice sleep, once the local wildlife stops bothering you of course. As you drift into slumber, you are jolted awake by the weird groans from afar. Standing up, you prepare yourself to fight as you see a hoard of zombies creeping towards you. @");
        FactoryCreateStory(3, "As you lower your weapon, you look at the rotten corpses that were once roaming the forests. The fatigue has set in and now you lay down in the sea of bodies. @ The line of carriages is back, along with Dave. They approach where you once laid down.@ Dave: So ya back are ya? And with so many bodies as well. @Didja find find any of them thingymajiggies whilst you’re here?");
        FactoryCreateChoices(3, 8,
            "Offer the 2 Fangs#Hide them",
            "Dave: Perfect! That'll keep em satisfied, lemme just~ @Mysterious voice: Pass them over!@Rather on the small side, a woman approached them, as she left the carriage. Several assistants rush over, helping her carry other equipment, whether it be microscopes, or weird tablets.@The person quickly snatches the fangs away from you, she lowers a monocle, staring it.@Rude Mysterious Person: Mana Quality isn't dropping like most embedded objects. Check the Mh/P for me.@An assistant rushes over, poking a stick into it@Assistant: 150 over 20. Stable as well.@Mysterious Person: Did they temporarily stabilize the internal mana core using a van wiltz circle?@Assistant: Unlikely, there seems to be no residual leakage.@Mysterious Person: Odd, we'd need to bring this back for testing.. See if we can substitute the mana type for windless, or fire. If we can stimulate the channels here in any way, this area can get back to healing.@You: What is this girl yapping about.@Dave: Ya gotta greet guests ya know?@Mysterious girl: Oh yes. Yes, that's right. Call me Camilla, I’m a researcher in the institute of magic. By all means, I'm your contractor.@You: Right, so.@Camilla: Anyways, collect some Mana Core for me. The more the better, darkness types are always troublesome to unblock, especially when you need a natural mana conduit to do it with…@The girl starts quietly mumbling to herself as she walks away with her assistants.@Dave: Well don't mind ya, now that ya met ma boss now. So, as per normal, do ya accept the quest?@You: Sure, why not @Dave: Perfect, I'll get that sorted for ya. (Open shop)#Dave: Shame, well everyday can’t be winning. Keep a lookout for em though.",
            0, nullptr, 0);
        FactoryCreateStory(3, "The carriage strides away, disappearing as they get further and further away. Till they blend into the background. @");
        FactoryCreateStory(4, "The sun sets once again and you start to get weary of your surroundings. As you expected, another monster rises from the shadows. This time it is a huge gang of goblins rushing towards you as you ready yourself to fight. @ You slowly let yourself down as you think to yourself about the cycle of violence that has fallen upon you and wonder when will all of these come to an end.@ The caravan from the previous day has arrived, this time, hauling an extra carriage. It was a sleek black color, standing out from the rest of the white carriages. @ Dave: New day, new start, are ya buying anything this time? @");
        FactoryCreateStory(5, "After another harsh battle, the sun sets and it is time for you to rest. However this time, you expect that another wave of monster is rushing towards you, hence you don't rest. As expected, a swarm of mutated bats rush towards you and this time you are prepared to defend yourself.@ After a long harsh battle, you can finally rest. You think to yourself if you could ever escape from this nightmare. @You wake up to Dave's foot nudging your stomach.@Dave: Well I'll be darned. How're ye still up and kickin after all this? Look 'ere, boss wants to talk to ya'.@Camilla: After careful consideration, I have confirmed your reliability and strength which has been proved to be useful time and time again. What I'm saying is, I'd like to offer you a stable job.@...@Camilla: So, what will it be?@");
        FactoryCreateChoices(5, 11,
            "Handshake hire # Reject",
            "Camilla: They said you were a soldier from the previous era. That, I mean what was left of the old records.@Camilla: What was left of that wonderful era? I haven't had the chance to see it for myself.@Camilla takes a peak at John, looking at him, with slight excitement@Camilla: Now that is a wonderful story that the elders told of long ago. That may be the key to healing these lands.@Camilla: I will be frank with you at least, the lands aren’t stable. Leylines choked with too much foreign mana are bound to expel it sooner or later. When it does, it will undoubtedly cause the biggest mana reflux the continent has seen.You have what is essentially to unblock those passages, allowing the planet to breathe.@You: That's… a big responsibility.@Camilla: No doubt, but those are the facts that I have learned.@Camilla: Maybe once this is all over, we're able to look forward to a day where we don't get attacked by Hellhounds every night@Dave: That'll be nice, for sure.@Dave: We can't dilly dally all day. Let's get back to work.#Camilla: That's a shame.@Camilla: No matter, we must go forth. May you prosper in your travels, John.@Dave: This caravan aint returning anytime soon. So farewell, this may be the last time we're seeing each other.@Dave waves his hand, you do a small wave back@The caravan disappears into the distance. @GAME END: Homestaying",
            0, nullptr, 0);
        FactoryCreateStory(6, "You stay on guard as you survey the area, searching for any creatures lurking in the dark, ready to pounce on you. As you slowly come to a halt, you see an army of skeletons. You ready yourself to fight.@You are finally able to rest as you slowly have gotten used to this loop. You look around at the piles of bones as you realise this would be the second time you have fought the undead. Tired, you lay down and fall asleep.@The morning arrives, the caravan is here again, just as always. Camilla comes out, holding an odd looking contraption. It looked to be just small enough to be portable, one part container for an item, another part with a barrel jutting out.@Racking that old brain of yours, you recall that it looks oddly similar to a matter-mana resonance disruptor. Using the intricate mana-lengths that a natural item has to disrupt or dispel the properties of said item.@Camilla: You’ve seen this before? I notice that look in your eyes.@");
        FactoryCreateChoices(6, 12,
            "Where did you get such a dangerous thing from? (Suspicious)#Why do we need the disruptor? (Confused)#More fire-power is always welcome. (Accepting)",
            "Camilla: Something that I created based off of old texts. This precarious balance between insanity and practicality.@Camilla: Of course, its something I would destroy after I’m done with it. There’s no point of having such a dangerous thing around. The concept, elegant and an intuitive understanding of how mana wavelengths affect the particulate structure of molecules.@Camilla: Reversing the mana polarity of certain selected mana wavelengths causes the item to de-harmonize with the entire item. This causes a phenomenon called particulate instability, where said mana wave lengths directly change particulate bonds, destabilizing the item and risk it collapsing into the particles of the surroundings, effective ‘vaporizing’ the object. @Camilla: Such a shame, but I’ll have to destroy it.@You: But there’s no guarantee that you will destroy it, is there?@Camilla: I suppose there is not.@Camilla: Then to soothe your views.  I ask you to destroy it, with your own hands. Beat it, crunch it. Whatever that satisfies your desire for destruction.@You: What if you don’t keep your promise?@Camilla: Nothing can be said that is axiomically always sincere. That is up to you to determine.@You:...#Camilla: The expulsion of dark mana from the leylines of the earth is going to be a violent procedure. Inevitably such a large build-up of dark mana will inevitably trigger what is known as mana coagulation. You would notice this in forms of your zombies, bats, monsters.@Camilla: A sufficient built up of mana would convert surroundings particles into matter based on the mana’s wavelength instructions. This violent expulsion of dark mana, cleansing the leylines, will be the largest buildup of latent dark mana in the atmosphere since the death of the Devil Lanthania’s heart.@Camilla: An Abomination will be made. A creation of dark mana that all that is unholy.@Camilla: The later we defuse it, the more we risk a mana implosion, caused by the negative and positive wave lengths of dark mana physically affecting the world around it.@Camilla: The disruptor de-harmonizes the wavelengths of dark mana, making it so we delay the possibility of a mana-type implosion. Safely dispersing the mana into the air, for it to be sufficiently broken down, or reduced to safe levels.@You: The resulting creature will destroy everything around it?@Camilla: Yes, whether by being a mere beast, bludgeoning everything manually, no doubt poisoning the ecosystem by disrupting natural mana processes, or manifesting a mana implosion, causing the surrounding areas to be completely uninhabitable as the dark mana violently assimilates or integrates matter to vibrate at its wavelength.Camilla: That is why such a terrible weapon, and fascinating phenomenon is allowed to be used for such purposes.#Camilla: It will help us greatly for the final expulsion. I don’t doubt your strength, but there’s limits that the gods have set upon us. The bigger expulsion will be in 2 days, so that is when this would be ready. This machine is still incomplete. I request a Red Ruby, from the dungeons.@Camilla: If it’s too dangerous, there’s no need to push yourself. I will procure the red ruby at the site itself. But, I would.. like if I didn’t have to do it myself.@Quest activated: Red Ruby Excavation@(Camilla walks away)",
            0, nullptr, 0);
        FactoryCreateStory(7, "The sun slowly started to set, marking another night to survive. Familiar with this routine, you prepare for battle. Surely enough, you see a hoard of flying grey gargoyles rushing towards you.@You finally vanquished the gargoyles as you remember the first time you fought these monsters. You feel a sense of pride as you see how far you’ve come, though that feeling is short-lived as fatigue settles in. You slowly lay yourself down and drift into sleep.@The caravan is back, busier than ever. The device you say from yesterday, was kept, improved upon with a mass of wires going into and out of the main chamber. An extra lens was added inside the machine itself.@Camilla was working on the device, carving intricate patterns into the container. She notices you approaching them, jumping down from her high position, landing with a thump.@Camilla: Did you find the red ruby? I’m almost done on my end, just making sure the device works, that is.@Offer the Red Ruby?");
        FactoryCreateChoices(7, 12,
            "",
            "",
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
                    entry.quest[choiceId];
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