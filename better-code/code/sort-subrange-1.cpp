template <typename I> // I models RandomAccessIterator
void sort_subrange_1(I f, I l, I sf, I sl) {
    std::nth_element(f, sf, l); // partitions [f, l) at sf
    if (sf != l) {
        ++sf;
        std::partial_sort(sf, sl, l);
    }
}
