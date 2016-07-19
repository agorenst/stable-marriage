#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

// For "woman_ranking_map"
#include "stable-marriage.h"

using namespace std;

bool check_stability(const vector<int>& preferences,
                     const vector<int> assignment,
                     const int n) {
    auto woman_ranks = woman_ranking_map(preferences, n);
    for (int i = 0; i < n; ++i) {
        auto suitor_preferences = begin(preferences) + (n*i);
        for (auto pw = suitor_preferences; pw != suitor_preferences+n; ++pw) {
            // For every woman the suitor prefers over his current match
            if (assignment[*pw] == i) { break; }
            auto woman = *pw;
            auto woman_rank = begin(woman_ranks) + (n*woman);
            // If that woman prefers the suitor over her current match, that's
            // an instability.
            if (woman_rank[assignment[*pw]] > woman_rank[i]) {
                return false;
            }
        }
    }
    return true;
}

using namespace std;

// Simply reads in from stdin.
int main(int argc, char* argv[]) {
    int n;
    cin >> n;

    auto preferences = read_preferences(n);
    vector<int> matching(n);
    for (int i = 0; i < n; ++i) { matching[i] = i; }
    do {
        vector<int> marriage(n);
        for (int i = 0; i < n; i++) {
            marriage[i] = matching[i];
        }

        cout << "Trying: ";
        for (int i = 0; i < n; ++i) {
            cout << "(" << i+1 << " " << marriage[i]+1 << ") ";
        }
        if (check_stability(preferences, marriage, n)) {
            cout << "Stable" << endl;
        }
        else {
            cout << "Nope" << endl;
        }
    } while(next_permutation(begin(matching), end(matching)));
}
