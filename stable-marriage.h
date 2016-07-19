#include <vector>

std::vector<int> stable_marriage(const std::vector<int>& preferences, const int n);


// A utility that comes up a few times.
std::vector<int> woman_ranking_map(const std::vector<int>& preferences, const int n);

// Another utility.
std::vector<int> read_preferences(int n, bool bigoffset = false);
