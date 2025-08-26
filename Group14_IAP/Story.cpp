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
        // Wave 0
        FactoryCreateStory(0, "After a magical fallout between two major kingdoms, the world is pretty much doomed. You spawn as John, the last soldier from the war that just ended. You claw out of the rocks that buried you and fall onto the floor. Monsters start noticing you. You slowly recover your memory from head concussion. @");

        // Wave 1
        FactoryCreateStory(1, "After the tough battle, you wandered around to find resources. You found a camp. It's empty. @");
        FactoryCreateStory(1, "... @");
        FactoryCreateStory(1, "Someone approaches while you are starving. They give you food and water. @");
        FactoryCreateStory(1, "Mysterious Person: Huh, you're up... What are you doing out in a death zone? @");
        FactoryCreateChoices(1, 0,
            "I'm John, I found myself laying in a pile of rubble and I.. uh. (calm)#Who are you and what are ye lookin at! (aggressive)#You ready your sword. 'Who are you?!' (defensive)",
            "Mysterious Person: Hah! Knocked out by a pile of rubble, what a piece of work you are!@Mysterious Person: I've taken the courtesy to patch anything that you broke, but that'll be your last help.@#Mysterious Person: I'm looking atcha, if ya cant tell. And if ya want ma name, well ask it properly yerself. No way to introduce yourself to yer savior.@#Mysterious Person: Woah woah woah… Bud, Calm down ye. I am just tryna help you. Put that sword away before I fold ya in half. Isn't it common courtesy to introduce yourself to your savior instead of tryna kill them?@Mysterious Person: If ya can't tell, I'm a wandering traveller.@Mysterious Person:  know I know, it's rare...but we do what we can to get by.@Mysterious Person: Name's Dave, for your convenience. @",
            0, nullptr, 0);
        int next[] = { 1, 2, 1, 1 };
        FactoryCreateChoices(1, 1,
            "What do you sell? (Opens Shop) # I'm fine for now. # Do you travel here often? # What happened?",
            "@Dave: That depends, what're ya looking to buy? @ # @Dave: Alright, suit yourself. Nights are cold, so stay warm. @The sun wanes, as day turns to night. Tonight, you are not alone. Beasts growl as they look and see, hellhounds, should they believe. Hunting out, they shall tonight, watching for your very fright.@ # @Dave: Nay, you're lucky. I travel here once in a few months. @ # @Dave: Better question is, what happened to you? Nobody wakes up with no memory. @",
            0, next, 4
        );
        FactoryCreateStory(2, "The wind calms, as the dogs remain. Down on the floor, they stay twitchin. A night a day, you save your skin. Now it's time to sleep in between.@");
        FactoryCreateStory(2, "As you wake up, the sun bears down on you. The beasts from the previous day are gone, as if no trace of them had remained, only ashes and shadows remain in their wake. Lifting yourself up, it's the start of a new day. What will you do?@");

        // Choice 2
        int next2[] = { 3, 3, 3 };
        FactoryCreateChoices(2, 2,
            "Wander around until you find a point of interest # Find Water # Survey the area",
            "@After walking, you find a mysterious hole by a well. You notice white carriages traveling. Seems there was civilization here, before whatever happened. @ # @After walking, you find a mysterious hole by a well. You notice white carriages traveling. Seems there was civilization here, before whatever happened. @ # @After walking, you find a mysterious hole by a well. You notice white carriages traveling. Seems there was civilization here, before whatever happened. @",
            0, next2, 3
        );

        // Choice 3
        int next3[] = { 5, 4 };
        FactoryCreateChoices(2, 3,
            "Head towards the white carriages. # Explore the hole.",
            "@Heading towards the carriages, you see a royal insignia. @ Guard: Halt! @ Player: (What the-) @ Dave: Didn't expect to meetcha. @ *Dave and Guard talk privately.* @ Guard: You may approach. @ Dave: Gist is, a very interesting proposal came up. See, this place isn’t exactly the most hospitable. Luckily for you, the research ladies want some data from here.@Dave: So, you get em what they want, and ya get gold. See, G O L D! Ya won’t meet em, but they’ll be travelling through here often. What do ya say?",
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
        FactoryCreateStory(2, "Dave: Well then, see ya! We’ll be making our way back from here. @The trail of caravans disappear into the horizon.");
        FactoryCreateStory(2, "It's getting pretty late now, time for you to have a nice sleep, once the local wildlife stops bothering you of course. As you drift into slumber, you are jolted awake by the weird groans from afar. Standing up, you prepare yourself to fight as you see a hoard of zombies creeping towards you. @");
        FactoryCreateStory(3, "As you lower your weapon, you look at the rotten corpses that were once roaming the forests. The fatigue has set in and now you lay down in the sea of bodies. @ The line of carriages is back, along with Dave. They approach where you once laid down.@ Dave: So ya back are ya? And with so many bodies as well. @Didja find find any of them thingymajiggies whilst you’re here?");
        FactoryCreateChoices(3, 8,
            "Offer the 2 Fangs#Hide them",
            "Dave: Perfect! That'll keep em satisfied, lemme just~ @Mysterious voice: Pass them over!@Rather on the small side, a woman approached them, as she left the carriage. Several assistants rush over, helping her carry other equipment, whether it be microscopes, or weird tablets.@The person quickly snatches the fangs away from you, she lowers a monocle, staring it.@Rude Mysterious Person: Mana Quality isn't dropping like most embedded objects. Check the Mh/P for me.@An assistant rushes over, poking a stick into it@Assistant: 150 over 20. Stable as well.@Mysterious Person: Did they temporarily stabilize the internal mana core using a van wiltz circle?@Assistant: Unlikely, there seems to be no residual leakage.@Mysterious Person: Odd, we'd need to bring this back for testing.. See if we can substitute the mana type for windless, or fire. If we can stimulate the channels here in any way, this area can get back to healing.@You: What is this girl yapping about.@Dave: Ya gotta greet guests ya know?@Mysterious girl: Oh yes. Yes, that's right. Call me Camilla, I'm a researcher in the institute of magic. By all means, I'm your contractor.@You: Right, so.@Camilla: Anyways, collect some Mana Core for me. The more the better, darkness types are always troublesome to unblock, especially when you need a natural mana conduit to do it with…@The girl starts quietly mumbling to herself as she walks away with her assistants.@Dave: Well don't mind ya, now that ya met ma boss now. So, as per normal, do ya accept the quest?@You: Sure, why not @Dave: Perfect, I'll get that sorted for ya. (Open shop)#Dave: Shame, well everyday can't be winning. Keep a lookout for em though.",
            0, nullptr, 0);
        FactoryCreateStory(3, "The carriage strides away, disappearing as they get further and further away. Till they blend into the background. @");
        FactoryCreateStory(3, "The sun sets once again and you start to get weary of your surroundings. As you expected, another monster rises from the shadows. This time it is a huge gang of goblins rushing towards you as you ready yourself to fight. @ ");
        FactoryCreateStory(4, "You slowly let yourself down as you think to yourself about the cycle of violence that has fallen upon you and wonder when will all of these come to an end.@ The caravan from the previous day has arrived, this time, hauling an extra carriage. It was a sleek black color, standing out from the rest of the white carriages. @ Dave: New day, new start, are ya buying anything this time? @");
        FactoryCreateStory(4, "After another harsh battle, the sun sets and it is time for you to rest. However this time, you expect that another wave of monster is rushing towards you, hence you don't rest. As expected, a swarm of mutated bats rush towards you and this time you are prepared to defend yourself.@");
        FactoryCreateStory(5, "After a long harsh battle, you can finally rest.You think to yourself if you could ever escape from this nightmare. @You wake up to Dave's foot nudging your stomach.@Dave: Well I'll be darned.How're ye still up and kickin after all this? Look 'ere, boss wants to talk to ya'.@Camilla: After careful consideration, I have confirmed your reliability and strength which has been proved to be useful time and time again. What I'm saying is, I'd like to offer you a stable job.@...@Camilla: So, what will it be?@");
        FactoryCreateChoices(5, 11,
            "Handshake hire # Reject",
            "Camilla: They said you were a soldier from the previous era. That, I mean what was left of the old records.@Camilla: What was left of that wonderful era? I haven't had the chance to see it for myself.@Camilla takes a peak at John, looking at him, with slight excitement@Camilla: Now that is a wonderful story that the elders told of long ago. That may be the key to healing these lands.@Camilla: I will be frank with you at least, the lands aren't stable. Leylines choked with too much foreign mana are bound to expel it sooner or later. When it does, it will undoubtedly cause the biggest mana reflux the continent has seen.You have what is essential to unblock those passages, allowing the planet to breathe.@You: That's... a big responsibility.@Camilla: No doubt, but those are the facts that I have learned.@Camilla: Maybe once this is all over, we're able to look forward to a day where we don't get attacked by Hellhounds every night@Dave: That'll be nice, for sure.@Dave: We can't dilly dally all day. Let's get back to work.@#Camilla: That's a shame.@Camilla: No matter, we must go forth. May you prosper in your travels, John.@Dave: This caravan aint returning anytime soon. So farewell, this may be the last time we're seeing each other.@Dave waves his hand, you do a small wave back@The caravan disappears into the distance. @GAME END: Homestaying@",
            0, nullptr, 0);
        FactoryCreateStory(5, "You stay on guard as you survey the area, searching for any creatures lurking in the dark, ready to pounce on you. As you slowly come to a halt, you see an army of skeletons. You ready yourself to fight.@You are finally able to rest as you slowly have gotten used to this loop. @"); 
        FactoryCreateStory(6, " You look around at the piles of bones as you realise this would be the second time you have fought the undead.Tired, you lay down and fall asleep.@The morning arrives, the caravan is here again, just as always.Camilla comes out, holding an odd looking contraption.It looked to be just small enough to be portable, one part container for an item, another part with a barrel jutting out.@Racking that old brain of yours, you recall that it looks oddly similar to a matter - mana resonance disruptor.Using the intricate mana - lengths that a natural item has to disrupt or dispel the properties of said item.@Camilla: You've seen this before? I notice that look in your eyes.@");
        int next8[] = { 12, 12, 13 };
        FactoryCreateChoices(6, 12,
            "Where did you get such a dangerous thing from? (Suspicious)#Why do we need the disruptor? (Confused)#More fire-power is always welcome. (Accepting)",
            "Camilla: Something that I created based off of old texts. This precarious balance between insanity and practicality.@Camilla: Of course, its something I would destroy after I'm done with it. There's no point of having such a dangerous thing around. The concept, elegant and an intuitive understanding of how mana wavelengths affect the particulate structure of molecules.@Camilla: Reversing the mana polarity of certain selected mana wavelengths causes the item to de-harmonize with the entire item. This causes a phenomenon called particulate instability, where said mana wave lengths directly change particulate bonds, destabilizing the item and risk it collapsing into the particles of the surroundings, effective 'vaporizing' the object. @Camilla: Such a shame, but I'll have to destroy it.@You: But there's no guarantee that you will destroy it, is there?@Camilla: I suppose there is not.@Camilla: Then to soothe your views.  I ask you to destroy it, with your own hands. Beat it, crunch it. Whatever that satisfies your desire for destruction.@You: What if you don't keep your promise?@Camilla: Nothing can be said that is axiomically always sincere. That is up to you to determine.@You:...#Camilla: The expulsion of dark mana from the leylines of the earth is going to be a violent procedure. Inevitably such a large build-up of dark mana will inevitably trigger what is known as mana coagulation. You would notice this in forms of your zombies, bats, monsters.@Camilla: A sufficient build up of mana would convert surroundings particles into matter based on the mana's wavelength instructions. This violent expulsion of dark mana, cleansing the leylines, will be the largest buildup of latent dark mana in the atmosphere since the death of the Devil Lanthania's heart.@Camilla: An Abomination will be made. A creation of dark mana that all that is unholy.@Camilla: The later we defuse it, the more we risk a mana implosion, caused by the negative and positive wave lengths of dark mana physically affecting the world around it.@Camilla: The disruptor de-harmonizes the wavelengths of dark mana, making it so we delay the possibility of a mana-type implosion. Safely dispersing the mana into the air, for it to be sufficiently broken down, or reduced to safe levels.@You: The resulting creature will destroy everything around it?@Camilla: Yes, whether by being a mere beast, bludgeoning everything manually, no doubt poisoning the ecosystem by disrupting natural mana processes, or manifesting a mana implosion, causing the surrounding areas to be completely uninhabitable as the dark mana violently assimilates or integrates matter to vibrate at its wavelength.@Camilla: That is why such a terrible weapon, and fascinating phenomenon is allowed to be used for such purposes.#Camilla: It will help us greatly for the final expulsion. I don't doubt your strength, but there's limits that the gods have set upon us. The bigger expulsion will be in 2 days, so that is when this would be ready. This machine is still incomplete. I request a Red Ruby, from the dungeons.@Camilla: If it's too dangerous, there's no need to push yourself. I will procure the red ruby at the site itself. But, I would.. like if I didn't have to do it myself.@Quest activated: Red Ruby Excavation@Camilla walks away",
            0, next8, 3);
        FactoryCreateStory(6, "The sun slowly started to set, marking another night to survive. Familiar with this routine, you prepare for battle. Surely enough, you see a hoard of flying grey gargoyles rushing towards you.@");
        FactoryCreateStory(7, "You finally vanquished the gargoyles as you remember the first time you fought these monsters. You feel a sense of pride as you see how far you've come, though that feeling is short-lived as fatigue settles in. You slowly lay yourself down and drift into sleep.@The caravan is back, busier than ever. The device you say from yesterday, was kept, improved upon with a mass of wires going into and out of the main chamber. An extra lens was added inside the machine itself.@Camilla was working on the device, carving intricate patterns into the container. She notices you approaching them, jumping down from her high position, landing with a thump.@Camilla: Did you find the red ruby? I'm almost done on my end, just making sure the device works, that is.@Offer the Red Ruby?");
        int next9[] = { 14 };
        FactoryCreateChoices(7, 13,
            "Yes#No",
            "You: Found this Red Ruby in the dungeons, among a bunch of scattered remains.@Camilla: Lets hope that their ends weren't that gruesome..@Camilla: Come on-@She hops up, extending her hand for you to pass the Red Ruby. You do so, as she inspects the Ruby@Camilla: This.. should suffice, the quality is enough so that resonance scattering isnt an issue.@She places the ruby into the chamber, carefully. As she locks the chamber tight. Turning on several switches, the Red Ruby starts levitating, suspended by some aura.@Camilla: Locking Resonance signature..@Camilla looks at the chamber, manually adjusting knobs and buttons. She presses a button, finished with her adjustments.@Camilla: Dave!@Dave: Yeah?@Dave walks over, having being called. He looks over, at the machine.@Camilla: Rotate this thing for me!@The machine starts whirring louder and louder, as if powering up or charging an external source of storage.@Dave: Where do you want this thing to point at?@Camilla: Just by the forestline!@Dave: Gimme a sec..@Dave gets the lone guardsman from days ago, and they shift it, being careful to not to tilt the device@Dave: Ready on yer say.@Camilla: Capturing mana normalization readings..@Dave: You sure this is safe?@Camilla: Yes.. Well it should be.@Dave: Should be aint exactly a comforting answer now is it?@Camilla: That’s the best answer we got!@Camilla: Firing now!@The Red Ruby shines bright, through the chamber’s opaque covering, blinding everything for a second. A large beam shot out of the chamber, hitting the treeline in earnestly. Then the beam dissipates, leaving no trace that it was even there.@Dave: All this preparation for that?@Camilla: Give it a second or two.@The chamber starts heating up immensely, as steam leaves the chamber. The Ruby, red hot, exhales hard, as several components of the device starts to whirr. The chamber, once filled with just the crystal, was flooded with ice.@It melts quickly, as the ice is chuted away from the chamber, replaced by more ice. In a sort of recycling system, constantly cooling the Ruby.@Camilla: Watch the trees!@The trees, normal just a second ago, start warping. What once was solid, seemed to be blending together in a kaleidoscope of colors.@Dave: Get back!@Dave: He sets down the weapon, recognising the danger, as he hides behind a piece of rock cover.@Camilla: It’s so beautiful..@You find yourself instinctively pulling Camilla back from the impact point, running away from the area.@Camilla: Let me see the implosion!@The impact point blends with what was and what once was. Resonating out from the centre, what seems to be the color of the very space itself gets sucked into a singular point. As trees, ground and anything near it start to vanish, collecting into this singular point.@You: Put your head down!@The Singular point vanishes, before a massive wave of heat is released. The space where Camilla’s head used to be at, shot out a large surge of mana, visible even to the naked eye.@The rock visibly heated up,  as it started melting down into its base components, Rock became minerals, quartz, crystals, melting down, separating, before turning into component slurry.@The magic pulse dissipated over distance, as the worst has been weathered by the crew. Camilla looked at you, then at where her head used to be.@Camilla: How did you know?@You: Know what?@Camilla: Know that that reaction was going to happen. Everything should have been in my calculations..@Staying silent, you look at Dave. Then back to Camilla.@You: Not sure myself, but I know these types of explosions, they are always bigger than anticipated.@Camilla: …@Camilla peaked up looking at the destruction.@Camilla: Thank you, for saving my life. Who knows what might have happened if that wave really did..@Camilla: Hit me…@Dave: That thing is, way too powerful! Any more and we might kill ourselves in the process! Young lady, this is a step too far in the wrong direction!@Camilla: But.. The thing is worth the risk, if we did use it on the abomination we won’t even need to fight it!@Dave: And risk us dying because of a freak accident like this? Sorry to say this but, not everybody would appreciate this kind of end!@Camilla: …@Camilla looks the area of impact, replaced with just air. As if somebody carved up the world and removed it, and left technicolor slurry in its place.@Camilla: Alright.. We won’t use the Matter-Mana Resonance Disruptor. But, if at any point if we start losing the fight, we fire it off.@Dave:.. Fair enough. But, only if there is no other option.@Camilla: …Alright.@It was clear to everybody that the room was visibly in a bad mood. Camilla decided to take a break, walking away from the main pack.@Dave:...@You:...@Dave: I was probably too harsh on the young lady.#Camilla: Ah, that complicates thing.. But it shouldn’t matter regardless. The Device will be ready by the end of the day.@Dave: Ya going down into the dungeon yourself?@Camilla: I can handle myself Dave, Im not a child anymore.@Dave: Sure doesn’t feel like it.@Camilla: You just say that because you don’t want to feel old.@Dave: Whatever you say kiddo.@Camilla: Kiddo?! I’ll show you kiddo you-@Camilla proceeds to chase around Dave, as he runs, hiding and disappearing.@Camilla: Where did he go.. Aha!@Camilla proceeds to jump into a bush, pulling Dave out.@Camilla: There you go!@Dave: Ya got me there, can’t lie.@The bickering continued, as they settled down.@Camilla: No matter, the red ruby isn’t an issue. I’ll be heading down down on my own.@Dave: At least bring a guard with you, they’re there for a reason.@Camilla: Ah, fine. I’ll take one guard with me.@Dave, being satisfied, walks away. Camilla does too, no doubt to prepare to dive into the depths.",
            0, next9, 2);

        FactoryCreateChoices(7, 14,
            "Agree#Disagree",
            "You: Probably.. How old is she? Can’t be older than 19..@Dave: She never told her age, did she? She’s around there, too young to be holding the world up by her shoulders@You: Probably feels like she’s the only one capable of doing this work.@Dave: …Yeah@Dave: I should go apologize to them@You: Yeah, you should.@Dave: See ya later, slugger.@#You: No.. You had a fair point, what if it did go out of control?@Dave: I know it was it just..@You: Didn’t feel right?@Dave: Yeah..@You: We all have to make tough choices, in the end we’re left with what we have.@Dave: The dead cannot mourn for the living.@Dave: I should apologize to Camilla..@You:..@Dave: See you later, slugger.@",
            0, nullptr, 0);
        FactoryCreateStory(8, "The sun rises. A red light lingers unnaturally across the wasteland, like the world itself is breaking apart. The ground trembles as you stand, clutching your weapon.@Dave approaches, looking uneasy.@Dave: It’s time, did you get your items ready?@You: Yeah, is Camilla ready?@Dave: Yeah, give her a second, she’s still readying the device.@Out comes Camilla holding her weapon of mass destruction. Her assistants, now readily setting everything up@Camilla: Leyline destabillisation.. It’s starting. Get ready! The device still needs time to prepare, hold off the manifestation!@Camilla starts working together with her assistants, doing last minute checks as soon as possible.@The ground splits open. The corpses of monsters from previous nights merge together, twisting and writhing into a massive Abomination, an amalgamation of dogs’ heads, zombie torsos, skeletal wings, goblin claws, and a gargoyle’s armored face.@");
        FactoryCreateStory(8, "Camilia: It’s ready! Get back!@You bolted away from the battlefield, just in time to see the laser from the disruptor go off.@Directly hitting the abomination, it shrieked, its core struck. The monster, tried to tear out its own core, from whence it was struck, to no avail.@Spreading out, a crystallisation of the monster itself, It slowly lost its ability to move, ability to even cry out for help. The crystallisation spread into the ground, melting, separating into its parts constituent elements@All of the mana is sucked into an imaginary point, gathering up for a final release. Camellia stares in awe at the phenomenon@");
        FactoryCreateChoices(8, 15, 
            "Red Ruby Extraction Succesfull#Red Ruby Extraction Unsuccesfull",
            "Just before the implosion happened, Camilia ducked her head behind the rock, grabbing you with her under the cover.@All sound seemed to stop, halting. A large reverberation, felt in every soul. Scattering light shone, radiating everything in a flurry of magic release.@Camilia: Brace yourselves!@The stone visibly melted, , heating up as it turned from a cold grey, to red hot, then black, pink, green, blue. A glitch of the world@Then, it stopped. As if a plug on the world had been placed. Camilia waited for a while, as Dave held his breath.@She looked up, seeing the aftermath. The abomination was gone, eradicated. Nothing was left, as snow began to fall onto the area.@Dave: This didnt happen on our run of it..@Camilla: Is it safe?@You: No idea.@Camilla:...@#Having no one to pull her down, she stared as the implosion occurred. You pulled yourself behind cover as the rush of wild magic hit the surroundings.@It raged like a tempest, striking anything in its path. The abomination was replaced destruction in its pure form. Nothing left to waste, just nothingness.@There you stand, as the shock dies down. The Tempest calms into a wind, then none at all.@You stood up, looking across once was the battlefield. Replaced with something that can be described as in this world. A pure mana slurry, constituent mana flurry.@Camilla stood there, seemingly unaffected by the mana blast.@She fell.@Hitting the floor, like a puppeteer’s doll, strings cut. She laid there on the floor. You rushed over, along with Dave.@Laying her flat on the floor, Dave checked her heartbeat.@Dave: Still Breathing..@Dave let out a sigh, before her assistants usher her over, grabbing her and hauling her off to the carriages to heal.@You: Is she..@Dave: I don't know. It's.. too far to see.@Dave: I don’t know enough about magic to help her, only her assistants do.@You:..@Time passed, blended together. The job was done, what now?@Camilla woke up the next day, she seemed fine, but something was off.@Camilla: Hey.@You: What Happened?@Camilla: Got hit with the residual Mana blast.. My internal mana signature’s disrupted.@You: So..@Camilla: I’ll die.@You:..@Camilla: Not now though, we still have time. I want to at least give the world a positive impact before I go.@You:..@She stayed silent for a while@Camilla: Did we win?@Dave arrived to see the awakening of Camilla@Dave: Yeah.@",
            0, nullptr, 0);
        FactoryCreateStory(8, "Camilla: We actually did it.@She stayed silent for a while.@Camilla: Honestly, I didn’t expect to win there. All thanks to you that we won.@Dave: That was one hell of a fight. You gave em hell.@Dave gives you a fistbump@Camilla: Now that its gone, the mana culture of the world can now settle down.@Camilla: I.. would have to ask you though. Since there isn’t much left. Would you, like to come with me? Travel the world, and hopefully rebuilt civilisation. Let’s not repeat the mistakes of the old world.@Dave crosses his arms, looking at you with mixed concern and respect.@Dave:Choice is yours, bud. Ain’t no one gonna make it for ya.");
        FactoryCreateChoices(8, 16,
            "You accept Camilla's Offer.#You reject Camilla's Offer.",
            "Ending A: Rebuilder@You join Camilla and helped to start rebuilding the civilization.@Thus is the story you have chosen. The Last Soldier of the war became the first of a new era.@GAME END: Rebuilding#Ending B: The Wanderer@You step back, crossing your arms@John: That's not the life I want to lead. I appreciate the offer, but I will have to turn you down.@Camilla: Ah, well..: I suppose this is the end of our meeting then.@Dave: Last time we’ll meet ya, before we go to our next destination.@John: Suppose so.@Camilla: We’ll get going then. Will this be the last time I’ll meet you?@John: Let the fates decide on that.@Camilla: Fates..@Camilla: Hah, alright then.@Dave: Have a nice journey, wherever ya end up.@Camilla and Dave walk back to the caravan.@GAME END: Wandering",
            0, nullptr, 0);
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
                    /*entry.quest[choiceId];*/
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