---
title: Concurrency
tagline: No Raw Synchronization Primitives
mathjax: true
layout: book-page
tags: [ better-code ]
chapter: 5
---

### Motivation

_Concurrency_ is when multiple tasks start, run, and complete in overlapping time-periods and should not be confused with _parallelism_ which is when multiple tasks execute simultaneously. Parallelism requires some form of hardware support, whereas concurrency can be achieved strictly through software, such as a cooperative tasking system.

There are two primary benefits of concurrent code. The performance is the first by enabling parallelism. The second is to improve interactivity by not blocking the user while a prior action is being processed.

As clock rates on systems have stagnated, hardware developers have turned to parallelism to increase performance. Figure [xxx] shows the performance distribution on a typical desktop system. A single-threaded, non-vectorized, application can only utilize about 0.25% of the performance capabilities of the machine.

The goal of this chapter is to develop concurrent code without using raw synchronization primitives.

### Definition of _raw synchronization primitives_

A _raw synchronization primitive_ is a low-level construct used to synchronize access to data. Examples include locks and mutexes, condition variables, semaphores, atomic operations, and memory fences.

{::comment} Discuss the difference between data parallelism and task concurrency, so far this chapter is only dealing with tasking. However, it could be expanded upon. {:/comment}

### Problems of _raw synchronization primitives_

The first problem with raw synchronization primitives is that they are exceedingly error-prone to use because, by definition, they require reasoning about non-local effects.

For example, the following is a snippet from a copy-on-write[^cow_definition] data type, this is a simplified version of code from a shipping system. 

{% include code.md name='05-bad_cow' caption='Incorrect copy-on-write' %}

The code contains a subtle race condition. The `if` statement at line 16 is checking the value of an atomic count to see if it is 1. The `else` statement handles the case where it is not 1. Within the else statement the count is decremented at line 19. The problem is that if decrementing the count results in a value of 0 then the object stored in `_object` should be deleted. The code fails to check for this case, and so an object may be leaked.

The initial test to see if the count was 1 isn't sufficient, between that check and when the count is decremented and another thread may have released ownership and decremented the count leaving this object instance as the sole owner.

The fix is to test atomically with the decrement in the same statement, line 19. The correct code is shown in shown below:

{% include code.md name='05-correct_cow' caption='Correct copy-on-write' %}

The code of the complete, correct implementations is available online[^cow].

Another problem with raw synchronization primitives is that their use can have a large negative impact on system performance. To understand why, we need to understand Amdahl's Law.

The intuition behind Amdahl's Law is that if a part of the system takes time x to complete on a single core or processor, then it will encounter a speedup of y if it is run on y cores, but only if no synchronization takes places between the different cores or processors. 
$$S(N) = \frac{1}{(1-P)+\frac{P}{N}}$$
Where the speedup $$S$$ is defined by this equation. $$P$$ is hereby the amount of synchronization in the range of $$[0 .. 1]$$ and $$N$$ the number of cores or processors.

Drawing the abscissa in logarithmic scale illustrates that there is only a speedup of 20 times, even when the system is running on 2048 or more cores and just 5% synchronization takes place.

{% include figure.md name='05-amdahl_log' caption="Amdahl's law logarithmic scale" %}

Since most desktop or mobile processors have less than 64 cores, it is better to take a look at the graph with a linear scale. Each line below the diagonal represents 10% more serialization. So if the application just has 10% of serialization and it is running on 16 cores then the speed-up is just a little better than the factor of six.

{% include figure.md name='05-amdahl_lin' caption="Amdahl's law linear scale" %}

So Amdahl's law has a huge impact. Serialization doesn't mean only locking on a mutex. Serialization can mean sharing the same memory or sharing the same address bus for the memory, if it is not a NUMA architecture. Sharing the same cache line or anything that is shared within the processor starts to bend that curve down and it bends down rapidly, even an atomic bends that curve down.

The following illustrates an often used model for implementing exclusive access to an object by multiple threads:

{% include figure.md name='05-traditional_locking-1' caption="Different threads need access to single object" %}

As soon as the different threads do not only want to read the single object, but need write access as well, it is necessary to give just a single thread exclusive access. (Otherwise, undefined behavior is the result.) All other threads have to wait for their turn to get read or access. 

{% include figure.md name='05-traditional_locking-2' caption="Exclusive access with locking" %}

When the one thread does not need anymore its exclusive access, it gives its up. 

{% include figure.md name='05-traditional_locking-3' caption="Exclusive access by different threads" %}

