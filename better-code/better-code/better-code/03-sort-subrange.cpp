//
//  main.cpp
//  better-code
//
//  Created by Sean Parent on 10/3/16.
//  Copyright © 2016 Sean Parent. All rights reserved.
//

#include <algorithm>
#include <chrono>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

#include "../../code/sort-subrange-0.cpp"

using namespace std;

template <typename I> // I models RandomAccessIterator
void shuffle(I f, I l) {
    thread_local std::default_random_engine engine;
    shuffle(f, l, engine);
}

int test_03() {
    vector<int> a(1024 * 1024);
    iota(begin(a), end(a), 0);
    shuffle(begin(a), end(a));
    sort_subrange_0(begin(a), end(a), begin(a) + 3, end(a) + 5);
    return 0;
}
