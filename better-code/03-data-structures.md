---
title: Data Structures
tagline: No Incidental Data Structures
layout: book-page
tags: [ better-code ]
chapter: 3
---

### What is an _incidental_ data structure?

A typical definition for _data structure_ is:

> A _data structure_ is a format for organizing and storing data. {::comment}citation?{:/comment}

However, this definition doesn't define _structure_, instead it replaces it with the related word, _format_. In mathematics, _structure_ is defined as:

> A _structure_ on a set consists of additional entities that, in some manner, relate to the set, endowing the collection with meaning or significance. {::comment}citation?{:/comment}

{::comment}Some of this section may move into chapter[01]{:/comment}

As we saw in [chapter 1](01-types.html) a type is a pattern for storing and modifying objects. That is, a type is a structure that relates a set of objects to a set of values. This is a _representational_ relationship and it is this relationship that imbues the type with meaning. A representational relationship creates a _trivial data structure_ consisting of a single value.

Values are related to other values, for example, 3 is not equal to 4.

If two objects of the same type have the same representation then they represent the same value. Representational equality implies value equality. If the representation is unique then the converse is also true. A hash is a regular function on a representation or a value. Because it is regular, if two values are equal then the hash of the values are also equal.

Because objects exist in memory, they have a _physical_ relationship. The value at the first location in an array is located before the value in the second location. If we sort the values, we establish a correspondence between the physical and value relationships, i.e. an element before another element is less than or equal to that element. We can represent locations as values (pointers) and use those to represent additional relationships, such as "is a child of".

Given this, an improved definition of a data structure is:

> A _data structure_ is a structure utilizing value, representational, and physical relationships to encode semantic relationships on a collection of objects.

The choice of encoding can make a dramatic difference on the performance of operations performed on the collection.

{::comment} Digression into memory hierarchy performance and utilizing relationships with algorithms - where should that go {:/comment}

Although data structures tend to be thought of simply in terms of containers such as arrays, lists, or maps, anytime a relationship is established between objects a data structure is created. However, to avoid confusion we will reserve the term _data structure_ to refer to types with a set of invariants which insure a set of relationships are maintained. More transient data structures will be referred to as _structured data_.

As an example of utilizing structured data_, consider the problem of finding the `nth` to `mth` elements of an array as if the array was in sorted order. The trivial way to do this is to simply sort the entire array and then print the `nth` to `mth` elements. In this example `[sf, sl)` is a subrange of `[f, l)`. {::comment}appendix to describe half open notation?{:/comment}

{% include code.md name='sort-subrange-0' caption='inefficient sort subrange' %}

{::comment} Should this section start with partial_sort then add nth_element instead of the other way around? {:/comment}

This function, however, does more work than is necessary. There is a function in the standard library, `nth_element()` which given a position `nth` within a range `[f, l)` has the post condition that the element at `nth` is the same element that would be in that position if `[f, l)` were sorted. `nth_element()` is a special case of sort_subrange when the subrange is of length 1 (or 0 if `nth == l`).

This would not be of much use to build `sort_subrang()` except that `nth_element()` has an additional post condition. The range `[f, l)` is partitioned such that all elements prior to `nth` are less than or equal to the final element at `nth`. This post condition leaves us with _structured data_ and we can take advantage of that structure. If we find the `nth_element()` where `nth` is `sf` then we only need to sort the remaining elements to `sl` which can be done with `partial_sort()`.

{% include code.md name='sort-subrange-1' caption='improved sort subrange' %}

We can improve this function slightly by noting that in the case where `sf == f` we do not need to call `nth_element()`.

```cpp
template <typename I> // I models RandomAccessIterator
void sort_subrange(I f, I l, I sf, I sl) {
    if (sf == sl) return;
    if (sf != f) {
        std::nth_element(f, sf, l);
        ++sf;
    }
    std::partial_sort(sf, sl, l);
}
```

{::comment} add exercise to test performance? {:/comment}

#### Composite Objects and the Whole-Part relationship

A _composite object_ is consists of other objects, called _parts_. These parts are related to the _composite object_ via a _whole-part_ relationship.

> A _whole-part_ relationship is _connected_, _non-circular_, _logically disjoint_, and _owned_. _Connected_ means any part is reachable from the object's starting address. _Non-circular_ means that no part is a part of itself. _Logically disjoint_ means that if two objects share a subpart where modifications to the subpart affect the value of both objects, then one of the objects must be a subpart of the other. _Owned_ means that copying the object copies its parts, and destroying the object destroys its parts. [^eop]

[^eop]:
    Alexander Stepanov & Paul McJones. _Elements of Programming_ (Addison-Wesley, 2009), 12.1
