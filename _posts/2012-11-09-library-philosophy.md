---
layout: post
title: "Library Philosophy"
tagline: ""
description: ""
category:
date: 2012-11-14
tags: []
---
{% include JB/setup %}

I'm just getting started on a major overhaul of [ASL](http://stlab.adobe.com) and moving ASL to [GitHub](http://github.com/stlab). There is a lot of good work in ASL but it is looking a bit dated. With the 2.0 release I intend to clean out the cruft and polish up the jewels. The goal for 2.0 is to be a significantly smaller and more practical release.

I've been giving some thought about what makes a good library, and decided to use this opportunity to learn about blogging and GitHub.

<!-- more start -->

### Defer to a Higher Standard

This is the first rule of a good library and the rest are really lemmas that follow.

If there are established vocabulary, semantics, or conventions use them. Don't invent yet another way to say the same thing. The hierarchy for deferral should be:

* Truth
* Mathematics
* Computer Science
* Language Standard
* Other Standards (including de-facto standards)
* Established Libraries
* Platform (only for platform specific code)
* Company
* Product/Team
* Your Conventions

Invention should be a rarity. The hierarchy applies to both writing and using library components. If either `ProductString` or `std::string` will suffice, then use `std::string`. In doing so you make it easier for an outsider to understand, and increase the opportunities for reuse.

I know this list is going to be contentious. It is often inverted with teams putting a high priority on consistant code within a product. This, however, is a often a good way to ensure that your code will not be shared and will not interoperate well with useful libraries.

*WARNING: Following this advice may get more people to adopt your code, but it is also a proven career limiting tactic. Unfortunately, sometimes you may have to do as the Romans do.*

### Explicit Rationale

When writing a library it is important to document the rationale for each component. This provides guidance to the developer as to when the type or function is needed. If you can compare to an existing component, all the better.

How often do you see a library that has a `String` class, and no explanation as to why the library introduces yet another string type. Does it have an advantage over `std::string`?

### Augment

Whenever possible, choose to augment an existing library instead of writing a replacement. Can the needed functionality be added with stand alone functions or simple adaptors? Prefer those to writing replacement components.

If you must replace, try to superset or provide as much functionality from the component you are replacing as is feasible. You don't want to force developers to make unnecessary tradeoffs.

### Avoid Complexity

There is a tendency by library developers to over generalize and try to parameterize all possible use cases through [policies](http://en.wikipedia.org/wiki/Policy-based_design). Start concrete and generalize as needed for established use cases in the simplest way possible. More code, means more complexity, more documentation, testing, and maintenance.

Be aware that the solution is sometimes more painful than the problem. If using a library is more complex, error prone, or otherwise more burdensome than reconsider the approach. Sometimes the best we can do a simple approximation to an ideal.

### Composable

One of the halmarks of good library components is that they are composable with components in the same library and other libraries. Paying attention to how your code composes is a good way to validate your interfaces and to discover new useful components. Bad interfaces hide behind "boilerplate", "common idioms", and other polite names for glue code.

The interesting code is the code that actually does something, not the glue code. When writing a loop, factor it out and give it a name (or use an existing component instead). Figuring out how an algorithm relates to other algorithms will give you insight into how to make the interface composable.

<!-- more end -->
