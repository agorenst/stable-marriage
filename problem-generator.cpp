#include <time>
#include <random>
#include <vector>

// This is a helper file designed to generate random instances
// of stable-marriage problems. These instances don't have any
// particular quality (such as uniqueness of solution, or
// whatever).

// List comprehension...
vector<int> default_assignment(int length, int start_value) {
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

    for (int i = 0; i < n; ++i) {
        cout << i << " ";
        auto v = default_assignment(n,n);
        // TODO create a random shuffle?
        for (auto&& x : v) { cout << v << " "; }
        cout << endl;
    }

    for (int i = n; i < 2*n; ++i) {
        cout << i << " "
        auto v = default_assignment(n,0);
        // TODO create a random shuffle?
        for (auto&& x : v) { cout << v << " "; }
        cout << endl;
    }
}
