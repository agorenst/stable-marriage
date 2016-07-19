#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

/*
This is a modern C++ implementation of the "perfect matching" algorithm
as explained by Kleinberg and Tardos. The key design approach is to
ensure that every iteration of the main loop runs in constant time. We
do this by maintaining several helper functions. 

The input format is in line with http://www.spoj.com/problems/STABLEMP/.
*/

using namespace std;


// Really, we're inverting permutation matrices here.
// The only caveat is that "preferences" is a 2n x n matrix,
// so we look at the second half.
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



// This is the main algorithm, see page 42, section 2.3.
// We do a slight variation (vectors only, no LL), but basically follow
// their approach.
vector<int> stable_marriage(const vector<int>& preferences, const int n) {
    // We preprocess the preference list so that it's 
    // constant time to determine if a woman prefers one suitor
    // over the other.
    auto woman_preferences = woman_ranking_map(preferences, n);

    // This is the ultimately product, where assignment[w] = m
    // means that woman w is married to man m.
    vector<int> assignment(n,-1);

    // Set up our worklist, of men still needing to be married.
    // We use an array so that we only allocate memory once.
    vector<int> worklist(n);
    for (int i = 0; i < n; ++i) { worklist[i] = i; }

    // Set up who's next for each man to ask.
    // They each start out wanting to ask their top choice, of course.
    vector<int> men_next(n,0);


    while (worklist.size()) {
        // Get the current suitor
        auto suitor = worklist[worklist.size()-1];
        worklist.pop_back();

        // Determine the next woman our suitor will ask.
        // This pattern of iterator + n*offset is to emulate our vector
        // being a matrix.
        auto suitor_preferences = begin(preferences) + (n*suitor);
        auto woman = suitor_preferences[men_next[suitor]++];

        // if she's unattached, easy.
        if (assignment[woman] == -1) {
            assignment[woman] = suitor;
        }
        else {
            // otherwise, she chooses the man she prefers
            auto woman_pref = begin(woman_preferences) + (n*woman);
            if (woman_pref[assignment[woman]] > woman_pref[suitor]) {
                worklist.push_back(assignment[woman]);
                assignment[woman] = suitor;
            }
            else {
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
