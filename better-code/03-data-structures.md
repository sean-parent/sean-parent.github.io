---
title: Data Structures
tagline: No Incidental Data Structures

layout: book-page
tags: [ better-code ]
---

### What is an _incidental_ data structure?

A typical definition for _data structure_ is:

> A _data structure_ is a format for organizing and storing data. {::comment}citation?{:/comment}

However, this definition doesn't define _structure_, instead it replaces it with the related word, _format_. In mathematics, _structure_ is defined as:

> A _structure_ on a set consists of additional entities that, in some manner, relate to the set, endowing the collection with meaning or significance. {::comment}citation?{:/comment}

{::comment}Some of this section may move into chapter[01]{:/comment}

As we saw in [chapter 1](https://broken-link) a type is a pattern for storing and modifying objects. In other words, a type is a structure that relates a set of objects to a set of values. This is a _representational_ relationship and it is this relationship that imbues the type with meaning. A representational relationship creates a _trivial data structure_ consisting of a single value.

Values are related to other values, for example, 3 is not equal to 4.

If two objects of the same type have the same representation then they represent the same value. Representational equality implies value equality. If the representation is unique then the converse is also true. A hash is a regular function on a representation or a value. Because it is regular, if two values are equal then the hash of the values are also equal.

Because objects exist in memory, they have a _physical_ relationship. The value at the first location in an array is located before the value in the second location. If we sort the values, we establish a correspondence between the physical and value relationships, i.e. an element before another element is less than or equal to that element. We can represent locations as values (pointers) and use those to represent additional relationships, such as "is a child of".

Given this, an improved definition of a data structure is:

> A _data structure_ is a structure utilizing value, representational, and physical relationship to encode semantic relationships on a collection of object.

The choice of encoding can make a dramatic difference on the performance of operations performed on the collection.

{::comment} Digression into memory hierarchy performance and utilizing relationships with algorithms - where should that go {:/comment}

Although data structures tend to be thought of simply in terms of containers such as arrays, lists, or maps, anytime a relationship is established between objects a data structure is created.

As an example, consider the problem of finding the `nth` to `mth` elements of an array as if the arra was in sorted order. The trivial way to do this is to simply sort the entire array and then print the `nth` to `mth` elements:

~~~ C++
template <typename I> // I models RandomAccessIterator
void sort_subrange(I f, I l, I sf, I sl) {
    sort(f, l);
}
~~~

{::comment}

use the sort a range of a container example.

{:/comment}
