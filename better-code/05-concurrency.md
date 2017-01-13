---
title: Concurrency
tagline: No Raw Synchronization Primitives

layout: book-page
tags: [ better-code ]
---

### Motivation

{::comment}
For this section I need to first provide motivation for concurrency, and define concurrency and parallelism. which are not commonly understood. Do I need to provide a motivation section for each chapter?
{:/comment}

_Concurrency_ is when multiple tasks start, run, and complete in overlapping time periods and should not be confused with _parallelism_ which is when multiple tasks execute simultaneously. Parallelism requires some form of hardware support, where as concurrency can be achieved strictly through software, such as a cooperative tasking system.

There are two primary benefits for concurrent code. The first is performance by enabling parallelism. The second is to improve interactivity by not blocking the user while a prior action is being processed.

As clockrates on systems have stagnated, hardware developers have turned to parallelism to increase performace. Figure [xxx] shows the performance distribution on a typical desktop system. A single threaded, non-vectorized, application can only utilize about 0.25% of the performance capabilities of the machine.

### Definition of _raw synchronization primitives_.

A _raw synchronization primitive_ is a low level construct used to synchronize access to data. Examples include locks and mutexes, condition variables, semephors, atomic operations, and memory fences.

{::comment} Discuss difference between data parallelism and task concurrency, so far this chapter is only dealing with tasking. However, it could be expanded upon. {:/comment}

The goal of this chapter is to develop concurrent code without using raw synchronization primitives.

The first problem with raw synchronization primitives are that they are exceedingly error prone to use because, by defintion, they require reasoning about non-local effects.

For example, [xxxx] is a snippet from a copy-on-write datatype, this is a simplified version of code from a shipping system.

{::comment}
Insert bad cow example here. Can this example be simplified even more? Remove the template and make it a string?
{:/comment}

The highlighted lines {::comment} how? {:/comment} contain a subtle race condition. The `if` statement is checking the value of an atomic count to see if it is 1. The else statement handles the case where it is not 1. Within the else statement the count is decremented. The problem is that if decrementing the count results in a value of 0 then the object should stored in object_m should be deleted. The code fails to check for this case, and so an object may be leaked.

The initial test to see if the count was 1 isn't sufficient, between that check and when the count is decremented another thread may have released ownership and decremented the count leaving this object instance as the sole owner.

The fix is to test atomically with the decrement, the correct code is shown in [xxxx].

Another problem with raw synchronization primitives is that their use can have a large negative impact on system performance. To understand why, we need to understand Amdahl's Law.

The intuition behind Amdah's Law is that if part of system takes time x to complete,

{::comment}
Math experiment for Fibonacci matrix.
{:/comment}
$$
\begin{align*}
  \left[ \begin{array}{cc}
      1 & 1 \\
      1 & 0
    \end{array} \right]^{n} =
    \left[ \begin{array}{cc}
      F_{n+1} & F_n \\
      F_n & F_{n-1}
    \end{array} \right]
\end{align*}
$$

