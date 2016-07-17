#include <algorithm>
#include <iostream>
//#include <time>
#include <random>
#include <vector>

// This is a helper file designed to generate random instances
// of stable-marriage problems. These instances don't have any
// particular quality (such as uniqueness of solution, or
// whatever).
using namespace std;


// List comprehension...
vector<int> default_assignment(int n, int start_value) {
    // this initiail
    vector<int> v(n);
    for (auto&& x : v) {
        x = start_value++;
    }
    return v;
}


int main(int argc, char* argv[]) {
    int n;
    cin >> n;
    cout << n << endl;

    random_device rd;
    mt19937 g(rd());

    for (int i = 0; i < n; ++i) {
        cout << i+1 << " ";
        auto v = default_assignment(n,n);
        // TODO create a random shuffle?
        shuffle(begin(v), end(v), g);
        for (auto&& x : v) { cout << ((x+1)-n) << " "; }
        cout << endl;
    }

    for (int i = n; i < 2*n; ++i) {
        cout << ((i+1)-n) << " ";
        auto v = default_assignment(n,0);
        // TODO create a random shuffle?
        shuffle(begin(v), end(v), g);
        for (auto&& x : v) { cout << x+1 << " "; }
        cout << endl;
    }
}
