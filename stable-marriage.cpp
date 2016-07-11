#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

// This file provides a straightforward implementation of the "stable marriage"
// algorithm. It's literally a textbook algorithm problem (see T&K). The goal
// is, given a list of n "men" and n "women", provide a list of pairs (i,j)
// where i is a man and j is a woman, 

using namespace std;

// This is the main event!
// Performance consideration: we better be returning the assignment by move-
// semantics.
// This is based off of the classic algorithm, though I'm a bit fuzzy on the
// details.
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


// Pretty hacky, in the sense that we're reading in from stdin.
vector<int> read_preferences(int n) {
    vector<int> preferences(2*n*n);

    // read in all the preferences
    for (int j = 0; j < n*2; ++j) {
        int p;
        cin >> p;
        //cout << "Read in person: " << p << endl;
        assert(p-1 == j%n);
        for (int k = 0; k < n; ++k) {
            int suitor;
            cin >> suitor;
            //cout << "\tRead in suitor: " << suitor << endl;
            --suitor; // to avoid indexing issues.

            // here we scale to play better with our labeling model.
            if (j < n) { suitor += n; }
            preferences[(j*n)+k] = suitor;
        }
    }
    // we really should be smart about move construction here.
    return preferences;
}

// Simply reads in from stdin.
int main(int argc, char* argv[]) {
    int numtests;
    cin >> numtests;

    for (int i = 0; i < numtests; ++i) {
        int n;
        cin >> n;

        // Do the stable matching!
        auto preferences = read_preferences(n);
        //auto preferences_copy{preferences};
        auto assignment = compute_stable_marriage(preferences, n);

        //cout << "Result: " << endl;
        for (int j = 0; j < n; ++j) {
            cout << j << " " << assignment[j] << endl;
            //cout << j+1 << " " << (assignment[j]+1)-n << endl;
        }
    }
}
