#include "StoryEntry.h"

StoryEntry::StoryEntry(int w, const std::string& t):wave(w), text(t), choice(0), quest(0)
{

}

StoryEntry::StoryEntry(int w, int c, const std::vector<std::string>& ch, const std::vector<std::string>& r, int q) : wave(w), choice(c), choicetext(ch), results(r), quest(q)
{

}