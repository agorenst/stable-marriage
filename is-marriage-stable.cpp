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

