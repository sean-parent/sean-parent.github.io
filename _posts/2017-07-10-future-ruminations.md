---
layout: post
title: Future Ruminations
description: ""
category:
tags: [future, regular, efficient, basis]
---
## Background

This post is a lengthy answer to a question from Alisdair Meredith:

<blockquote class="twitter-tweet tw-align-center" data-lang="en"><p lang="en" dir="ltr">How do we know if the primitives are adequate until we know the feature set that must be supported?</p>&mdash; Alisdair Meredith (@AlisdairMered) <a href="https://twitter.com/AlisdairMered/status/884399387954360323">July 10, 2017</a></blockquote>
<script async src="//platform.twitter.com/widgets.js" charset="utf-8"></script>

The question is regarding the numerous proposals for a better `future` class template for C++, including the [proposal from Felix Petriconi, David Sankel, and myself](http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0676r0.pdf).

It is a valid question for any endeavor. To answer it, we need to define what we mean by a `future` so we can place bounds on the solution. We also need to understand the problems that a `future` is trying to solve, so we can determine if a `future` is, in fact, a useful construct for solving those problems.

The [proposal](http://open-std.org/JTC1/SC22/WG21/docs/papers/2017/p0676r0.pdf) started with me trying to solve a fairly concrete problem; how to take a large, heavily threaded application, and make it run in a single threaded environment (specifically, compiled to asm.js with the Emscripten compiler) but also be able to scale to devices with many cores. I found the current standard and boost implementation of futures to be lacking. I [open sourced my work on a better solution](http://stlab.cc/libraries/concurrency/index.html), and discussed this in my _[Better Code: Concurrency](/papers-and-presentations#title-better-code-concurrency)_ talk. Felix heard my [CppCast interview](http://cppcast.com/2015/06/sean-parent/) on the topic, and became the primary contributor to the project.

## Futures

Conceptually, a _future_ is a way to separate the result of a function from the execution of the function. The _task_ (the function packaged so it returns void) can be executed in a different _context_ (the execution context is controlled by _executors_ in some of the proposals) and the result will become available via the future.

A future also serves as a handle to the associated task, and may provide some operation to control the task.

The primary advantage of a future over a simple callback is that a callback requires you to provide the subsequent operation in advance. Where a future allows you to provide a _continuation_, via a `then()` member function, at some later point. This feature makes futures easier to compose, easier to integrate into an existing system, and more powerful as they can be stored and the _continuation_ can be attached as the result of another action, later. However, this flexibility comes with inherent cost, it requires an atomic test when the continuation is attached to determine if the value is already available. Because of this cost, for many library operations it makes sense to provide a form taking a callback as well as one returning a future. Although at first glance it may appear that a callback from is easily adapted to a future form, that is not the case for reasons discussed below.

## Design Issues

Because a future is a token for a type T, we can look at the regular properties of types in evaluating if we have an efficient basis for a future. In order to be a reasonable proxy for a value of type T, we must be able to pass a future to a function, return it from a function, and store it as the member of an object. But should a future be copyable if T is copyable? One thought is that copying should not be handled specially, and should be done on the value of T within a continuation. To illustrate, here is an initial attempt to split a value using the current WG21 proposal for non-copyable futures.

```cpp
// 'split' fut to call two functions, f & g
auto a = fut.then([](auto x){
    auto value = x.get();
    return make_pair(f(value), g(value));
});
```

Immediately we can see the problem with the approach. Even though f and g are independent operations, they will be serialized and the result will not be available until both f and g are complete. If we want to continue calculations down separate paths, those paths will continue to be packaged together.

A better option is to write a form of copy to _split_ or fork the future. Here is a simple implementation (which ignores exception for illustrative purposes only):

```cpp
template <class T>
auto split(future<T>& x) {
    auto tmp = std::move(x);
    promise<T> p;
    x = p.get_future(); // replace x with new future
    return tmp.then([_p = move(p)](auto _tmp) mutable {
        auto value = _tmp.get();
        _p.set_value(value); // assign to new "x" future
        return value; // return value through future result
    });
}
```

This can then be called as:

```cpp
// 'split' fut to call two functions, f & g
auto a = split(fut).then([](auto x) { return f(x); });
auto b = fut.then([](auto x){ return g(x); });
```

Using split() we can fan out our parallel computations. However, split has a significant disadvantage over a copy, it requires that the argument being split is mutable. Being able to copy immutable objects is a fundamental building block for reasoning about parallel computations, having `split()` be our only way to copy means that copying larger constructs containing futures requires additional synchronization.

Another approach is to introduce a new type such as `shared_future` (in my opinion this is a horrible misnomer). It is a potential option if implementing copy, even when not called, requires additional overhead. I do not know that this has been demonstrated (and do not believe it to be true). An approach more inline with the current language is to optimize for the rvalue case to provide performance.

There may well be good reasons to have a _concept_ for a future with many type models. And then a type erased `any_future<>` similar to the relationship between function objects and `std::function<>`. However such type erasure comes at a cost, and it needs to be demonstrated that there are enough useful models of a _Future_ concept and potential optimization benefits to have them be distinct types to warrant such an approach. I do not believe a copy basis operation rises to this level.

So for basis operations on a future we have:

* default ctor
* construct with a value
* construct with associated task
* move ctor and move assignment
* copy ctor and copy assignment
* equality/inequality

For the later, two futures are equal if they refer to the same shared state (copies are equal).

## Future as Task Handle

If the result of the task is no longer necessary the task can be _canceled_, and if it hasn't yet started to execute then it can be destructed releasing all subordinate resources. This is a way to provide RAAII to computation, frequently the most valuable resource in the machine. The _progress_ of a task may be made available, specifically if it is _complete_ (the future is _ready_), if it has _started_, and this attribute could be tracked through subordinate tasks to determine an estimate towards completion. Finally, the _context_ within which a task is to be executed may be changed, the most common change is to _promote_ a task to execute with a higher priority or immediately within the current execution context.

## Getting Value from Future

Since a future is a token for a value, there needs to be some mechanism to retrieve the value. Starting with C++11, futures have a `get()` function and various forms of `wait()` functions to retrieve the value. However, both `get()` and `wait()` are very problematic operations. Unless the associated task can be promoted to immediate execution, these operations may deadlock if the associated task is unable to run concurrently with the waiting task. This can happen for numerous reasons:

* The system doesn't have thread support and tasks are executed on a main serial queue.
* The tasks are scheduled within a thread pool and all threads within the pool are similarly blocked on `get()` or a condition variable.
* The number of available threads within the system is exhausted.

In order to promote a task, the task must be fully _resolved_, all of it's arguments are known, if the tasks is required to be serialized with other tasks then the system must assure that all prior tasks, and the task cannot require execution within a context other than the current context.

These guarantees can be provided by `std::async()` which is why it is permissible to implement `std::async()` with the `std::launch::async` policy on a thread pool by implementing task promotion and promotion is the mechanism by which `std::launch::deferred` tasks work (_deferred_ can be viewed as promoting from _execute never_ to _execute immediately_). However, neither of this is implementable through the `std::promise` mechanism because `std::promise` does not provide a task handle and so has no way to implement promotion.

In the presence of continuations, `get()` is more problematic because the task associated with a continuation future is not resolved. Promotion of such a task would require first promoting all upstream tasks. This is why many of the proposals, including ours, for better futures with continuations remove the `get()` and `wait()` operations. Even if implementable, they may be undesirable (see discussion on why context matters).

## Cancellation

Another question is what should happen when the future destructs? C++11 has three answers:

* Futures returned from `std::async` with `std::launch::async` will block (wait) on destruction until the task completes.

* Futures returned from `std::async` with `std::launch::deferred` will cancel the associated task and free the resources.

* Future obtained from an `std::promise` will _detach_ the associated task, allowing it to run to completion but drop any result.

If we bias our design towards functions whose behavior is not defined by their side-effects then the option of canceling on destruction makes the most sense as a default behavior. A task may be canceled even if it is not resolved, so cancellation of a future associated with a continuation has the effect of "unraveling" the dependency graph through associated tasks. However, for this to be the default behavior, the default mechanism to create a `future` should not be through something like `std::promise` which cannot act as a task handle, but rather should be a function that splits a function into a task and future.

```cpp
template <class Sig, class F>
auto package(F&& f) -> std::pair<task_t<Sig, F>, future<task_result_t<Sig, F>>
```

Often cancellation is discussed as being a mechanism which is separate from futures. Usually with some handwaving "just have a shared atomic flag you check for cancellation." The problem with such an approach is that in order to cancel a task, you have to know if the result of the task is needed or not. In the presence of splits or copies, that information cannot be known locally. In a large system, you do not know what to cancel, only what is not needed locally. Consider an animation system which is generating a stream of futures for frames. If a frame is not ready on time it is canceled (dropped), but any calculations for that frame which are needed by subsequent frames should continue.

Because `std::promise` does not provide a task handle, there is no way to implement cancellation or blocking behaviors with just `std::promise` as a primitive. Composing an `std::launch::deferred` task with a promise it is possible to build a task that cancels on destruction. This `cancelable_task()` function is called in a similar manner to `package()` defined above.

```cpp
template <class Sig, class F>
auto cancelable_task(F&& f) {
    using shared_t = std::packaged_task<Sig>;

    auto p = std::make_shared<shared_t>(std::forward<F>(f));
    auto r = p->get_future();

    return std::make_pair([_p = std::weak_ptr<shared_t>(p)] (auto&&... args) {
        if (auto p = _p.lock()) (*p)(std::forward<decltype(args)>(args)...);
    },
    std::async(std::launch::deferred, [_p = std::move(p), _r = std::move(r)] () mutable {
        return _r.get();
    }));
}
```

The problem of `promise` not providing a task handle is why you cannot simply adapt a callback based interface into one returning a future by providing a `promise` in the callback. Although callbacks can be more efficient in some cases, they do not provide a cancellation mechanism and the transformation from "asynchronous function with callback" to "cancelable task with future" is more complicated.

## Context Matters

The context within which a task is executed may be of significance for a number of reasons including:

* Side-effects which need to be serialized with other tasks (part of a sequential process).
* Interruption of a time critical system.
* Access to specific resources (such as access to thread local variable on a specific thread or access to hardware resources such as a GPU).

A common design for futures is to default to _immediate_ (or inline) execution for continuations and consider execution context as something that can always be "composed in" later. The problem with this is that immediate execution on a continuation will execute either in the context of the associated task, or in the context of the future. In the case of an asynchronous task, the exact context will be determined by execution performance. In practice, for a given piece of code on a given machine, it will _almost_ always execute in one of those contexts, which is determined by the latency between the asynchronous invocation and when the continuation is attached.

My experience in working with teams using such a mechanism, and even in discussion with experts, is that they will make assumptions about which of the two contexts the code will execute in, and introduce race conditions. Even in the absence of a direct race condition they may inadvertently introduce a performance issue by interrupting a time critical system with a long task. For example, let's say that I execute expensive operation as continuation. The continuation is attached within the main event loop of an application, in testing, the continuation always executes in the context of the associated task and there are no performance problems. After the application is shipped, new hardware, or a new OS version, is released which changes the timing of the asynchronous operation so now it complets before the continuation is attached. Suddenly the continuation is executed in the context of the main event loop, and delays processing of user events causing the interactive performance of the system to become unusable.

For someone coming from a background where they have used promises in a language such as ES6, where continuations are always queued, this can be very surprising behavior.

In practice, immediate execution is nearly always a poor default and is of value only when the continuation is "small" (will execute quickly) and has no side effects or external dependencies. It is better to default to execute the task inside of a task system (such as a task stealing thread pool) and force the developer to explicitly choose immediate execution. Having a bad default forces a lot of code like this:

```cpp
auto a = fut.then([](auto x){
    return async(default_executor, [_x = x.get()]{ f(_x); });
});
```

Which could otherwise just be:

```cpp
auto a = fut.then(f);
```

If the default is not immediate execution, then there must be a way to override the default, otherwise there is no way to obtain immediate execution.

I do not believe that the issue of context (executors) can be meaningfully separated from futures with continuations.

Unfortunately, this is not a universal answer, there are problems where futures are useful, which have nothing to do with asynchronous development and you want a fast implementation that doesn't require synchronization always executes continuations immediately. A channel as a stream of futures for connecting co-routines as communicating sequential processes is one possible example. As of yet, I don't have enough experience with implementing this to fully understand the impact on the design of futures.

## Conclusions

There is a lot of industry experience with promise/future systems and they have become popular because they allow fairly straightforward transformation of sequential code into asynchronous code without needing to provide explicit synchronization. With continuations, futures can scale from single threaded environments to large grid systems with very high performance. This makes them an attractive basis upon which to build. By viewing futures as a token for a value of type T and as a handle for an associated task, we have a reasonable framework to explore a bounded feature set and make the engineering tradeoffs between performance and capabilities. In general, we shouldn't drop a capability unless there are no compelling use cases, or there is a demonstrable performance implication, and in the later case the performance implications must be weighed against the value of the use cases and if there is reasonable alternative implementations for those use cases.