And the next thread can get the exclusive [^access].

This is a horrible way to think about threading. The goal has to be to minimize waiting at all costs. David [^Butenhof], one of the POSIX implementors, coined the phrase that mutex should be named better bottleneck, because of the property of slowing down an application.

In the following, let's take a look at a traditional piece of code:

{% include code.md name='05-registry-0' caption='Registry example' %}

It is a registry class with a shared `set` and `get` function. The access to the underlying unordered map is protected against concurrent access with a mutex. At the first glance, it seems that only minimal work is done under the mutex. The unordered map is a fairly efficient data structure, it is a hash map. But the amount of time it takes to hash the key depends on the length of the string. So the work that is being done under the lock here is fairly unbounded. It depends completely on the lengths of the string. It may be probably typically small but it could be big. On top of the hash calculation comes a potential allocation of a new bucket within the unordered map, which in most cases requires another lock within the memory manager. This lock can be, depending on the operating system, a global lock within the process.

For a better understanding of the purpose of using locks, it is necessary to take a step back. The C++ standard states here: _It can be shown that programs that correctly use mutexes and memory\_order\_seq\_cst operations to prevent all data races and use no other synchronization operations behave as if the operations executed by their constituent threads were simply interleaved, with each value computation of an object being taken from the last side effect on that object in that interleaving. This is normally referred to as ‘sequential consistency.’_, C++11 Standard 1.10.21.

So why is this an important sentence? It means that one can always think about mutexes as if one has some set of interleaved operations. 

{% include figure.md name='05-sequential_operations' caption='Sequential operations' %}

* A mutex serializes a set of operations, $$Op_n$$, where the operation is the code executed while the mutex is locked
* Operations are interleaved and may be executed in any order and may be repeated
* Each operation takes an argument, $$x$$, which is the set of all objects mutated under all operations
  * $$x$$ may not be safely read or written without holding the lock if it may be modified by a task holding the lock
* Each operation may yield a result, $$r_m$$, which can communicate information about the state of $$x$$ while it’s associated operation was executed
* The same is true of all atomic operations

So there is not a lot of difference between a `std::atomic`. In fact there, is a call on `std::atomic` that returns `true`, if it is lock-free. This means the processor supports to do that as an atomic item within the processor or is there, not processor support and the compiler has to generate a mutex pair to lock, make the change on the atomic operation, and do the unlock. So all that mutexes and locks are the way to construct atomic operations. 

That means that any piece of code that has a mutex can be transformed into a queued model. This idea applied to the registry example from above leads to this:

{% include code.md name='05-registry-1' caption='Registry with queue' %}

Given that there is a serial queue `_q` with an `async` function which executes the passed item and it uses the same calling conventions as `std::async`. Hereby with the difference that it guarantees that the next item being processed in that queue doesn't start until the previous one is completed. Then one can rewrite the `set` string function to be executed with `_q.async`.
As well one can rewrite the `get` string operation. But here the difference is, that one needs the result back out, paired with that particular `get`. It is realized here with a future. (Futures will be covered later in more detail.) So the result of the `get` function, e.g. with a continuation, can be used whenever the `key` is hashed and the lookup in the hash is completed.

{% include code.md name='05-registry-2' caption='Enhanced registry with queue' %}

Why is it important to understand this concept? Because at any place with a mutex in the code one can always make this transformation. One can always transform it into a serialized queue model. And this means that within the serialized queue model anytime somebody can come along and calls `set`, regardless of the amount of work that `set` takes, the time it takes for `set` to return to the caller itself constant. This means as well that one can add something like an arbitrary `set`, e.g a whole vector of key-value pairs. And to the caller, this `set` will take just as much time as the previous `set`. It's a non-blocking operation with an upper bound of overhead. 

### Problems of _raw threads_

A _thread_ is an execution environment consisting of a stack and processor state running in parallel to other threads.
A _task_ is a unit of work, often a function, to be executed on a thread.

Another common scenario is that increased work within an application is outsourced to a spawned background thread with the intent that the available CPU cores are better utilized. 

{% include figure.md name='05-background_thread' caption='Background thread executing tasks' %}

Since this is recognized as a successful idiom to solve performance problems of an application, it becomes easy the default way to solve such issues. 

{% include code.md name='05-background_worker' caption='Simple background worker' %}

