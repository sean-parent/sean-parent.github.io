---
layout: post
title: "About Move"
description: ""
category: 
tags: [C++11, move, regular, efficient]
---
{% include JB/setup %}

## background

At C++Now 2014 I presented a talk about writing complete and efficient types. My talk came directly after a talk Eric Neibler gave about writing library API's.

Although not the major part of either of our talks, we presented differing opinions, and advice, regarding the requirements of an object that has been moved from.

To further confuse matters, I discussed how std::move() is an inefficient operation, and how one could implement a destructive move that would be more efficient. I was not advocating taking such an approach, simply pointing it out to illustrate what is meant by efficient basis operations.

Several people have confused the two issues, including Eric, and have asserted that I claim std::move is destructive (it isn't).

Yesterday the topic exploded again when Eric Neibler posted on twitter:

>A debate raged at @cppnow between me and @SeanParent about destructive move. The committee wants in on the action: http://isocpp.org/files/papers/n4034.pdf

Again, we debated the requirements of a moved from type, not destructive move. Though we discussed if a destructive move couldn't be more efficient, and safer, than the current move. Having such a debate on twitter in 140 character snippets isn't possible, and it spilled over onto github wikis, with comments on twitter. I'd been meaning to get a blog functioning with jekyll so this provided the excuse.

## terms

The C++ standard defines _move_ as:

> T u = rv; u is equivalent to the value of rv before the construction  
> rv’s state is unspecified.

I would strengthen that to be "u is equal to..." but otherwise I agree with the wording up to this point. This is the description of MoveConstructible, a concept fragment, there is no need to discuss the semantics of MoveAssignment because assignment can be expressed in terms of MoveConstruction.

Borrowing from _Element of Programming_ (section 1.3), an object is _well formed_ iff it is a valid representation of a value. This relates to the notion of a type _invariant_ and _validity_. Because those terms seem to be so heavily charged, I will not use them, instead preferring well-formed.

A regular type, may be _partially formed_ (section 1.5). In such a state the only valid operations are destruction and assignment-to (partially formed objects may only appear on the left hand side of assignment). A partially formed object need not be a valid representation of a value.

An operation on an object is _safe_ if the post-condition for the operation leaves the object in a well formed state. An operation is _unsafe_ if it does not. An unsafe operation requires subsequent operations to either return the object to a well formed state or to safely destroy it. That doesn't imply that an unsafe operation leaves an object in a partially formed state, it may leave the object in any state, but a partially formed state is a common and useful state. Generally we like to keep unsafe operations contained within the implementation of our type and not as part of the public interface.

An operation is _efficient_ if there is no faster way to perform it.

The _computational basis_ for a type is a finite set of operations that enable the construction of any other operation on the type.

An _efficient basis_ is a computational basis that enables the construction of any other efficient operation on the type.

## the conflict

Unfortunately there are times where the goal of safety conflicts with the goals of providing an efficient basis and one must choose.

**Move, as defined above, is an unsafe operation.**

How can I say that? The observant standard reader will point out that the standard says a bit more about the state of rv:

> rv’s state is unspecified [ _Note_: rv must still meet the requirements of the library component that is using it. The operations listed in those requirements must work as specified whether rv has been moved from or not. — _end note_ ]

That would seem to require that rv, though it's state is unspecified, must be well formed. But in general there is no value rv could hold that could satisfy that requirement. In order to satisfy that requirement the value left in rv would have to be dependent on the algorithm being used.

To illustrate. Let's say I had the following class:

``` cpp
struct double_t
{
   double_t(const double_t&) = default;
   double_t(double_t&& x) noexcept : m_(x) { x = nan(""); }
   double_t& operator=(const double_t&) = default;
   double_t& operator=(double_t&& x) noexcept { m_ = x; x = nan(""); }

   inline friend bool operator<(const double_t& x, const double_t& y)
   { return x < y; }

   double m_;
};
```

If I were to pass a range of `double_t` to std::sort(), then a moved from value would violate the requirements of the algorithm, the ordering is no longer strict-weak. But there is _no_ way to know the requirements of the algorithm when deciding the value for the moved from state. Perhaps 0.0 is bad for an algorithm relying on divide, another algorithm may require no duplicates.

The only valid operations on the moved from type are to establish a new value, through assignment, or to destroy the object. That is, the requirement is that the object is partially formed. The only way to strengthen that requirement would be to require that the value of the moved from object is the same as another object being used by the algorithm. The only way to satisfy that locally is by defining move to be copy. It works, but we lose efficiency. Unless a specific value is required, the only requirement is that the object is partially formed.

(In the assignment case, there is another option, which is to define move assignment as swap. Prior to C++11, swap was used by many of the STL algorithms as a form of move which could be implemented more efficiently than copy.)

## could we fix move?

There isn't anything broken about it, other than the note in the standard. But if the question is can we make move safe, the answer is no. But we could make it safe in more circumstance, and we could make it efficient.

Here is where we go down the destructive move rathole. For background you might look at a wiki page I started (it will get migrated at some point to this blog) for a [non-proposal for destructive move](https://github.com/sean-parent/sean-parent.github.com/wiki/Non-Proposal-for-Destructive-Move).

The key thing here is that we change our definition of move to:

> T u = rv; u is equivalent to the value of rv before the construction  
> rv is uninitialized and will not be destructed if it is a local variable

We get around the whole _unspecified_ issue by defining the state of rv to be gone. This makes the one case where a local variable is explicitly moved safe. It also makes move efficient because it doesn't require writing back to the moved from object.

Unfortunately it doesn't make all uses of move safe. Consider the problem of sorting a vector. As each element is moved, it leaves a "hole" in the vector. At this point, the vector is not well-formed, it isn't until the last element is moved from a temporary back into the vector that the invariants of the vector are restored. A side effect of the gained efficiency is a lack of safety.

And here is the crux - every permutation algorithm suffers from this same problem, so there is, by definition no possible safe efficient basis.

## unsafe isn't _bad_

An unsafe operation is a fragment of a safe operation, it must be combined with other operations to restore the object to a fully formed state. Move is unsafe, but sort is not. Calling a destructor explicitly can also be an unsafe operation but if we are to have efficient code, sometimes we must. The notion of a partially formed type in EoP is used to specify the requirements of a default constructed value. This runs into exactly the same issues as a moved from object, one must either specify the value of default initialization (such as zero initialized) or accept that the requirement is only that the object is partially formed. The former is inefficient and imposes a requirement that a zero initialized object represent some value.

None of this answers what move or default initialization should do for a specific type, only what the requirements are for what they _must_ do to interoperate. Any specific type may freely make stronger guarantees.
