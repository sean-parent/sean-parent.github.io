# Local Reasoning

## Introduction

_Local reasoning_ is the ability to look at a defined unit of code, understand it, and verify its correctness without understanding all the contexts in which it is used.

There are two units of code we will discuss:

- Functions
- Classes

### Terminology

An API has two sides, the `caller` and the `callee`. The `caller` invokes the `callee`. The `callee` is the function or method being invoked.

## Functions

Let's start with a function signature.

```cpp
void f();
```

Either this function does _nothing_ or whatever it does is entirely through side effects. Either way, we should document it:

```cpp
// Does nothing
void f();
```

Now we can implement `f`.

```cpp
// Does nothing
void f() { }
```

I hope everyone is convinced that `f()` is implemented correctly. A requirement for local reasoning about a function is that the function has a specification, a contract.

Suppose a piece of code has a contract, and everything it invokes also has one. In that case, we can read the implementation and prove (or disprove) that the function body is correct and fulfills the contract. But this isn't another talk about contracts; instead, it is about general principles for constructing code that is _simple_ to reason about and that we can prove correct.

### Function Arguments

Let's make our function just a bit more complicated:

```cpp
// Increments the value of `x` by 1
void f(int& x) { x += 1; }
```

This is still very simple. But an additional precondition just crept in. What is it?

What if another thread is reading `x` at the moment we update it? This would be undefined behavior. There is an implicit precondition here:

```cpp
// Increments the value of `x` by 1
// precondition: no other thread of execution is accessing `x` during this operation
void f(int& x) { x += 1; }
```

This precondition cannot be tested or verified by `f`. The caller must ensure it. We raise the prospect of aliasing when we introduce indirection into an interface. Aliasing makes reasoning about a piece of code more complex. The rest of this talk is about techniques to control aliasing and to confine the effect of an operation so that the processes of local reasoning are simplified.

Take a look at this precondition. We would need to write this on every mutable argument to every function. And that's ridiculous. We have two options to simplify the contract. The first is to remove the indirection - eliminate mutation (you can't mutate the caller's argument without indirection).

```cpp
// Returns the successor of `x`.
int f(int x) { return x + 1; }
```

The second option is to add a general rule about aliasing arguments.

**General Preconditions:**

- Arguments passed to a function by non-const reference cannot be accessed by other threads during the operation
- Arguments passed to a function by const reference cannot be written by another thread during the operation

```cpp
// Increments the value of x by 1.
void f(int& x) { x += 1; }
```

We don't normally pass `int` by reference, we pass objects by reference as an optimization to avoid unnecessary copies.

> There is a duality between transformations and the corresponding actions: An action is definable in terms of a transformation, and vice versa:

```cpp
void a(T& x) { x = f(x); } // action from transformation
```

> and

```cpp
T f(T x) { a(x); return x; } // transformation from action
```

> -- [Elements of Programming, Section 2.5](http://elementsofprogramming.com/eop_coloredlinks.pdf)

For a given instance, either an action or transformation may have a more efficient implementation.

Let's give our increment function a decent name and make it a little more general:

```cpp
// Offsets the value of x by n
void offset(int& x, const int& n) {
  x += n;
}
```

If we call this as:

```cpp
int x{2};
offset(x, x);
```

What is the value of x after? We expect `4` but also the `n` argument to be unchanged. If we implemented this as:

```cpp
// Offsets the value of x by n
void offset(int& x, const int& n) {
  for (int i = 0; i != n; ++x) ;
}
```

```cpp
int x{2};
offset(x, x);

print("{}", x);
```

Now, what happens? For what value of `x` is this code correct? Nobody would ever write this code, but what about:

```cpp
vector a{ 0, 1, 1, 0 };

erase(a, a[0]);

print("{}", a);
```

```
[1, 0]
```

Why? In the presence of aliasing under mutation, the semantics of an operation are nearly impossible to define.
https://godbolt.org/z/hP1dsTPsa

> To achieve memory safety, Swift requires exclusive access to a variable in order to modify that variable. In essence, a variable cannot be accessed via a different name for the duration in which the same variable is being modified as an inout argument or as self within a mutating method.
> -- [Swift 5 Exclusivity Enforcement](https://www.swift.org/blog/swift-5-exclusivity/)

> Mutable references have one big restriction: if you have a mutable reference to a value, you can have no other references to that value.
> -- [The Rust Programming Language: References and Borrowing](https://doc.rust-lang.org/book/ch04-02-references-and-borrowing.html)

[ Garbage collection is for people who don't know what their pointers are doing. ]

So now we have a more general rule.

[ Need a more precise statement here. ]

**General Preconditions:**

- A mutated argument cannot be accessed except through the argument for the duration of the operation.
- Constant arguments cannot be mutated for the duration of the operation

But what is "an argument"? Let's consider this interface:

```cpp
void operation(shared_ptr<widget>);
```

Is the argument the `shared_ptr`? The `widget`? Maybe something _held_ by the `widget`?

To understand the effect of this operation, we need to understand what _value_ the argument represents.

[ This gets us to _equational reasoning_ which is essential for referential transparency. define equality ]

To understand equality, we need to understand what _parts_ constitute the value of our type. This gets us to whole/part relationships. When passing an argument, we want to pass the _value_ being used or modified.

[ A pointer type represents two values - the pointer and the value being pointed to. ]

[ Copy is an essential operation defined in terms of equality. "If you don't understand what it means to copy an object of a given type, you don't understand the type."]

[ All types are copyable. It is inherent to our definition of type and machine model. We may create types where we choose not to implement copy when the type represents an extrinsic relationship. ]

In this case, it is unlikely that our operation is operating on the pointer. The interface should be:

void operation(widget& a);

There is an argument that a reference is _just_ a pointer, but this isn't the case. A reference cannot be nil, and does not allow any operation on the pointer, only on the object pointed to. A reference has no value of its own. Pointers are _positions_ of object.

- Pointers should not appear in interfaces unless object positions are relevant to the operation (such as with iterators).

It could be a position in memory, such as a raw pointer, or it could be a position within a data structure.

[ efficient functional updates. ]

We still need to understand what it means for two shapes to be equal to understand the extent of this operation. Consider a shape defined as:

```cpp
class shape {
  rectangle _root;
  vector<shared_ptr<shape>> _shapes; // [
public:
  // Does this modify the _root? _shapes?
  // If this modifies the _shapes, what are they shared with?
  void offset(int x, int y);
};
```

Interfaces that conditionally take ownership are difficult to use.

Don't project overlapping references unless immutable
Call things that produce references _projections_.

Terminology - escape
Notion of lifetime of projection (end of projection).

```

```
