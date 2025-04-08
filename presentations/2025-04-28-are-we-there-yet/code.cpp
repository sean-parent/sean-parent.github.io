int* lower_bound(int* first, int* last, int x) {
    while (first != last) {
        int* middle = first + (last - first) / 2;
        if (*middle < x)
            first = middle + 1;
        else
            last = middle;
    }
    return first;
}

int* binary_search_insert_position(int* first, int* last, int x) {
    while (first < last) {
        int* mid = first + (last - first) / 2;
        if (*mid < x) {
            first = mid + 1;
        } else {
            last = mid;
        }
    }
    return first;
}
