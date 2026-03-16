---
title: Preface
layout: book-page
tags: [ better-code ]
chapter: 0
---

To understand what _better code_ is, we first need to understand what _good code_ is. Students are taught that good code is code that does what the specification says it should. But such an answer begs the question of what is a good specification? Nearly every experienced developer I've met has a snippet of code filed away that has profound beauty - it likely has no corresponding specification and may not even contain a single comment. So what is good code?

By working through this book and applying the ideas within, I hope that you will gain a deeper understanding of what good code is, and in striving to write good code, you will write better code.

This book presents a collection of software development goals. The word _goal_ was chosen carefully. These are not guidelines or rules, and achieving the goal is not always simple or straightforward. Each goal is phrased such that it is not prescriptive and often states what not to do, but what _to do_ is an open-ended challenge. You will find that while trying to apply these goals, you will sometimes fail, but through the effort, you will gain a deeper understanding of your code and learn to write _better code_.

The examples in this book are primarily in C++ for two reasons. The majority of my professional career has been spent writing C++ code. And C++ is the best mainstream language to present the ideas within, but I will also point out some significant shortcomings. The ideas in this book are not limited to the C++ developer but apply equally to other languages. Where I think there is value in illustrating this point, I provide examples in other languages. <!-- REVISIT: It may just be JavaScript as the other language, as yet undecided -->

<!--

Format for each chapter:

a short introduction that provides motivation
explain the problem
    show examples
define the goal
present possible solutions
concluding remarks

-->

<!-- Test of mermaid, remove the `display:none` attribute to enable. -->

```mermaid
graph TD
A[Client] --> B[Load Balancer]
    B --> C[Server1]
    B --> D[Server2]
```{: style="display:none"}