Over time the application gets enhanced with more modules and plugins. When now for each of these the same idea was applied then the complete application uses a huge number of threads.
An over-subscription of threads is then easily the case. That means that more threads are used than CPU cores are available. So the kernel of the operating system has to constantly switch the threads between the available cores to prevent starvation of single threads. 
Within such a switch - named context switch - the CPU registers, program counter, and stack pointer of the old thread are saved and the ones from the new thread need to be restored. This saving and restoring take time that is lost for computational tasks of an application. Besides this, the translation lookaside buffer (TLB) must be flushed and the page table of the next process is loaded. The flushing of the TLB causes that the memory access of the new thread is slower in the beginning. This causes an additional slow down.
So the goal has to be that the number of context switches is as low as possible.

One way to archive this goal is to use a task system. A task system uses a set of threads, normally equal to the number of CPU-cores and distributes the submitted tasks over the available threads. In case that more tasks are submitted than free threads are available then they are put into a queue and whenever one is done the next task is taken from the queue and executed.

{% include figure.md name='05-simple_tasking_system' caption='Simple task system' %}

Since the number of threads is constant, ideally there is no need to perform any context switches. (Because of simplification reasons a fact is here ignored that other system services have running threads as well so there are happening context switches in any case.) A task system within an application is an appropriate measure to reduce the number of context switches as long as all modules within it use the same instance of the task system.

To illustrate the purpose and gaining a better understanding of the implications within such a task system, its code is developed in the following. 

