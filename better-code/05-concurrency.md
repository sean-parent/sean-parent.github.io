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

### Definition of _raw synchronization primitives_.
