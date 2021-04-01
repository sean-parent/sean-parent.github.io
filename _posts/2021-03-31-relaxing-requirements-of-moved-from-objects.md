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
}
.comment {
  background: #eeeeff;
}
</style>

| Document Number: | D2345R0 |
| Date: | 2021-03-31 |
| Reply-to: | Sean Parent, [sean.parent@stlab.cc](sean.parent@stlab.cc) |
| Audience: | LWG & LEWG |

### Table of Contents
{: .no_toc }

- This unordered seed list will be replaced by the TOC as an unordered list
{: toc }

## Introduction
{: .no_toc }

The C++ Standard Library requirements are overly restrictive regarding the state of a moved-from object. The strong requirements impose an unnecessary burden on implementers and imposes a performance impact of user-defined operations.

This paper details the issue and presents some suggested wording to address it. Depending on the wording chosen, it may be possible to address the issue with a defect report retroactively.
{: .comment }

### Document Conventions

> This is the proposed wording for the standard. There may be more than one proposed variant for the same section.
> {: .proposed }

> This is a quote from an existing document.

> This is a comment or work in progress.
> {: .comment }

## Motivation and Scope

Given an object, `rv`, which has been moved from, the C++20[^cpp-requirements] Standard specifies the [required postconditions of a moved-from object](http://eel.is/c++draft/utility.arg.requirements):

> `rv`'s state is unspecified <br>
  \[_Note_: `rv` must still meet the requirements of the library component that is using it. The operations listed in those requirements must work as specified whether `rv` has been moved from or not. &mdash; _end note_\] &mdash; Table 28, p. 488 C++20 Standard.

The requirement applies to both [_Cpp17MoveConstructible_](http://eel.is/c++draft/utility.arg.requirements#tab:cpp17.moveconstructible) and [_Cpp17MoveAssignable_](http://eel.is/c++draft/utility.arg.requirements#tab:cpp17.moveassignable). The note is [not normative](https://www.iso.org/sites/directives/current/part2/index.xhtml#_idTextAnchor321) but does clarify that the requirements on a moved-from object are not relaxed.

In general, unless move is specified to make a copy, this requirement is not achievable. For example, the [sorting algorithms require](https://eel.is/c++draft/alg.sorting.general) `comp(*i, *j)` induce a strict weak ordering. Therefore, a moved-from object must be ordered with respect to every other value in the sequence with an arbitrary user-supplied comparison function. Only a value within the initial sequence could satisfy that requirement.

No implementation of the Standard Library will ever invoke a comparison, user-defined or otherwise, on an object that the library itself moved-from. Such a comparison would not have meaning. However, the way this requirement is frequently taught is that all operations required by the Standard Library must be _total_ when used with a moved-from object. i.e. `rv < a` must be valid and induce a strict weak ordering for _all possible values of `a`_. However, even such a strong guarantee does not solve the issue for arbitrary operations passed to the Standard Library.

Attempting to make all operations total with respect to moved-from objects imposes an unnecessary performance penalty and the implementation of such operations is error-prone. Examples and details are provided in an _Annoyance_ I wrote for the upcoming [_Embracing Modern C++ Safely_]({% post_url 2021-03-31-move-annoyance %}).

### Requirements of a Moved-From Object

All Standard Library implementations only require the following operations on an object, `rv`, that the Library moved from within an operation:

- `rv.~()` (The language also requires this for implicitly moved objects)
- `rv = a`
- `rv = move(a)`
- `rv = move(rv)`

The last requirement only appears in the implementation of `std::swap()` when invoked as `swap(a, a)`. It imposes some additional complexity because `a = move(a)` is, in general, a contradiction and is not required. The required postcondition of `a = move(b)` is that `a` holds the prior value of `b` and the value of `b` is unspecified but may be guaranteed to be a specific value. For example, if `a` and `b` are both of type `my_unique_ptr<T>` with the guarantee that `a` will hold the prior value of `b`, and `b` will be equal to `nullptr`. Then for the expression `a = move(a)`, the only way both of those guarantees could be satisfied is if `a` is already equal to `nullptr`. The current standard avoids this contradiction by defining the postcondition of move assignment for `std::unique_ptr<T>` as equivalent to `reset(r.release())` which provides a stronger guarantees than any standard component requires. There is open debate as to if `swap(a, a)` should be required to be valid if `a` satisfies the requirements for an argument of `swap()`. I propose two options for wording, to handle both cases.

### Non-Requirements

There is not a requirement to provide guarantees across operations that result in moved-from objects. For example:

```cpp
T a[]{ v0, v1, v1, v2 };
(void)remove(begin(a), end(a), v1);
sort(begin(a), end(a));
```

After `remove()`, the last two object at the end of `a` have unspecified values and may have been moved from. There is no requirement that these moved-from objects also satisfy the requirements of `sort()` by being in the domain of the operation `operator<()`, even if `v0`, `v1`, and `v2` are within the domain. The two sets of requirements are independent and this invocation of `sort()` for a particular type, `T`, may or may not be valid. 

Assuming `v0` and `v2` are in the domain of `operator<()` for `sort()` the following is guaranteed:

```cpp
T a[]{ v0, v1, v1, v2 };
auto p = remove(begin(a), end(a), v1);
sort(begin(a), p);
```

## Impact on the Standard

 All components which are _Movable__ in the Standard Library currently satisfy the requirements as stated by both options below. **Both options are non-breaking changes in that they only relax the requirements.** However, the second option would not guarantee that `swap(a, a)` is valid, but it would continue to be valid for all library components since those components provide stronger guarantees. With either option, it may be possible to adopt these options retroactively as part of addressing a defect since neither option is a breaking change.

## Technical Specifications

We need a general requirement regarding _the domain of an operation_. Borrowing from [the text for input iterators](http://eel.is/c++draft/iterator.cpp17#input.iterators-2):

> Unless otherwise specified, there is a general precondition for all operations that the requirements hold for values within the _domain of the operation_.
> {: .proposed}

> The term _domain of the operation_ is used in the ordinary mathematical sense to denote the set of values over which an operation is (required to be) defined. This set can change over time. Each component may place additional requirements on the domain of an operation. These requirements can be inferred from the uses that a component makes of the operation and is generally constrained to those values accessible through the operation's arguments.
> {: .proposed}

The above wording should appear in the [Requirements section of the Library Introduction](https://eel.is/c++draft/structure.requirements#8).

Given the above general requirement, we can then specify what operations must hold for a moved-from object.

A separate issue is if new library components going forward should make stronger guarantees than those required as they currently do.
{: .comment }

### Option 1

Option 1 requires that a moved-from object can be used as an rhs argument to move-assignment only in the case that the object has been moved from and it is a self-move-assignment. It introduces a fixed-point notion to discuss the properties of the moved from value without specifying a specific value and requires that self-move-assignment for the moved-from object is a no-op. The wording allows for `swap(a, a)` without allowing the generally contradictory `a = move(a)`.

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
    - `rv` is a _fixed-point_, such that following a subsequent operation, `u = (T&&)(rv)`, where `u` and `rv` refer to the same object, the value of the object is equivalent to the value before the operation
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
_Preconditions:_ `t` and `rv` do not refer to the same object, or the object value is a fixed-point (see _Cpp17MoveConstructible_)

_Postconditions:_

- `t` is equivalent to the value of `rv` before the assignment
- If `T` meets the _Cpp17Destructible_ requirements;
  - `rv` is in the domain of _Cpp17Destructible_
- `rv` is in the domain of the lhs argument of _Cpp17MoveAssignable_
- If `rv` meets the _Cpp17CopyAssignable_;
  - `rv` is in the domain of the lhs argument of _Cpp17CopyAssignable_
- The value of rv is otherwise unspecified

</td><!-- no indent -->
    </tr>
</table>
</blockquote>
{: .proposed}

### Option 2

> Option 2 requires that a moved-from object can be used as an rhs argument to move-assignment always, but the result of self-move-assignment is unspecified.
> {: .comment }

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

## References

Sutter, Herb. “Move, Simply.” _Sutter's Mill_, 21 Feb. 2020, [herbsutter.com/2020/02/17/move-simply/](https://herbsutter.com/2020/02/17/move-simply/).

Several links contained within the document. They will be listed here in a future draft along with any existing papers that come to my attention.
{: .comment }

## Acknowledgements

Thanks to Howard Hinnant, Herb Sutter, Jonathan Wakely, Nicolai Josuttis, Nicholas DeMarco, Eric Neibler, Dave Abrahams, and John Lakos for the many discussions and arguments that resulted in this paper.

[^cpp-requirements]:
    Similar wording with the same intent appears in every version of the C++ Standard since C++11.