The [figure](#05-simple_tasking_system) above shows that the task system consist out of a notification queue:

{% include code.md name='05-notification_queue-1' caption='Notification queue class' %}

This notification queue consists of a `deque` of `std::function` with a `mutex` and a `condition_variable`. It has a `pop()` operation which is just going to pull one item off of the queue. And it has a `push()` operation to push one item into the queue and notify anybody who might be waiting on the queue.

{% include code.md name='05-task_system-1' caption='Task system class' %}

The task system has a `_count` member which is set to the number of available cores. The system has a vector of threads and the notification queue. The `run()` function is the function that will be executed by the threads. Inside that function is an empty function object. As soon as an item is available in the queue, it pops it from the queue and executes it, and tries to pick the next one.
The constructor of the task system spins up as many threads as there are cores. Each thread is bound with a lambda against the `run()` function.
When the task system gets destructed, it is necessary to join all threads. The function that is used by the outside is `async`. It takes the function object and pushes it into the queue.
This system is so far very primitive, e.g. it would hang on destruction. The latter is corrected by the following additions:

{% include code.md name='05-notification_queue-2' caption='Notification queue with done switch' %}

So with the new `done()` function the new member `_done` is set and the queue is notified about the change. In case the code is waiting in the `pop()` function, it is woken up from the condition variable and it is checked if `_done` is set and then returns `false`.

{% include code.md name='05-task_system-2' caption='Non-blocking task system on destruction' %}

The task system notifies within the destructor all queues to ignore all potentially remaining entries which allow that the threads can be joined without delay. (With C++20 this could be enhanced with `jthread`s.) 

This task system performs very badly compared to MacOS' Grand Central Dispatch (GCD) for instance. It just has a throughput of about 6%. Why does this system perform so badly even that this is the recommended design at several places? 
This design follows principle from the above [figure](#05-traditional_locking-2). It has a single queue and a bunch of threads. These are banging on that queue and so the threads are waiting often on the mutex. 

Unfortunately, it is not possible to transform this model as it is described above because all that is there is a queue. So a different approach is needed.

{% include figure.md name='05-task_system_multiple_queues' caption='Task system with multiple notification queues' %}

A way to reduce the contention on this single queue is to change the task system in a way that each thread has its individual queue.

{% include code.md name='05-task_system-3' caption='Task system with multiple queues' %}

The task system has now as many queues as threads and the `run()` function gets the parameter index for its corresponding queue so that it can pick its belonging items.
On destruction, the task system has now to notify all queues about to end their work.
A continuously incremented atomic integer modulo the number of queues is used within the `async()` function to distribute the tasks in a round-robin manner over the queues. An atomic member is used to enable that this function can be used from multiple threads.

This system now performs about as twice as fast compared to the previous approach. But this new way has still two problems: A long-running task will block the execution of all other tasks which are behind this one in the queue even that the queues of other cores went dry. And on a fully loaded system, there is a fair amount of contention on the mutex of a queue.

These problems can be minimized by using the mechanism of task stealing.

{% include figure.md name='05-task_system_task_stealing' caption='Task System with Task Stealing' %}

There are different sophisticated, highly optimized approaches in implementing task stealing. Here is take a very simple strategy. 

{% include code.md name='05-notification_queue-3' caption='Notification queue with try_pop and try_push' %}

The queue is enhanced by two new functions, `try_pop()` and `try_push()`.
The `try_pop()` function return `false` when the attempt fails to get the lock on the mutex with the additional `try_to_lock` property. This can be the case when either another thread currently pushes or pops an item into or from the queue. The other possibility of a `false` result is when the queue is empty.
Similar applies for the `try_push()` function. 
The important difference is that a thread using one of these two functions does never stop on a blocked mutex!

{% include code.md name='05-task_system-4' caption='Task system with task stealing' %}

The code tries within the `run()` function to pop an item from its corresponding queue by calling `try_pop()`. This can either fail because that queue is currently busy or empty. In both cases, the code tries to steal a task from a different thread until it has checked for all other queues. If there are no tasks to execute, then it calls for a blocking `pop()` and it is woken up whenever there is more work to do.
The same approach is taken for pushing an item into the queue in the `async()` function with the difference that the code spins some time over every queue until it finds one to push the task to. The spinning is done to lower the probability that the calling thread is getting stuck on the later finally implemented `push()` call.

The task system reaches now about 85% of the performance of the reference implementation.

So the first goal, reducing the number of arbitrary threads is fulfilled; the number of context switches can be minimized by using a task system. But as soon as every single application on a machine uses its instance of a task system, there is again the problem of over-subscription because each instance would start as many threads as there are cores.
Such a task system with a fixed number of threads has another problem. The risk is there of dead-locks.

{% include figure.md name='05-dead_lock' caption='Dead lock within queued tasks' %}

As soon as a task `a` creates a new task `b` and the progress of `a` depends on the result of task `b` and task `b` got stuck in the queue behind `a` then the system is in a dead-lock. Figure [](#05-dead_lock) illustrates the problem just with a single queue. But the same problem is there with multiple queues and depending task get stuck behind other tasks that are blocked because they are waiting for getting a lock on a mutex or waiting for another result.

So the only solution to reduce the problem of having an unbound number of threads and the probability of dead-locks because of depending tasks is that all applications within a system use the same task system. Only a task system on OS's kernel-level knows about threads that currently don't make progress and can spawn new threads to prevent a dead-lock situation.
MacOS and Windows e.g. provide here out of the box a task system through a low-level API.


{::comment}
Shall call backs be discussed here? Technically they don't introduce a problem. But from the point of view of maintainability it is one because the control flow is hard to understand.
{:/comment}

### Problems of call backs
* Contra: Hard dto reason about the flow in the code
* Contra: Callback must be set beforehand, futures can be attached later
* Pro: Can be faster than futures because no overhead of shared state counter


#### Futures as abstraction

Futures
Conceptually, a future is a way to separate the result of a function from the execution of the function. The task (the function packaged so it returns void) can be executed in a different context (the execution context is controlled by executors in some of the proposals) and the result will become available via the future.

A future also serves as a handle to the associated task, and may provide some operation to control the task.

The primary advantage of a future over a simple callback is that a callback requires you to provide the subsequent operation in advance. Where a future allows you to provide a continuation, via a then() member function, at some later point. This feature makes futures easier to compose, easier to integrate into an existing system, and more powerful as they can be stored and the continuation can be attached as the result of another action, later. However, this flexibility comes with the inherent cost, it requires an atomic test when the continuation is attached to determine if the value is already available. Because of this cost, for many library operations, it makes sense to provide a form taking a callback as well as one returning a future. Although at first glance it may appear, that a callback from is easily adapted to a future form, that is not the case for reasons discussed below.

#### Channels (or actors) as abstraction




### Motivation

1st example export from ui with compression and possibility to cancel
2nd example export group of images with compression and possibility to cancel

### Develop Solution

1st solution Use futures 
2nd solution Use channels

### Conclusion

[^cow_definition]:
    Copy-on-write [https://en.wikipedia.org/wiki/Copy-on-write](https://en.wikipedia.org/wiki/Copy-on-write)

[^cow]:
    Copy-on-write implementation in stlab. [https://github.com/stlab/libraries/blob/develop/stlab/copy_on_write.hpp](https://github.com/stlab/libraries/blob/develop/stlab/copy_on_write.hpp)

[^butenhof]:
    Recursive mutexes by David Butenhof [http://zaval.org/resources/library/butenhof1.html](Recursive mutexes by David Butenhof)