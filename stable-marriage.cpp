#include <algorithm>
#include <vector>
#include <string>
#include <iostream>
#include <list>
#include <map>
#include <stack>
#include <cassert>

using namespace std;

void inline set_preferences(vector<int>& v, int n, int person, int index, int pref) {
    v[(person*n)+index] = pref;
}



int main(int argc, char* argv[]) {
    int numtests;
    cin >> numtests;
    //cout << "Test count: " << numtests << endl;

    for (int i = 0; i < numtests; ++i) {
        int n;
        cin >> n;
        //cout << "Size of this test: " << n << endl;
        vector<int> preferences(2*n*n);
        vector<int> assignment(2*n,-1);

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
                if (j < n) { suitor += n; }
                preferences[(j*n)+k] = suitor;
            }
        }

        // TODO add in a check to make sure the preferences make sense.
        for (int j = 0; j < 2*n; ++j) {
            cout << j << " ";
            for (int k = 0; k < n; ++k) {
                cout << preferences[n*j+k] << " ";
            }
            cout << endl;
        }

        // Do the stable matching!
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
            //cout << "Length of man's preferences: " << distance(mans_pref_list_begin, mans_pref_list_end) << endl;
            auto witer = find_if(mans_pref_list_begin, mans_pref_list_end, [](int v) { 
                //cout << "Current index: " << v << endl;
                return v != -1; });
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

        //cout << "Result: " << endl;
        for (int j = 0; j < n; ++j) {
            cout << j << " " << assignment[j] << endl;
            //cout << j+1 << " " << (assignment[j]+1)-n << endl;
        }
    }
}
