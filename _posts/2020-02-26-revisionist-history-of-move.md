---
layout: post
title: Revisionist History of Move
description: ""
category: draft
mathjax: true
tags: [standard, move]
---

<style>
.proposed {
    background: #eeffee;
}
.comment {
    background: #eeeeff;
}
</style>

## Abstract
{:.no_toc}
> This page is a work in progress.
{: .comment }

### Table of
{: .no_toc }
* this unordered seed list will be replaced by toc as unordered list
{: toc }

### Document Conventions

> This is proposed wording for the standard. There may be more than one proposed variant for the same section.
{: .proposed }

> This is a quote from an existing document.

> This is a comment or work in progress.
{: .comment }

## History

## Problem

Compare the description for the old [SGI STL LessThanComparable concept](http://www.martinbroadhurst.com/stl/LessThanComparable.html) versus the [C++17 concept](http://eel.is/c++draft/utility.arg.requirements#tab:cpp17.lessthancomparable).

## Proposed Solutions

> We need wording to state that these are requirements for operations with the domain of values passed to the STL component. Borrowing from [the text for input iterators](http://eel.is/c++draft/iterator.cpp17#input.iterators-2):
{: .comment }

> The term _domain_ of an operation is used in the ordinary mathematical sense to denote the set of values over which an operation is (required to be) defined. This set can change over time. Each component may place additional requirements on the domain of an operation. These requirements can be inferred from the uses that a component makes of the operation and is generally constrained to those values accessible through the arguments of the operation. \[ _Example:_ The call `find(a, b, x)` is defined only if the value of `a` has the property _p_ defined as follows: `b` has property _p_ and a value `i` has property _p_ if (`*i == x`) or if (`*i != x` and `++i` has property _p_). _— end example_ \]
{: .proposed}

### Option 1

> This option only requires that a moved-from object can be used as an rhs argument to move-assignment only in the case that the object has been moved from and it is a self-move-assignment. It introduces a fixed-point notion to discuss the properties of the moved from value without specifying a specific value and requires that self-move-assignment for the moved-from object is a no-op. This allows for `swap(a, a)` without allowing the generally contradictory `a = move(a)`.
{: .comment }

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

> This option requires that a moved-from object can be used as an rhs argument to move-assignment always, but the result of self-move-assignment is unspecified.
{: .comment }

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

## Acknowledgements

Howard Hinnant, Herb Sutter, Jonathan Wakely, Nicolai Josuttis, Nicholas DeMarco
