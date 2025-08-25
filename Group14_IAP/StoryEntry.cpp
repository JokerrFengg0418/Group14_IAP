#include "StoryEntry.h"

StoryEntry::StoryEntry(int w, const std::string& t)
    : wave(w), text(t), choice(0), quest(0) {
}

StoryEntry::StoryEntry(int w, int choiceId, const std::vector<std::string>& c,
    const std::vector<std::string>& r, int q,
    const std::vector<int>& next)   // ❌ no default here
    : wave(w), choice(choiceId), choicetext(c), results(r), quest(q), nextChoices(next) {
}

StoryEntry::StoryEntry(int w, int choiceId, const std::vector<std::string>& c,
    const std::vector<std::string>& r, int q,
    const int* nextArr, size_t nextSize)
    : wave(w), choice(choiceId), choicetext(c), results(r), quest(q),
    nextChoices(nextArr, nextArr + nextSize) // convert array to vector
{
}
