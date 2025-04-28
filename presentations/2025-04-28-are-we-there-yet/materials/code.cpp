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

int binary_search(int x[], int n, int v) {
    int l = 0;
    int u = n - 1;

    while (true) {
        if (l > u) return -1;

        int m = (l + u) / 2;

        if (x[m] < v) l = m + 1;
        else if (x[m] == v) return m;
        else /* (x[m] > v) */ u = m - 1;
    }
}
