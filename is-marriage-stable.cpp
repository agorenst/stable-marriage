
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
