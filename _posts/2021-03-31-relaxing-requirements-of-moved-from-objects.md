---
layout: post
title: Relaxing Requirements of Moved-From Objects
description: ''
mathjax: true
tags:
- standard
- move
date: 2021-03-31 21:16 +0000
---
<style>
.proposed {
  background: #eeffee;
  border-left: dashed;
  border-left-color: #00ff00;
}
.comment {
  background: #eeeeff;
  font-family: sans-serif;
  padding: 8px;
}
.strike {
  text-decoration: line-through;
}
</style>

| Document Number: | D2345R2 |
| Date: | 2021-04-14 |
| Reply-to: | Sean Parent, [sean.parent@stlab.cc](sean.parent@stlab.cc) |
| Audience: | LWG & LEWG |

### Table of Contents
{: .no_toc }

- This unordered seed list will be replaced by the TOC as an unordered list
{: toc }

## Introduction
{: .no_toc }

The C++ Standard Library requirements are overly restrictive regarding the state of a moved-from object. The strong requirements impose an unnecessary burden on implementers and imposes a performance impact of user-defined operations.

The issue was recognized in Geoffrey Romer's paper, [P2027R0](#p2027). The approach outlined here differs in the following ways:

- The requirements are (slightly) stronger to support `swap(a, a)`
- It avoids introducing a new object state, _partially formed_, (see [Future Implications](#future-implications))

This paper details the issue and presents some suggested wording to address it. Depending on the wording chosen, it may be possible to address the issue with a defect report retroactively.
{: .comment }

### Document Conventions

> This is the proposed wording for the standard. There may be more than one proposed variant for the same section.
{: .proposed }

> This is a quote from an existing document.

This is a comment or work in progress.
{: .comment }

This document discusses _requirements_ in multiple different contexts. The following terms are used when the meaning is otherwise ambiguous. The _Standard requirements_ refer to the current, C++20, documented requirements. _Implementation requirements_ refers to the actual requirements necessary to implement the library components. This may be weaker than the stated requirements. Finally there are the _proposed requirements_, this is the proposed wording to bring the _Standard requirements_ more inline with the _implementation requirements_.

## Motivation and Scope

Given an object, `rv`, which has been moved from, the C++20[^cpp-requirements] Standard specifies the [required postconditions of a moved-from object](http://eel.is/c++draft/utility.arg.requirements):

> `rv`'s state is unspecified <br>
  \[_Note_: `rv` must still meet the requirements of the library component that is using it. The operations listed in those requirements must work as specified whether `rv` has been moved from or not. &mdash; _end note_\] &mdash; Table 28, p. 488 C++20 Standard.

The Standard requirement applies to both [_Cpp17MoveConstructible_](http://eel.is/c++draft/utility.arg.requirements#tab:cpp17.moveconstructible) and [_Cpp17MoveAssignable_](http://eel.is/c++draft/utility.arg.requirements#tab:cpp17.moveassignable). The note is [not normative](https://www.iso.org/sites/directives/current/part2/index.xhtml#_idTextAnchor321) but does clarify that the requirements on a moved-from object are not relaxed.

In general, unless move is specified to make a copy, the Standard requirement is not achievable. For example, the [sorting algorithms require](https://eel.is/c++draft/alg.sorting.general) `comp(*i, *j)` induce a strict weak ordering. Therefore, a moved-from object must be ordered with respect to every other value in the sequence with an arbitrary user-supplied comparison function. Only a value within the initial sequence could satisfy that requirement.

No implementation of the Standard Library will ever invoke a comparison, user-defined or otherwise, on an object that a library component itself moved-from during the course of the same operation. Such a comparison would not have meaning.

The Standard has a slightly different requirement for the [`move_constructible` concept](https://eel.is/c++draft/concept.moveconstructible):

> If `T` is not const, `rv`'s resulting state is valid but unspecified ([\[lib.types.movedfrom\]](https://eel.is/c++draft/lib.types.movedfrom)); otherwise, it is unchanged.

And from the requirements for the [`assignable_from` concept](https://eel.is/c++draft/concept.assignable):

> If `rhs` is a non-const xvalue, the resulting state of the object to which it refers is valid but unspecified ([\[lib.types.movedfrom\]](https://eel.is/c++draft/lib.types.movedfrom)).

The term _valid_ in this context does not impose any actual requirement on the moved from object since there is no requirements about what operations must be available on a valid object. The referenced definition for _valid but unspecified_ refers to a general guarantee of library types, but is not a library requirement. The only restriction on the moved from object appears to be that it is _unspecified_, which is to say there is no requirement.

The Standard even notes that [assignment need not be a total function](https://eel.is/c++draft/concept.assignable#2).

 The way the Standard requirements for moved-from objects are frequently taught is that all operations used by Standard components must be _total_ when used with a moved-from object. i.e. `rv < a` must be valid and induce a strict weak ordering for _all possible values of `a`_. However, even such a strong guarantee does not solve the issue for arbitrary operations passed to the Standard Library.

Geoffrey Romer's paper, [P2027R0](#p2027) makes the observation that _valid but unspecified_ does not compose. The result is that any composite object requires additional code to move the object into a valid state on move.

Attempting to make all operations total with respect to moved-from objects imposes an unnecessary performance penalty and the implementation of such operations is error-prone. Examples and details are provided in an _Annoyance_ I wrote for the upcoming [_Embracing Modern C++ Safely_](#move-annoyance). An example directly from the standard library is detailed in the [Weaker Guarantees for Future Components] section.
{:#bl-move-annoyance}

In [the discussion](https://lists.isocpp.org/lib-ext/2020/01/14004.php) of [P2027R0](#p2027) there is a lot of confusion about the difference between requirements and guarantees in the Standard. In short, the standard library _requirements_ impose a set of syntactic and semantic requirements on operations on arguments (both types and values) passed to a standard component. i.e., Give `std::find(f, l, v)`; it is required that `f` and `l` denotes a [_valid range_](https://eel.is/c++draft/iterator.requirements#def:range,valid).

A _guarantee_ is provided by a standard component, and may be conditional on requirements of the components arguments. i.e., `std::vector<T>::operator==()` is (guaranteed to be) an equivalence relation iff `T` meets the `Cpp17EqualityComparable` [requirements](https://eel.is/c++draft/tab:container.req). The confusion in part comes from stating a guarantee as _satisfying_ a named requirement.

The standard guarantees regarding moved from objects are specified in [lib.types.movedfrom](https://eel.is/c++draft/lib.types.movedfrom), as being _valid but unspecified_. The wording proposed in this paper does not change the guarantees. I included some discussion in the [_Future Implications_][weaker-guarantees-for-future-components] section about why it may be desirable to revisit this terminology.

### Requirements of a Moved-From Object

All known standard library implementations only require the following operations on an object, `mf`, that the library moved from within an operation:

- `mf.~()` (The language also requires this for implicitly moved objects)
- `mf = a`
- `mf = move(a)`
- `mf = move(mf)`

The last implementation requirement comes from `std::swap()` when invoked as `swap(a, a)`. It is note worthy that self-move-assignment is only required in the case where the object has already been moved-from. Self-swap does appear in some older standard library implementations of `std::random_shuffle()`. This underscores the need to support self-swap by the standard requirements.

Supporting self-move-assignment for this narrow case imposes some additional complexity because `a = move(a)` is, in general, a contradiction and is not required by the implementation of any standard component. The implementation required postcondition of `a = move(b)` is that `a` holds the prior value of `b` and the value of `b` is unspecified, but may be guaranteed to be a specific value. For example, if `a` and `b` are both of type `my_unique_ptr<T>` with the guarantee that `a` will hold the prior value of `b`, and `b` will be equal to `nullptr`. Then for the expression `a = move(a)`, the only way both of those guarantees could be satisfied is if `a` is already equal to `nullptr`. The current standard avoids this contradiction by defining the postcondition of move assignment for `std::unique_ptr<T>` as equivalent to `reset(r.release())` which provides a stronger guarantee than any standard component implementation requires while satisfying the Standard requirements.

### Non-Requirements

There is not a standard requirement to provide guarantees across operations that result in moved-from objects. For example:

```cpp
T a[]{ v0, v1, v1, v2 };
(void)remove(begin(a), end(a), v1);
sort(begin(a), end(a));
```

After `remove()`, the last two objects at the end of `a` have unspecified values and may have been moved from. There is no requirement that these moved-from objects also satisfy the requirements of `sort()` by being in the domain of the operation `operator<()`, even if `v0`, `v1`, and `v2` are within the domain. The post conditions of `remove()` and the requirements of `sort()` are independent. An invocation of `sort()` for a particular type, `T`, may or may not be valid depending on the guarantees provided by `T`.

Assuming `v0` and `v2` are in the domain of `operator<()` for `sort()` the following is guaranteed:

```cpp
T a[]{ v0, v1, v1, v2 };
auto p = remove(begin(a), end(a), v1);
sort(begin(a), p);
```

As another example:

```cpp
for (std::string line; std::getline(std::cin, line);) {
  v.push_back(std::move(line));
}
```

For the call to `std::getline()` to be valid with a moved from string it requires that `std::string()` _guarantees_ that `std::erase()` is valid on a moved from string. Changing the requirements of a moved from object does not change the guarantees of the standard components.

## Impact on the Standard

 All components which are _Movable_ in the Standard Library currently satisfy the proposed requirements as stated by both options below. **Both options are non-breaking changes and relax the requirements.** With either option, it may be possible to adopt these options retroactively as part of addressing a defect since neither option is a breaking change.

Concern has been raised that changing the documentation for requirements, especially the named requirements and concepts, would break existing code documentation that referenced the standard. However, taking a strict view of this would mean that the standard documentation could not be changed. For example, one of the libraries I work on has a `task<>` template which is documented as being ["Similar to `std::function` except it is not copyable and supports move-only and mutable callable targets..."](https://stlab.cc/libraries/concurrency/task/task/). Of note, this goes on to specify, "`stlab::task` satisfies the requirements of [MoveConstructible](https://en.cppreference.com/w/cpp/named_req/MoveConstructible) and [MoveAssignable](https://en.cppreference.com/w/cpp/named_req/MoveAssignable)." Weakening the requirements would mean that statement is still true.

However, the concern over changing the documentation is one that should be considered in light of weakening the requirements retroactively as a defect. As that would mean even citing a specific version of the standard would break. 

## Technical Specifications

We need a general requirement regarding _the domain of an operation_. Borrowing from [the text for input iterators](http://eel.is/c++draft/iterator.cpp17#input.iterators-2):

> Unless otherwise specified, there is a general precondition for all operations that the requirements hold for values within the _domain of the operation_.
{: .proposed}

> The term _domain of the operation_ is used in the ordinary mathematical sense to denote the set of values over which an operation is (required to be) defined. This set can change over time. Each component may place additional requirements on the domain of an operation. These requirements can be inferred from the uses that a component makes of the operation and is generally constrained to those values accessible through the operation's arguments.
{: .proposed}

The above wording should appear in the [Requirements section of the Library Introduction](https://eel.is/c++draft/structure.requirements#8).

Given the above general requirement, we can then specify what operations must hold for a moved-from object.

{: .comment }

### Option 1

Option 1 requires that a moved-from object can be used as an rhs argument to move-assignment only in the case that the object has been moved from and it is a self-move-assignment. It introduces a _moved-from-value_ to discuss the properties of the moved-from object without specifying a specific value and requires that self-move-assignment for the moved-from object is valid. The wording allows for `swap(a, a)` without allowing `a = move(a)` in general.

<blockquote><p style='text-align:center;' markdown="span">
    Table 28: _Cpp17MoveConstructible_ requirements
</p>

<table style='margin-left:auto;margin-right:auto;'>
    <tr>
        <th>Expression</th>
        <th>Assertion/note<br>pre-/post-condition</th>
    </tr>
    <tr>
        <td markdown="span" style='white-space:nowrap;'>`T u = rv;`</td>
        <td markdown="span">_Postconditions:_ `u` is equivalent to the value of `rv` before the construction</td>
    </tr>
    <tr>
        <td markdown="span">`T(rv)`</td>
        <td markdown="span">_Postconditions:_ `T(rv)` is equivalent to the value of `rv` before the construction</td>
    </tr>
    <tr>
        <td markdown="span">_common_</td>
        <td markdown="block">
_Postconditions:_
- If `T` meets the _Cpp17Destructible_ requirements;
    - `rv` is in the domain of _Cpp17Destructible_
- If `T` meets the _Cpp17MoveAssignable_ requirements;
    - `rv` is in the domain of the lhs argument of _Cpp17MoveAssignable_ and,
    - `rv` is a _moved-from-value_, such that following a subsequent operation, `t = (T&&)(rv)`, where `t` and `rv` refer to the same object, `rv` still satisfies the postconditions of _Cpp17MoveConstructible_
- If `T` meets the _Cpp17CopyAssignable_ requirements;
    - `rv` is in the domain of the lhs argument of _Cpp17CopyAssignable_
- The value of `rv` is otherwise unspecified
</td><!-- no indent -->
    </tr>
</table>
</blockquote>
{: .proposed}

<blockquote><p style='text-align:center;' markdown="span">
    Table 28: _Cpp17MoveAssignable_ requirements
</p>

<table style='margin-left:auto;margin-right:auto;'>
    <tr>
        <th>Expression</th>
        <th>Return type</th>
        <th>Return value</th>
        <th>Assertion/note<br>pre-/post-condition</th>
    </tr>
    <tr>
        <td markdown="span" valign="top">`t = rv`</td>
        <td markdown="span" valign="top">`T&`</td>
        <td markdown="span" valign="top">`t`</td>
        <td markdown="block">
_Preconditions:_ `t` and `rv` do not refer to the same object, or the object is a _moved-from-value_ (see _Cpp17MoveConstructible_)

_Postconditions:_

- If `t` and `rv` do not refer to the same object, `t` is equivalent to the value of `rv` before the assignment, otherwise the value of `t` is unspecified
- If `T` is required to meet the _Cpp17Destructible_ requirements;
  - `rv` is in the domain of _Cpp17Destructible_
- `rv` is in the domain of the lhs argument of _Cpp17MoveAssignable_
- If `rv` is required to meet the _Cpp17CopyAssignable_ requirements;
  - `rv` is in the domain of the lhs argument of _Cpp17CopyAssignable_
- The value of rv is otherwise unspecified

</td><!-- no indent -->
    </tr>
</table>
</blockquote>
{:.proposed}

Concept `copy_constructible`:

> - If `T` is not `const`;
    - If `T` is required to satisfy `destructible`;
      - `rv`'s resulting state is in the domain of `destructible`.
    - If `T` is required to satisfy `assignable_from` as the `LHS` argument;
      - `rv`'s resulting state is in the domain of the `lhs` argument of `assignable_from and,
      - If `T` is also required to satisfy `assignable_from` as the `RHS` argument;
        - `rv`'s resulting state is a _moved-from-value_, such that following a subsequent operation, `t = (T&&)(rv)`, where `t` and `rv` refer to the same object, `rv` still satisfies the requirements for the resulting state of `rv` in `move_constructible`.
    - Otherwise, the value of `rv` is not specified.
{:.proposed}

The concept `assignable_from` does not place any preconditions on the domain of the operation, therefore there is not a need to reference the _moved-from_value_ requirements.

I find the lack of preconditions and postconditions in the concept requirements as confusing, and in this case. The lack of a precondition for `assignable_from` may be a defect.
{:.comment}

> - If `rhs` is a non-`const` xvalue, the resulting state of the object to which it refers is valid but unspecified ([lib.types.movedfrom]).
{: .strike}

> - If `rhs` is a non-`const` xvalue;
    - If `RHS` is required to satisfy `destructible`;
      - the resulting state of the object to which `rhs` refers is in the domain of `destructible`.
    - If `RHS` is required to satisfy `assignable_from` as the `LHS` argument;
      - the resulting state of the object to which `rhs` is in the domain of the `lhs` argument of `assignable_from`.
    - Otherwise, the value of `rhs` is not specified.
{:.proposed}

### Option 2

Option 2 requires that a moved-from object can be used as an rhs argument to move-assignment always and the result of self-move-assignment is unspecified.

<blockquote><p style='text-align:center;' markdown="span">
    Table 28: _Cpp17MoveConstructible_ requirements
</p>

<table style='margin-left:auto;margin-right:auto;'>
    <tr>
        <th>Expression</th>
        <th>Assertion/note<br>pre-/post-condition</th>
    </tr>
    <tr>
        <td markdown="span">`T u = rv;`</td>
        <td markdown="span">_Postconditions:_ `u` is equivalent to the value of `rv` before the construction</td>
    </tr>
    <tr>
        <td markdown="span">`T(rv)`</td>
        <td markdown="span">_Postconditions:_ `T(rv)` is equivalent to the value of `rv` before the construction</td>
    </tr>
    <tr>
        <td markdown="span">_common_</td>
        <td markdown="block">
_Postconditions:_
- If `T` meets the _Cpp17Destructible_ requirements;
    - `rv` is in the domain of _Cpp17Destructible_
- If `T` meets the _Cpp17MoveAssignable_ requirements;
    - `rv` is in the domain of _Cpp17MoveAssignable_
- If `T` meets the _Cpp17CopyAssignable_ requirements;
    - `rv` is in the domain of the lhs argument of _Cpp17CopyAssignable_
- The value of `rv` is otherwise unspecified

</td><!-- no indent -->
    </tr>
</table></blockquote>
{: .proposed}

<blockquote><p style='text-align:center;' markdown="span">
    Table 28: _Cpp17MoveAssignable_ requirements
</p>

<table style='margin-left:auto;margin-right:auto;'>
    <tr>
        <th>Expression</th>
        <th>Return type</th>
        <th>Return value</th>
        <th>Assertion/note<br>pre-/post-condition</th>
    </tr>
    <tr>
        <td markdown="span" valign="top">`t = rv`</td>
        <td markdown="span" valign="top">`T&`</td>
        <td markdown="span" valign="top">`t`</td>
        <td markdown="block">
_Postconditions:_
- If `t` and `rv` do not refer to the same object, `t` is equivalent to the value of `rv` before the assignment, otherwise the value of `t` is unspecified
- `rv` is in the domain of _Cpp17MoveAssignable_
- If `T` meets the _Cpp17Destructible_ requirements;
    - `rv` is in the domain of _Cpp17Destructible_
- If `rv` meets the _Cpp17CopyAssignable_ requirements;
    - `rv` is in the domain of the lhs argument of _Cpp17CopyAssignable_
- The value of `rv` is otherwise unspecified

</td><!-- no indent -->
    </tr>
</table></blockquote>
{: .proposed}

Concept `copy_constructible`:

> - If `T` is not `const`;
    - If `T` is required to satisfy `destructible`;
      - `rv`'s resulting state is in the domain of `destructible`.
    - If `T` is required to satisfy `assignable_from` as the `LHS` argument;
      - `rv`'s resulting state is in the domain of the `lhs` argument of `assignable_from and,
      - If `T` is also required to satisfy `assignable_from` as the `RHS` argument;
        - `rv`'s resulting state is in the domain of the `rhs` argument of `assignable_from`.
    - Otherwise, the value of `rv` is not specified.
{:.proposed}

The concept `assignable_from`:

> - If `rhs` is a non-`const` xvalue, the resulting state of the object to which it refers is valid but unspecified ([lib.types.movedfrom]).
{: .strike}

> - If `rhs` is a non-`const` xvalue;
    - If `RHS` is required to satisfy `destructible`;
      - the resulting state of the object to which `rhs` refers is in the domain of `destructible`.
    - If `RHS` is required to satisfy `assignable_from` as the `LHS` argument;
      - the resulting state of the object to which `rhs` is in the domain of the `lhs` argument of `assignable_from`.
    - Otherwise, the value of `rhs` is not specified.
{:.proposed}

## Future Implications

### Weaker Guarantees for Future Components

There are several cases in the existing standard where satisfying the guarantee of _valid but unspecified_ has negative performance implications. For example, move operations on node based containers such as `std::list()` are not `noexcept` because some implementations require an allocation for a valid list. To avoid the allocation, the implementation would have to add additional null-checks for nearly all list operations.

Running a release build compiled with Visual C++ 2019[^vc-version] and given the following:

{% raw %}
```cpp
std::vector<std::list<int>> c{{9}, {8}, {7}, {6}, {5}, {4}, {3}, {2}, {1}, {0}};
std::list<int> a;
```
{% endraw %}

A subsequent call `c.push_back(std::move(a));` generates 21 unnecessary calls to `new` and 20 unnecessary calls to `delete`. The additional call to `new` is to leave `a` in a valid but unspecified state. Sorting the same initial vector with `std::sort(begin(c), end(c));`. Generates 9 calls to new and delete, all unnecessary.

Unfortunately, weakening the guarantees of an existing component is a breaking change but the committee should consider carefully before compromising new components in a similar fashion.

Note that a new component could still provide stronger guarantees than is required, for example a container may support `clear()`, `reset()`, or other operations that establish a new value.

### Class invariants

Although the proposal _Support for contract based programming in C++_, [P0542R5](#p0542), did not include class invariants, it is possible that a future version of the standard will. At which time the likely correct decision would be that invariants are not checked on an object that has just been moved from by default, and more generally allow the declaration of _unsafe_ operations where invariants are not checked.

## References

Parent, Sean. _Move Annoyance_, Addison-Wesley, 31 Mar. 2021, <https://sean-parent.stlab.cc/2021/03/31/move-annoyance.html>.&nbsp;[&#8617;](#bl-move-annoyance)
{: #move-annoyance}

Sutter, Herb. “Move, Simply.” _Sutter's Mill_, 21 Feb. 2020, <https://herbsutter.com/2020/02/17/move-simply/>.

Romer, Geoffrey. “Moved-from Objects Need Not Be Valid.” _C++ Standards Committee Papers_, ISO/IEC WG21, 10 Jan. 2020, <https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2020/p2027r0.pdf>.
{: #p2027'}

G. Dos Reis et. al. "Support for contract based programming in C++." _C++ Standards Committee Papers_, ISO/IEC WG21, 08 June 2018, <http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0542r5.html>
{: #p0542'}

Several links contained within the document. They will be listed here in a future draft along with any existing papers that come to my attention.
{: .comment }

## Acknowledgements

Thanks to Howard Hinnant, Herb Sutter, Jonathan Wakely, Nicolai Josuttis, Nicholas DeMarco, Eric Neibler, Dave Abrahams, and John Lakos for the many discussions and arguments that resulted in this paper.

[^cpp-requirements]:
    Similar wording with the same intent appears in every version of the C++ Standard since C++11.

[^vc-version]:
    Microsoft Visual C++ 2019  00435-60000-00000-AA859
