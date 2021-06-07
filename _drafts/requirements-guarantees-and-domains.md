—
layout: post
title: Requirements, Guarantees, & Domains (Oh My!)
mathjax: true
—
<style>
.comment {
  background: #eeeeff;
  font-family: sans-serif;
  padding: 8px;
}
</style>

> Understanding why software fails is important, but the real challenge is understanding why software works. &mdash; _Alexander Stepanov_

### Table of Contents
{: .no_toc }

- This unordered seed list will be replaced by the TOC as an unordered list
{: toc }

## Introduction and History

In C++, we take for granted that calling one of the Standard algorithms will do something meaningful. i.e., when invoking `p = std::find(first, last, value)` we expect that if an item equal to `value` is contained in the sequence `[first, last)` then `p` will point to the first such item, otherwise `p` will equal `last`. **How do we know `find` will actually perform this operation?**

The correctness of `find()` and all of the Standard components are built with an intricate set of logic stated in the form of _requirements_ and _guarantees_ that ensure, when used correctly, the operation performs as expected. The _requirements_ and _guarantees_ are rooted in _Hoare logic_.

### Hoare Logic

_Hoare logic_ is a formal system for reasoning about the correctness of computer programs.[^hoare-logic] It was proposed in 1969 by Tony Hoare and based on prior work by Robert Floyd. Hoare logic is also known as _Floyd-Hoare logic_.

Hoare logic describes computation statements as a _Hoare triple_, two assertions bracketing an operation.

$$
\begin{aligned}
  P\{Q\}R.
\end{aligned}
$$

Where $$P$$ and $$R$$ are assertions and $${Q}$$ is an operation. $$P$$ is the precondition, and $$R$$ the postcondition. When the precondition is met, executing the command establishes the postcondition. An operation without preconditions would be written as $$true\{Q\}R.$$

### Design by Contract

_Design by contract_ is a term coined by Bertrand Meyer and his Eiffel programming language included constructs to specify preconditions, postconditions, and _class invariants_.[^design-by-contract] The latter is a collection of postconditions that apply to all operations on a class and hence, as preconditions the invariants are always guaranteed to hold.

If a given set of preconditions are met, an operation either succeeds or flags an exception and leaves the machine in a known or determinable state.

### Generic Programming and Concepts

_Generic programming_ is a term first used by Alex Stepanov and David Musser in 1988.[^generic-programming] Developed independently from Bertrand Meyer’s work, Generic Programming also relies on Hoare logic to reason about the correctness of generic components. A key idea in generic programming is associating _syntactic constructs_ with a set of axioms that specify the semantics. The syntax and axioms form a set of required preconditions and postconditions on operations on types used in generic components. Generic programming extends the idea of Hoare logic from values to types. In generic programming, a collection of associated syntactic requirements and axioms are named, creating a _named requirement_. i.e., _EqualityComparable_. In 1998, soon after the Standard Template Library was officially adopted into C++98, Alex and James Dehnert coined the term _concepts_ as a more formal definition of named requirements.

A concept is a similar notion to an algebraic structure in mathematics. For example, in mathematics a _semigroup_ is a structure formed by values with an associative binary operation, and by convention, the associative operation is denoted with a $$\cdot$$ operator.

## Requirements

For non-generic component operating on a known type, a set of preconditions on the arguments passed to an operation are sufficient. But for a generic component, there are additional requirement on the operations available on the arguments and those operations semantics. Collectively, the preconditions and concepts define the requiements of a generic component.

### Preconditions

Although a precondition is a predicate, it may not be testable.

### Class Invariants
### Concepts
## Guarantees
### Postconditions
### Models
## Domain of an Operation
### Weakest requirements, works as coded

Insert quote from Craig Silverstein here.
{: .comment}

##### Items here are snippets that may be incorporated above

The description of `std::find()` might have been better stated that `std::find()` requires the `value` be equality_comparable with values in the sequence, however operator==() of the items in the sequence is not required to be reflexive, but this must hold for `value`. That would allow `std::find(f, l, 3.0)` while disallowing `std::find(f, l, std::nan(“”))`.

Now, I think the best that can be done to salvage the requirements of `std::find()` is to add a note that if the `value` and items in the sequence satisfies equality_comparable then the requirements of `std::find()` are satisfied and if a value is found it will be equal to the `value`.

Compare the description for the old [SGI STL `find()` algorithm](http://eel.is/c++draft/alg.find), to the [specification for `std::find()` in C++20](http://eel.is/c++draft/alg.find).

SGI STL defined `find()` as requiring that the value type, “is a model of _EqualityComparable_.” And the concept _EqualityComparable_ contained the precondition that [“x and y are in the domain of `==`”](http://www.martinbroadhurst.com/stl/EqualityComparable.html).

The definition of domain for `==` still exists in the C++ standard but the use is limited to [_input iterators_](http://eel.is/c++draft/iterator.cpp17#input.iterators-2) and the [requirements for _sentinel_](https://eel.is/c++draft/iterator.concept.sentinel#3).

C++20 does not require the value type in `std::find()` is _EqualityComparable_ but instead requires `operator==()` exist, that it is callable with the value type, and that it returns an item convertible to bool. It no longer makes reference to an _EqualityComparable_ concept. To illustrate the distinction; in the SGI STL the following invocation of `find()` violates the preconditions and is undefined behavior:

```cpp
double a[]{1.0, nan(“”), 3.0};
double* p = find(begin(a), end(a), nan(“”));
```

The reason is that `nan` is outside the domain of operation for `operator==()` when used with `find()` because it doesn’t satisfy the requirements of _EqualityComparible_.

By contrast, in C++20, the above code is well defined, but it won’t actually find the `nan` value.

The weakening of the requirements in the C++ Standard allows for cases like the following:

```cpp
double a[]{1.0, nan(“”), 3.0};
double* p = find(begin(a), end(a), 3.0);
```

With the SGI STL specification this is still undefined behavior, but in C++20 this is well defined and will result with `*p == 3.0`.

Concepts, which were created to define the semantics of algorithms and containers, only rarely appear in the current specification for algorithms and containers, and then mostly for iterators. This leaves the algorithms devoid of semantics. i.e. The above `find()` example doesn’t actually _find_ anything. The concept requirements and preconditions were created




## History

The C++ Standard Library requirements and concepts are missing a general precondition that reachable values from the arguments must be within the operations domain. Instead, the requirements are stated as holding for all values of the type. The missing precondition has undesired effects, including imposing unnecessary restrictions on moved-from objects and reducing the usefulness of concepts.

Compare the description for the old [SGI STL LessThanComparable concept](http://www.martinbroadhurst.com/stl/LessThanComparable.html) versus the [C++17 concept](http://eel.is/c++draft/utility.arg.requirements#tab:cpp17.lessthancomparable).



The Standard no longer relies heavily on named requirements. For example, [`std::find()` in the SGI STL required that `*i` satisify _EqualityComparable_](http://www.martinbroadhurst.com/stl/find.html), but the current Standard only requires that `*i == value` is comparable to `true`.

[:hoare-logic]
 Hoare, C. A. R. [“An axiomatic basis for computer programming”](http://web.stanford.edu/class/cs357/hoare69.pdf). _Communications of the ACM_, vol. 12, no. 10, Oct. 1969, pp. 576–580. doi:10.1145/363235.363259.
