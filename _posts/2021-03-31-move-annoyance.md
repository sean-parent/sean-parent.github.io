---
layout: post
title: Move Annoyance
date: 2021-03-31 21:16 +0000
---
\[Written for John Lakos and Vittorio Romeo. _Embracing Modern C++ Safely._ Boston: Addison-Wesley, forthcoming. Reproduced with permission.\]

## Annoyances

### Required Postconditions of a Moved-From Object Are Overly Strict

Given an object, `rv`, which has been moved from, the C++20[^cpp-requirements] Standard specifies the [required postconditions of a moved-from object](http://eel.is/c++draft/utility.arg.requirements#tab:cpp17.moveconstructible):

> `rv`'s state is unspecified <br>
  \[_Note_: `rv` must still meet the requirements of the library component that is using it. The operations listed in those requirements must work as specified whether `rv` has been moved from or not. &mdash; _end note_\] &mdash; Table 28, p. 488 C++20 Standard.

The requirement applies to both move construction and move assignment for types used with the standard containers and algorithms. The note is [not normative](https://www.iso.org/sites/directives/current/part2/index.xhtml#_idTextAnchor321) but does clarify that the requirements on a moved from object are not relaxed.

To understand how this requirement causes an issue in practice, consider the following simple class definition. The intent of `my_type` is to create a class that always holds a valid value, is _copyable_ and _equality comparable_, and happens to contain a remote part. The remote part in this example is held as a `unique_ptr` to an `implementation` object. A remote part might be employed to improve compile times by separating the implementation from the interface, to allow a polymorphic implementation using inheritance, or to trade off a slower copy for a faster move:

```cpp
class my_type {
    class implementation;
    std::unique_ptr<implementation> _remote;

public:
    explicit my_type(int a) : _remote{std::make_unique<implementation>(a)} {}

    my_type(const my_type& a) : _remote{std::make_unique<implementation>(*a._remote)} {}

    my_type& operator=(const my_type& a) {
        *_remote = *a._remote;
        return *this;
    }

    friend bool operator==(const my_type& a, const my_type& b) {
        return *a._remote == *b._remote;
    }
};
```

We can add the ability to move the object by using a default move-constructor and move-assignment operator:

```cpp
class my_type {
    //...
public:
    //...
    my_type(my_type&&) noexcept = default;
    my_type& operator=(my_type&&) noexcept = default;
```

If we ignore the library requirements and consider only the language requirements, this implementation is sufficient. The only language requirement is that a moved-from object is destructible because, without a cast, the only operation the compiler will perform on a moved-from object is to destruct it. By definition, an _rvalue_ is a temporary object, and no other operations will be performed. The assignment `a = f()` where `a` is of type `my_type` and `f()` returns a value of type `my_type`, will work correctly with the default member-wise implementations.

However, using `my_type` in a standard container or algorithm will likely fail. Consider inserting an element into a vector at a position, `p`:

```cpp
my_type a{42};
std::vector<my_type> v;
//...
v.insert(p, a); // undefined behavior
```

If `p` is not at the end of the vector, this code may move the range `[p, end(v))` and then copy `a` over a moved-from object. Implementations of the Standard Library may use a different approach to insert that would not encounter this issue.[^lib-implementation] The copy of `a` results in a statement with the effect of `*p = a` where `*p` is a moved-from instance of `my_type`. The copy is likely to crash because of the implementation of copy assignment:

```cpp
    my_type& operator=(const my_type& a) {
        *_remote = *a._remote;
        return *this;
    }
```

After the move `_remote` is equal to `nullptr` and dereferencing `_remote` is undefined behavior. There are multiple ways to fix copy-assignment, but for illustration we'll add a conditional to test `_remote` and if it is equal to `nullptr`, use an alternative implementation:

```cpp
    my_type& operator=(const my_type& a) {
        if (_remote == nullptr) {
            *this = my_type(a); // copy construct and move assign
        } else {
            *_remote = *a._remote;
        }
    }
```

The additional check is sufficient to make _all_ of the standard containers and algorithms work correctly. Unfortunately, this check is not sufficient to satisfy a strict reading of the [standard requirements](http://eel.is/c++draft/utility.requirements#utility.arg.requirements) because:

- Copy construction from a moved-from object will fail
- Copy assignment from a moved-from object will fail
- Equality will fail if either operand has been moved from

All of these operations would cause a `nullptr` to be dereferenced. The Standard Library states that these operations must be valid for _all_ values of a given type.

The implementations of functions associated with the containers and algorithms in the Standard Library will never perform any operation on a moved-from object other than to destruct or assign a new value to it _unless_ called with an object that has already been moved from (i.e., by the caller directly). The operations in the list above will never be invoked (one additional requirement imposed by `std::swap()` is discussed below).[^swap-problem]

Adding the additional checks to satisfy the Standard's wording has an otherwise unnecessary performance impact and proves to be error-prone to implement. Beyond that, the additional code introduces a new _empty_ state for `my_type`, which must be considered if we introduce an ordering with `operator<()` or any other operation the Standard may invoke. The gratuitously induced empty state defeats the purpose of value semantics because coding with an object that may or may not be empty is equivalent to coding with a pointer that may or may not be null.

The root cause of this issue is broader than just the postconditions of move operations. There is a standard proposal to address these issues.[^proposal] Until the proposal is adopted &mdash; and it may not be &mdash; a type must include these additional checks to adhere to the standard requirements.

[^cpp-requirements]:
    Similar wording with the same intent appears in every version of the C++ Standard since C++11.

[^lib-implementation]:
    The 11.0.1 version of the `libc++` Standard Library does use the described approach and will result in a crash.

[^swap-problem]:
    The `std::swap()` algorithm imposes one additional requirement. Consider swapping a value with itself; `std::swap(a, a)` will generate:

    ```cpp
    my_type tmp = std::move(a);
    a = std::move(a); // self-move-assignment of a moved-from object
    a = std::move(tmp);
    ```

    The statement `a = std::move(a)` is doing a self-move-assignment of a moved-from object. The default move-assignment in the above implementation of `my_type` will work correctly for self-move assignment of a moved-from object. The default implementation satisfies the postconditions for both the right-hand and left-hand arguments and does not affect the value of `a`. The left-hand argument of move assignment must be equal to the prior value of the right-hand argument. The containers and algorithms in the Standard Library do not self-swap objects, but `std::swap()` (annoyingly) provides the guarantee that self-swap will work if the arguments satisfy the [requirements for the move-constructible and move-assignable](http://eel.is/c++draft/utility.swap#2) concepts. The requirement for self-swap is both a legacy requirement from when `std::swap()` was implemented in terms of copy and follows from a general requirement in the Standard that, unless otherwise specified, operations should work even if reference arguments alias each other in whole or in part. There is no known value in supporting self swap, and a self swap usually indicates a defect.

[^proposal]:
    [_Relaxing Requirements of Moved-From Objects_, Sean Parent, P2345R0]({% post_url 2021-03-31-relaxing-requirements-of-moved-from-objects %})
