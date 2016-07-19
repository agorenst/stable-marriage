#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

#include "stable_marriage.h"

//***************************************************************************//
// The main event! This contains the implementation of how to compute a stable
// marriage. It also contains the definition of a few helper functions, and
// an alternative implementation that is in some ways "simpler".
// The implementation is based off of Kleinberg and Tardos, page 42 (section
// 2.3). There is always a balance between a sophisticated implementation,
// and an implementation that can be read by others. I hope this strikes a
// good balance for students. Definitely email me at <mygithubname>@outlook.com
// if you have suggestions or questions.
//***************************************************************************//

using namespace std;

// The main algorithm.
vector<int> stable_marriage(const vector<int>& preferences, const int n) {
    // We preprocess the preference list so that it's 
    // constant time to determine if a woman prefers one suitor
    // over the other.
    auto woman_preferences = woman_ranking_map(preferences, n);

    // This is the ultimate product, where assignment[w] = m
    // means that woman w is married to suitor m. We use -1 to indicate
    // unattached.
    vector<int> assignment(n,-1);

    // Set up our worklist, of suitors still needing to be married.
    // We use an array so that we only allocate memory once.
    vector<int> worklist(n);
    for (int i = 0; i < n; ++i) { worklist[i] = i; }

    // Set up who's next for each suitor to ask.
    // They each start out wanting to ask their top choice.
    vector<int> men_next(n,0);

    // While there is an unattached suitor.
    while (worklist.size()) {
        // Get the current suitor
        auto suitor = worklist[worklist.size()-1];
        worklist.pop_back();

        // Determine the next woman our suitor will ask.
        // This pattern of iterator + n*offset is to emulate our vector
        // being a matrix.
        auto suitor_preferences = begin(preferences) + (n*suitor);
        auto woman = suitor_preferences[men_next[suitor]];
        men_next[suitor]++;

        // if she's unattached, easy.
        if (assignment[woman] == -1) {
            assignment[woman] = suitor;
        }
        else {
            // otherwise, she chooses the man she prefers
            auto woman_pref = begin(woman_preferences) + (n*woman);
            // The woman ranks the suitor sooner (i.e., better) than
            // her current fiance.
            if (woman_pref[assignment[woman]] > woman_pref[suitor]) {
                worklist.push_back(assignment[woman]);
                assignment[woman] = suitor;
            }
            else {
                // this should never allocate memory!
                worklist.push_back(suitor);
            }
        }
    }
    return assignment;
}

// This was my first implementation, where I was very much against
// any extra memory allocations. This requires a slightly different
// input format, where the women are labelled [n,n*2) rather than [0,n).
vector<int> compute_stable_marriage(vector<int>& preferences, const int n) {
    assert(n > 0);
    assert(preferences.size() == n*n*2);

    vector<int> assignment(n,-1);

    for (;;) {
        // this extracts the index of the first unattached man.
        auto miter = find(begin(assignment), end(assignment), -1);
        if (miter == begin(assignment)+n) { break; } // we're done!

        // we're equating the index of the man's assignment with the man's
        // label. I.e., men are labeled 0..n-1 in the preferences vector.
        auto m = distance(begin(assignment), miter);

        // extract that man's preference list
        auto mans_pref_list_begin = begin(preferences)+(m*n);
        auto mans_pref_list_end = begin(preferences)+(m*n+n);
        // find the first woman not yet "checked off"
        auto witer = find_if(mans_pref_list_begin, mans_pref_list_end,
                            [](int v) { return v != -1; });
        assert(witer != mans_pref_list_end);
        auto w = *witer;
        *witer = -1; // we are now considering this woman, so remove from prefs


        // This is the canonical question: should we cache the results
        // in a larger vector (easy to "save" the woman's latest fiance)
        // but would that actually make things faster than finding the man
        // who says that he's engaged to a woman? /insert joke here.

        // ptr to rival
        auto cmiter = find(begin(assignment), end(assignment), w);
        // who is our rival?
        auto cm = distance(begin(assignment), cmiter);

        // Case where woman has never been proposed to.
        if (cm == n) {
            assignment[m] = w;
        }
        else {
            auto woman_pref_begin = begin(preferences)+(w*n);
            auto woman_pref_end = begin(preferences)+(w*n+n);
            auto firstFound = find_if(woman_pref_begin, woman_pref_end,
                                [cm,m](int v) { return v == cm || v == m; });
            if (*firstFound == m) {
                assignment[m] = w;
                assignment[cm] = -1;
            }
        }
    }
    // Note: will we actually be smart about move construction?
    return assignment;
}

// The second half of the preferences matrix is an n x n matrix W
// where W[i,j] is woman i's j-th choice of suitor. So W[i,0] is her
// most preferred suitor, W[i,1] is her next, and so on. This is
// just like the men's preference ranking as well.
//
// This function produces a W[i,m] = j matrix, where given a woman i
// and a suitor m, we get the rank woman i gives suitor m.
// This makes it much easier for a woman to see if she prefers m1 or m2:
// she prefers m1 exactly when W[i,m1] < W[i,m2] (e.g., she may prefer
// m1 as her 2nd choice, so W[i,m1] == 2, and m2 is her 5th, so W[i,m2] == 5.
vector<int> woman_ranking_map(const vector<int>& preferences, const int n) {
    vector<int> woman_ranking(n*n,-1);

    auto women_prefs_begin = begin(preferences)+(n*n);
    auto women_ranks_begin = begin(woman_ranking);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            // the j-th most preferred man by woman i:
            women_ranks_begin[women_prefs_begin[j]] = j;
        }
        women_prefs_begin += n;
        women_ranks_begin += n;
    }

    return woman_ranking;
}


// Read in our input
// Pretty hacky, in the sense that we're reading in from stdin.
vector<int> read_preferences(int n, bool bigoffset) {
    vector<int> preferences(2*n*n);

    // read in all the preferences
    for (int j = 0; j < n*2; ++j) {
        int p;
        cin >> p;
        //cout << "Read in person: " << p << endl;
        //cout << p-1 << " " << j << " " << (j%n) << endl;
        assert(p-1 == j%n);
        for (int k = 0; k < n; ++k) {
            int suitor;
            cin >> suitor;
            --suitor; // to avoid indexing issues.

            if (bigoffset && j < n) { suitor += n; }
            preferences[(j*n)+k] = suitor;
        }
    }
    // we really should be smart about move construction here.
    return preferences;
}
