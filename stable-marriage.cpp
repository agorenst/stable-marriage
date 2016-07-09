#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <cassert>

// This file provides a straightforward implementation of the "stable marriage"
// algorithm. It's literally a textbook algorithm problem (see T&K). The goal
// is, given a list of n "men" and n "women", provide a list of pairs (i,j)
// where i is a man and j is a woman, 




using namespace std;

void print_preferences(const vector<int>& preferences, const int n) {
    for (int j = 0; j < 2*n; ++j) {
        cout << j << " ";
        for (int k = 0; k < n; ++k) {
            cout << preferences[n*j+k] << " ";
        }
        cout << endl;
    }
}

vector<int>::const_iterator find_match_iter(const vector<int>& preferences, int n, int p, int s) {
    auto pref_list_begin = begin(preferences)+(p*n);
    auto pref_list_end = begin(preferences)+(p*n+n);
    auto match_pref = find(pref_list_begin, pref_list_end, s);
    assert(match_pref < pref_list_end);
    return match_pref;
}

void check_stability(const vector<int>& preferences, const vector<int>& assignment, const int n) {
    assert(n*2 == assignment.size());
    assert(n*2+n == preferences.size());

    // make sure it's a valid marriage
    for (int m = 0; m < n; ++m) {
        int w = assignment[m];
        assert(assignment[w] == m);
    }

    for (int m = 0; m < n; ++m) {
        int w = assignment[m];
        auto woman_match_pref = find_match_iter(preferences, n, w, m);
        auto man_match_pref = find_match_iter(preferences, n, m, w);

        // now we want to see that there's no other married pair that would like to swap.

        for (int m2 = m+1; m2 < n; ++m2) {
            int w2 = assignment[m2];
            auto wmp2 = find_match_iter(preferences, n, w, m);
            auto mmp2 = find_match_iter(preferences, n, m, w);

            // so, it should be that at least someone won't agree with a spouse-swap. Colorful!
            // TODO put clever comparison here.
        }

    }

}

// This is the main event!
// Performance consideration: we better be returning the assignment by move-
// semantics.
// This is based off of the classic algorithm, though I'm a bit fuzzy on the
// details.
vector<int> compute_stable_marriage(vector<int>& preferences, const int n) {
    assert(n > 0);
    assert(preferences.size() == n*n*2);

    vector<int> assignment(n*2,-1);

    // We're going to constantly iterate over the men's current marriage
    // status, which is the first half of the "assignment" vector.
    // We maintain an explicit storage of the woman's fiance because
    // it makes it very easy to find the "currentSuitor". Maybe there's
    // a better way.
    auto assignBegin = begin(assignment);
    auto assignEnd = begin(assignment)+n;
    for (;;) {
        // this extracts the index of the first unattached man.
        auto miter = find(begin(assignment), begin(assignment)+n, -1);
        if (miter == begin(assignment)+n) { break; } // we're done!
        // we trust that the men are listed first, so we hack up
        // their the index from the iterator from subtraction
        auto m = distance(begin(assignment), miter);
        //cout << "Considering man at index " << m << endl;

        // find the woman we're going to propose to
        auto mans_pref_list_begin = begin(preferences)+(m*n);
        auto mans_pref_list_end = begin(preferences)+(m*n+n);
        auto witer = find_if(mans_pref_list_begin, mans_pref_list_end,
                            [](int v) { return v != -1; });
        assert(witer != mans_pref_list_end);
        auto w = int{*witer};
        *witer = -1; // we have considered this woman, so we never consider again.


        auto currentSuitor = int{assignment[w]};
        if (currentSuitor == -1) {
            assignment[w] = m;
            assignment[m] = w;
        }
        else {
            auto woman_pref_list_begin = begin(preferences)+(w*n);
            auto woman_pref_list_end = begin(preferences)+(w*n+n);
            auto curDist = find(woman_pref_list_begin, woman_pref_list_end, currentSuitor);
            auto newDist = find(woman_pref_list_begin, woman_pref_list_end, m);
            if (curDist > newDist) {
                assignment[w] = m;
                assignment[m] = w;
                assignment[currentSuitor] = -1;
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
    //cout << "Test count: " << numtests << endl;

    for (int i = 0; i < numtests; ++i) {
        int n;
        cin >> n;
        //cout << "Size of this test: " << n << endl;

        // TODO add in a check to make sure the preferences make sense.

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
