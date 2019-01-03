---
layout: post
title: '"Modern" C++ Ruminations'
description: ""
category: draft
tags: [efficient, loops]
---

This post is a response for a number of people who have asked me to give my 2&cent; to a [large Twitter thread](https://twitter.com/aras_p/status/1076947443823136768), and [post](http://aras-p.info/blog/2018/12/28/Modern-C-Lamentations/) by Aras Pranckevičius, that is rooted in a [post by Eric Niebler](http://ericniebler.com/2018/12/05/standard-ranges/) regarding C++20 standard ranges.

This post addresses points made directly by Aras's post, but also some of the Twitter sentiment reflected in the thread. There are far too many comments to try and address them all directly.

The first two main points from Aras's post are:

1. C++ compile times are important.
2. Non-optimized build performance is important.

These points are true, and are a valid issue with C++. I've raised [related issues](https://www.youtube.com/watch?v=iGenpw2NeKQ) regarding the language direction. But the degree to which these particular issues are valued largely depends on your style of coding and what you are working on. I'm probably one of the more extreme counter examples. I learned to write code in a notebook a few years before I ever saw a computer. I don't see the point in even trying to compile until I believe the code for the task I'm working on is correct and complete. So I will write, and rewrite code often for a couple of weeks before I even attempt to compile it. When I'm working on optimizations, even for interactive optimizations, I tend to instrument, gather data, think (a lot), and then write code to solve the issue. Where build times do have a significant impact is when I'm trying to land a large change (and since I just spent a couple of weeks dark my changes are more likely to be large). I end up stuck iterating on a merge and waiting for a large build on many platforms.

I've complained more than once about things like STL debug implementations that make O(1) operations into O(N) operations - these can make other algorithms go quadratic and a debug build becomes unusable (Microsoft use to ship with several such constructs). I'm certainly more forgiving for performance of debug builds than Aras, but when I am working on something that is very interactive it can be a pain to have to do everything with an optimized build.

I do appreciate that in the games domain these items are of higher importance, and I know many on my own team who would prefer to work in a rapid edit/compile/run iterative process. I've taken to writing a fair amount of code in [Jupyter Lab](https://blog.jupyter.org/interactive-workflows-for-c-with-jupyter-fe9b54227d92) because it provides a way for me to vet ideas and document my thought process.

Aras's third point, that cognitive load is important, I agree with in principal but my solution is to break code down into small functions that can be reasoned about (and proven) locally and to write all code as if it were a library. This is why my ["no raw loops" talk](https://sean-parent.stlab.cc/papers-and-presentations/#c-seasoning) has struck a cord. But a lot of people who tout no-raw-loops, either didn't watch the talk or missed much of the point. I state that a lambda function that is more complex than a simple composition of two functions (i.e. `f(g(x))`), should be given a name. Composition through nesting calls can be very problematic to reason about. You want simple straight line code. And using an algorithm should not make your code worse - if it does, write the raw loop and add a comment explaining the choice. Maybe revisit in the future to find a better solution.

To borrow an [example from Dave Abrahams](https://developer.apple.com/videos/play/wwdc2018/223/), consider removing all the selected elements from a container. Here is a simple raw loop to do just that.

```cpp
if (auto n = a.size()) do {
    --n;
    if (a[n].selected()) a.erase(begin(a) + n);
} while (n);
```
The cognitive load to reason about this code is very high and it is just a simple loop. When I see a loop in a code review I need to determine:

1. What are the preconditions for entering the loop?
    1. How are they guaranteed?
1. What are the loop invariants?
1. What is the base complexity for the loop, ignoring cost of operations performed in the loop.
    1. What is the complexity of every operation in the loop and the relation to the number of times the loop is iterated?
1. What are the postconditions of the loop?

Read through the above loop and answer those questions. I go through this exercise everytime I read or write a loop. Now consider this version:

```cpp
auto p = remove_if(begin(a), end(a), [](const auto& e) {
    return e.selected();
});
a.erase(p, end(a));
```

This code works with any sequence container (the first version does not), it works even if the container is empty (with no additional check), and it works in linear time (the first version is quadratic). And I know all of that at a glance. The only thing I need to consider is the cost of `e.selected()`.But I do have to know what `remove_if()` does, what it returns, and what the complexity is. But if I don't know, I can look it up.

As an exercise rewrite the above with a raw loop to solve these issues and prove your solution is correct. How long did that take?

Pulling in `<algorithm>` slows my compile and adds another layer of debugging, especially if I'm trying to debug my lambda expression. So the benefit isn't free, but it also saved me considerable time.

That brings us to Eric's example of generating Pythagorean triples. This is just a bad example, much like [the Fibonacci example used for many concurrency talks](https://youtu.be/zULU6Hhp42w?t=2600). Looking at both Eric's code and Aras's code, try and determine the complexity. You need a fair amount of number theory to do so. As with Fibonacci numbers, primitive Pythagorean triples can be generated efficiently using linear algebra. Pythagorean triples are related to Fibonacci numbers. A fast implementation wouldn't look like either solution.

There are a few statements that Aras made that trouble me:

> C++ way of creating lambdas, and the choice of C++ standard to make things look clever (“what’s an iota? it’s a Greek letter, look how smart I am!“) are both a bit cumbersome.

> Cristen Ericson has put it nicely [here](https://twitter.com/ChristerEricson/status/1078095388244996096) "Goal of programmers is to ship, on time, on budget."

>Some of the “all feedback will be ignored unless it comes in form of a paper presented at a C++ committee meeting” replies that I’ve seen in the past few days sound to me like a not very productive approach.

The name `iota()` was borrowed from APL. Ken Iverson's ideas had a significant influence on the development of STL, and our profession as a whole. That is why he has a Turing award. The "&iota;" symbol was likely chosen because of its use to represent an inclusion map in mathematics.

Programming is a profession. It is an ethical obligation to work to improve our profession. The more senior and talented you are, the more you owe to the community. Giving back can take many forms; mentoring, lecturing, publishing, serving on committees, and furthering open source projects. Part of that obligation is to continue to study, to read papers and work through books. Not knowing the history of `iota()` should not be something to be proud of, but an embarrassment.

The C++ standards committee is filled with people with diverse interests. Some who make their living teaching the language and so want to be informed, some who represent companies and the interests of those companies, some from academia who are seeking publication, grants, and tenure. I don't know of anyone on the committee who is primarily there to just collect feedback from the users of the language and try to incorporate it, except possibly Bjarne. The fact that many do this regardless, is out of their own sense of professionalism. If you want a stronger say in the future of the language, you will have to sit at the table.

A surprising amount of code that I've written in my career is still actively used (not always for the better). Code I never intended to be part of a library is now used by many applications and in environments I never foresaw. A goal for a programmer has to be to look beyond the product they are shipping and recognize their obligation to create correct and efficient solutions and understand that their code may well endure, for good or bad.

I can agree with the stated goals of improving compilation time and debug run times. However, I also want to save time researching, designing, proving, reviewing, testing, and reading code. The range library is a pretty powerful tool for the later points. And yes, you can write horrible code with the range library and it will requires some level of study to use well. As with mathematics, there is no royal road.
