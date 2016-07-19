#include <algorithm>
#include <vector>
#include <iostream>
#include <cassert>

#include "stable_marriage.h"

using namespace std;

// The real focus of this repository is in stable_marriage.cpp
// This file simply provides a "main" function to be able to read
// in a set of preferences, and output the stable marriage.

// Simply reads in from stdin.
int main(int argc, char* argv[]) {
    int numtests;
    cin >> numtests;

    for (int i = 0; i < numtests; ++i) {
        int n;
        cin >> n;

        auto preferences = read_preferences(n);
        auto assignment = stable_marriage(preferences, n);

        // We output the woman, and the man she's assigned to.
        for (int j = 0; j < n; ++j) {
            cout << j+1 << " " << assignment[j]+1 << endl;
        }
    }
}
