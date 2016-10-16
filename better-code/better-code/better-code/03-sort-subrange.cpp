//
//  main.cpp
//  better-code
//
//  Created by Sean Parent on 10/3/16.
//  Copyright © 2016 Sean Parent. All rights reserved.
//

#include <algorithm>
#include <iterator>
#include <vector>
#include <numeric>

#include "../../code/sort-subrange-0.cpp"

using namespace std;

int test_03() {
    vector<int> a(1024*1024*1024);
    iota(begin(a), end(a), 0);
    random_shuffle(begin(a), end(a));
    sort_subrange_0(begin(a), end(a), begin(a) + 3, end(a) + 5);
    return 0;
}
