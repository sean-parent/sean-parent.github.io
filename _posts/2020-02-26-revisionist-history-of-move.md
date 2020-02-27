---
layout: post
title: Revisionist History of Move
description: ""
category:
mathjax: true
tags: [efficient, loops]
draft: true
---

<table>
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
        <td markdown="span">_Postconditions:_ `T(rv)` is equivalent ot the value of `rv` before the construction</td>
    </tr>
    <tr>
        <td markdown="span">_common_</td>
        <td markdown="block">
_Postconditions:_
- `rv` is destructible
- If `rv` is a glvalue and `T` is required to be _Cpp17MoveAssignable_;
    - `rv` satisfies any preconditions for the lhs argument to move-assignment and,
    - `rv` is a _fixed-point_, such that following a subsequent operation, `u = (T&&)(rv)`, where `u` and `rv` refer to the same object, the value of the object is equivalent to the value before the operation
- If `rv` is a glvalue and `T` is required to be _Cpp17CopyAssignable_;
    - `rv` satisfies any preconditions for the lhs argument to copy-assignment
- The value of `rv` is otherwise unspecified

</td><!-- no indent -->
    </tr>
</table>

<table>
    <tr>
        <th>Expression</th>
        <th>Return type</th>
        <th>Return value</th>
        <th>Assertion/note<br>pre-/post-condition</th>
    </tr>
    <tr>
        <td markdown="span" valign="top">`t = uv`</td>
        <td markdown="span" valign="top">`T&`</td>
        <td markdown="span" valign="top">`t`</td>
        <td markdown="block">
_Preconditions:_ `t` and `rv` do not refer to the same object, or the object value is a fixed-point (see _Cpp17MoveConstructible_)

_Postconditions:_
- `t` is equivalent to the value of `rv` before the assignment
- `rv` is destructible
- If `rv` is a glvalue and `rv` is required to be _Cpp17CopyAssignable_;
    - `rv` satisfies any preconditions for the lhs argument to copy-assignment
- The value of rv is otherwise unspecified

</td><!-- no indent -->
    </tr>
</table>
