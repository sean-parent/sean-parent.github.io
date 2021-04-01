---
layout: post
title: Concepts, Guarantees, & Domains (Oh My!)
---

Compare the description for the old [SGI STL `find()` algorithm](http://eel.is/c++draft/alg.find), to the [specification for `std::find()` in C++20](http://eel.is/c++draft/alg.find).

SGI STL defined `find()` as requiring that the value type, "is a model of _EqualityComparable_." And the concept _EqualityComparable_ contained the precondition that ["x and y are in the domain of `==`"](http://www.martinbroadhurst.com/stl/EqualityComparable.html).

The definition of domain for `==` still exists in the C++ standard but the use is limited to [_input iterators_](http://eel.is/c++draft/iterator.cpp17#input.iterators-2) and the [requirements for _sentinel_](https://eel.is/c++draft/iterator.concept.sentinel#3).

C++20 does not require the value type in `std::find()` is _EqualityComparable_ but instead requires `operator==()` exist, that it is callable with the value type, and that it returns an item convertible to bool. It no longer makes reference to an _EqualityComparable_ concept. To illustrate the distinction; in the SGI STL the following invocation of `find()` violates the preconditions and is undefined behavior:

```cpp
double a[]{1.0, nan(""), 3.0};
double* p = find(begin(a), end(a), nan(""));
```

The reason is that `nan` is outside the domain of operation for `operator==()` when used with `find()` because it doesn't satisfy the requirements of _EqualityComparible_.

By contrast, in C++20, the above code is well defined, but it won't actually find the `nan` value.

The weakening of the requirements in the C++ Standard allows for cases like the following:

```cpp
double a[]{1.0, nan(""), 3.0};
double* p = find(begin(a), end(a), 3.0);
```

With the SGI STL specification this is still undefined behavior, but in C++20 this is well defined and will result with `*p == 3.0`.

Concepts, which were created to define the semantics of algorithms and containers, only rarely appear in the current specification for algorithms and containers, and then mostly for iterators. This leaves the algorithms devoid of semantics. i.e. The above `find()` example doesn't actually _find_ anything. The concept requirements and preconditions were created




## History

The C++ Standard Library requirements and concepts are missing a general precondition that reachable values from the arguments must be within the operations domain. Instead, the requirements are stated as holding for all values of the type. The missing precondition has undesired effects, including imposing unnecessary restrictions on moved-from objects and reducing the usefulness of concepts.

Compare the description for the old [SGI STL LessThanComparable concept](http://www.martinbroadhurst.com/stl/LessThanComparable.html) versus the [C++17 concept](http://eel.is/c++draft/utility.arg.requirements#tab:cpp17.lessthancomparable).



The Standard no longer relies heavily on named requirements. For example, [`std::find()` in the SGI STL required that `*i` satisify _EqualityComparable_](http://www.martinbroadhurst.com/stl/find.html), but the current Standard only requires that `*i == value` is comparable to `true`.
