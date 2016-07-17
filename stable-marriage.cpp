#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>


using namespace std;

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

    // correctness:
    for (auto&& x : woman_ranking) { assert(x != -1); }
    // moreover, every row should have exactly a [0,n) set of indices.
    return woman_ranking;
}


// The input format for "preferences" is:
// {
// These first 4 rows are the preference lists for men 0...3
//  3 2 0 1
//  1 0 2 3
//  0 2 3 1
//  3 2 0 1
// These last 4 rows are the preference lists for women 0...3
//  2 1 3 0
//  1 2 0 3
//  2 0 1 3
//  2 1 3 0
// }
// This allows for a lot of convenience as we can treat men and women
// as indices into arrays, so that data[x] can have information about man
// or woman x.
vector<int> stable_marriage(const vector<int>& preferences, const int n) {
    assert(n > 0);
    assert(preferences.size() == n*n*2);

    auto woman_ranks = woman_ranking_map(preferences, n);

    // Set up the list of men to process
    vector<int> men_to_process(n);
    for (int i = 0; i < n; ++i) { men_to_process[i] = i; }

    // Set up who's next for each man to ask.
    vector<int> men_next(n,0);

    vector<int> assignment(n,-1);

    while (men_to_process.size()) {
        auto suitor = men_to_process[men_to_process.size()-1];
        men_to_process.pop_back();

        auto suitor_preferences = begin(preferences) + (n*suitor);
        auto woman = suitor_preferences[men_next[suitor]++];

        if (assignment[woman] == -1) {
            assignment[woman] = suitor;
        }
        else {
            auto woman_rank = begin(woman_ranks) + (n*woman);
            if (woman_rank[assignment[woman]] > woman_rank[suitor]) {
                men_to_process.push_back(assignment[woman]);
                assignment[woman] = suitor;
            }
            else {
                men_to_process.push_back(suitor);
            }
        }
    }

    // check for stability:
    for (int i = 0; i < n; ++i) {
        auto suitor_preferences = begin(preferences) + (n*i);
        for (auto pw = suitor_preferences; pw != suitor_preferences+n; ++pw) {
            if (assignment[*pw] == i) { break; }
            auto woman = *pw;
            auto woman_rank = begin(woman_ranks) + (n*woman);
            if (woman_rank[assignment[*pw]] > woman_rank[i]) {
                assert(0);
            }
        }
    }
    return assignment;
}

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
        //cout << p-1 << " " << j << " " << (j%n) << endl;
        assert(p-1 == j%n);
        for (int k = 0; k < n; ++k) {
            int suitor;
            cin >> suitor;
            //cout << "\tRead in suitor: " << suitor << endl;
            --suitor; // to avoid indexing issues.

            // here we scale to play better with our labeling model.
            //if (j < n) { suitor += n; }
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

        //// Do the stable matching!
        //auto preferences = read_preferences(n);
        ////auto preferences_copy{preferences};
        //auto assignment = compute_stable_marriage(preferences, n);

        auto preferences = read_preferences(n);
        //for (int i = 0; i < 2*n; ++i) {
        //    cout << i << ": ";
        //    for (int j = 0; j < n; ++j) {
        //        cout << preferences[i*n+j] << " ";
        //    }
        //    cout << endl;
        //}
        auto assignment = stable_marriage(preferences, n);

        //cout << "Result: " << endl;
        for (int j = 0; j < n; ++j) {
            cout << j+1 << " " << assignment[j]+1 << endl;
            //cout << j+1 << " " << (assignment[j]+1)-n << endl;
        }
    }
}
