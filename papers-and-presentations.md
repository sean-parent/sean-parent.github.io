---
title: Papers and Presentations
permalink: /papers-and-presentations/
tagline: Too many projects, too little time
layout: page
comments: true
---

#### Contents
{:.no_toc}
* contents
{:toc}

***

## Presentations

***

Date: 2019-10-31, 2019-09-19

### Better Code: Relationships

Abstract: Computer scientists are bad at relationships. Nearly every program crash is rooted in a mismanaged relationship, yet we spend most of our time discussing types and functions and not the relationships connecting them together. This talk looks at common ways data and code are connected in an application, how those relationships are typically represented, and the problems caused by the use, and misuse of these paradigms. Then we'll look at ways to model these relationships in C++ and use them to build correct applications.

Note: Talk presented at [C++ Russia](https://cppconf.ru/en/), 2019-10-31

*   [PDF](presentations/2019-10-31-relationships/2019-10-31-relationships.pdf)
*   [Keynote](presentations/2019-10-31-relationships/2019-10-31-relationships.key)

Note: Talk presented at [CppCon 2019](https://cppcon.org/), 2019-09-19.

*   [Video](https://www.youtube.com/watch?v=ejF6qqohp3M)
*   [PDF](presentations/2019-09-19-relationships/2019-09-19-relationships.pdf)
*   [Keynote](presentations/2019-09-19-relationships/2019-09-19-relationships.key)

Errata: In the video, the slide for erasing an element from the registry contains a bug. It fails to check if the element has already been erased. Fixed in the posted slides.

***

Date: 2019-10-31

### Compose This!

Abstract: Ruminations on composability, why functional composition is inherently limited, and why we need a better theory of composition.

Note: Lightning talk presented at [C++ Russia](https://cppconf.ru/en/), 2019-10-31

*   [PDF](presentations/2019-10-31-compositional-efficiency/2019-10-31-compositional-efficiency.pdf)
*   [Keynote](presentations/2019-10-31-compositional-efficiency/2019-10-31-compositional-efficiency.key)

***

Date: 2019-03-20

### Bringing Photoshop to the iPad

Abstract: A "behind the scenes" look at the challenges to bring one of Adobe's premier products to mobile devices and the role that C++ developments have played in overcoming them.

Note: Talk presented at the [Northwest C++ Users' Group](https://nwcpp.org/March-2019.html), 2019-03-20.

*   [Video](https://www.youtube.com/watch?v=oQUtu_vD91g)
*   [PDF](presentations/2019-03-20-photoshop-on-ipad/2019-03-20-photoshop-on-ipad.pdf)
*   [Keynote](presentations/2019-03-20-photoshop-on-ipad/2019-03-20-photoshop-on-ipad.key)

***

Date: 2018-11-08, 2018-10-18

### Generic Programming

Abstract: The term "Generic Programming" was coined by Alex Stepanov and David Musser in 1988. It has become one of the most influential ideas to shape programming and has had a profound impact on the evolution of C++. Yet the core concepts of generic programming are still often misunderstood and misrepresented. 30 years in, this talk will look a little at how generic programming came to be, and why it is not simply "another paradigm" for software development.

Addition: When preparing this talk I was unable to find a picture of Meng and Alex from the Al Stevens interview in [Dr. Dobb's Journal, March 1995](http://stepanovpapers.com/drdobbs-interview.pdf). Alex was kind enough to give me a copy of the original publication.

*   [JPEG](presentations/2018-11-08-generic-programming/additional/1995-03-dr-dobbs.jpg)
*   [44MB TIFF](presentations/2018-11-08-generic-programming/additional/1995-03-dr-dobbs.tif)

Note: Talk presented at [code::dive 2018](https://codedive.pl/), 2018-11-08.

*   [Video](https://www.youtube.com/watch?v=FX78-1uBCNA&t=869s)
*   [PDF](presentations/2018-11-08-generic-programming/2018-11-08-generic-programming.pdf)
*   [Keynote](presentations/2018-11-08-generic-programming/2018-11-08-generic-programming.key)

Note: Talk presented at [pacific++ 2018](https://pacificplusplus.com/), 2018-10-18.

*   [Video](https://www.youtube.com/watch?v=iwJpxWHuZQY)
*   [PDF](presentations/2018-10-18-generic-programming/2018-10-18-generic-programming.pdf)
*   [Keynote](presentations/2018-10-18-generic-programming/2018-10-18-generic-programming.key)

***

Date: 2017-11-11

### Polymorphic Task Template in Ten

Abstract: A lightning talk demonstrating how to write a polymorphic task template (similar to `std::function<>`) with small object optimization in 10 minutes. The complete template is part of the stlab concurrency library and can be found [here](http://stlab.cc/libraries/concurrency/task/).

Errata: The solution for the small object optimization presented in this talk relies on undefined behavior. A detailed explanation of the issue and a proper solution can be found [here](http://stlab.cc/tips/small-object-optimizations.html).

Note: Talk presented as lightning talk at [Meeting C++ 2017](http://meetingcpp.com/2017/).

*   [Video](https://www.youtube.com/watch?v=2KGkcGtGVM4)
*   [PDF](presentations/2017-11-11-lightning-talk/lightning-talk.pdf)
*   [Keynote](presentations/2017-11-11-lightning-talk/lightning-talk.key)

***

Date: 2018-09-28, 2017-11-09, 2017-11-06

### Better Code: Human interface

Abstract: This talk explains the relationship between good code and a good human interface (graphical or otherwise). Human interface semantics are tightly coupled with code semantics and improving how we reason about code can have a direct on positive impact on the human interface behavior of the system. Part of my “Better Code” series of talks, the goal is “Don’t Lie.”

Note: Talk presented at [cppcon 2018](https://cppcon.org/) and [Wargaming.net](https://na.wargaming.net/).

*   [Video](https://www.youtube.com/watch?v=0WlJEz2wb8Y)
*   [PDF](presentations/2018-09-28-human-interface/2018-09-28-human-interface.pdf)
*   [Keynote](presentations/2018-09-28-human-interface/2018-09-28-human-interface.key)

Note: Talk presented as keynote at [Meeting C++ 2017](http://meetingcpp.com/2017/) and [C++ User Group, Russia](http://cpp-russia.ru/?p=1422).

*   [PDF](presentations/2017-11-09-human-interface/2017-11-09-human-interface.pdf)
*   [Keynote](presentations/2017-11-09-human-interface/2017-11-09-human-interface.key)

***

Date: 2017-01-25, 2016-11-15, 2016-05-16

### Better Code

Abstract: The process of improving an existing body of code can be very challenging. In this talk I'll offer a concise definition of "good code", inherent tradeoffs in any system, and techniques for approaching a large code base.

Note: This is a revised version of the talk presented at [ACCU Bay Area](https://www.meetup.com/ACCU-Bay-Area/).

*   [Video](https://www.youtube.com/watch?v=WPxax37AGz4)
*   [PDF](presentations/2017-01-25-better-code/2017-01-25-better-code.pdf)
*   [Keynote](/presentations/2017-01-25-better-code/2017-01-25-better-code.key)

Note: This is a revised version of the talk presented at [code::dive 2016](http://codedive.pl/en/index/).

*   [Video](https://www.youtube.com/watch?v=cK_kftBNgBc)
*   [PDF](presentations/2016-11-15-better-code/2016-11-15-better-code.pdf)
*   [Keynote](/presentations/2016-11-15-better-code/2016-11-15-better-code.key)

Note: Talk presented as keynote at [C++Now 2016](http://cppnow.org/2016-conference/announcements/2016/03/22/keynote-announced.html) and [ICPC 2016](http://2016.program-comprehension.org/keynote.html).

*   [C++Now 2016 Video](https://www.youtube.com/watch?v=giNtMitSdfQ)
*   [PDF](presentations/2016-05-16-better-code/2016-05-16-better-code.pdf)
*   [Keynote](/presentations/2016-05-16-better-code/2016-05-16-better-code.key)

***

Date: 2017-01-18, 2016-10-10

### Better Code: Runtime Polymorphism

Abstract: This talk explains why (and how) to implement non-intrusive polymorphism in C++. The talk contains many C++ tips and techniques, including many new features from C++11/14. During the course of the talk a key feature from Photoshop will be demonstrated and implemented. Part of my “Better Code” series of talks, the goal is “No Inheritance.”

Note: This is a revised version of the talk presented at [NDC { London } 2017](http://ndc-london.com/).

*   [Video](https://www.youtube.com/watch?v=QGcVXgEVMJg)
*   [PDF](presentations/2017-01-18-runtime-polymorphism/2017-01-18-runtime-polymorphism.pdf)
*   [Keynote](presentations/2017-01-18-runtime-polymorphism/2017-01-18-runtime-polymorphism.key)

Note: This is an updated version of the "Value Semantics and Concept-based Polymorphism" talk presented at Facebook.

*   [PDF](presentations/2016-10-10-runtime-polymorphism/2016-10-10-runtime-polymorphism.pdf)
*   [Keynote](presentations/2016-10-10-runtime-polymorphism/2016-10-10-runtime-polymorphism.key)

***

Date: 2017-01-18, 2016-11-16, 2016-08-08, 2015-06-01, 2015-04-21, 2015-02-27

### Better Code: Concurrency

Abstract: Despite all of the recent interest, concurrency in standard C++ is still barely in its infancy. This talk uses the primitives supplied by C++14 to build a simple, reference, implementation of a task system. And then builds on the tasking system to develop a conceptual model for using futures and continuations, and looks towards sequential communicating processes, higher level concurrent constructs. Part of my “Better Code” series of talks, the goal is to learn to write software that doesn’t wait.

__Erratum: I've incorrectly stated that `std::async()` changed with C++14. I intend to write a lengthier description of the issue soon.__

Note: This is a revised version of the talk presented at [NDC { London } 2017](http://ndc-london.com/).

*   [Video](https://www.youtube.com/watch?v=zULU6Hhp42w)
*   [PDF](presentations/2017-01-18-concurrency/2017-01-18-concurrency.pdf)
*   [Keynote](presentations/2017-01-18-concurrency/2017-01-18-concurrency.key)

Note: This is a heavily revised version of the talk presented at [code::dive 2016](http://codedive.pl/en/index/).

*   [Video](https://www.youtube.com/watch?v=QIHy8pXbneI)
*   [PDF](presentations/2016-11-16-concurrency/2016-11-16-concurrency.pdf)
*   [Keynote](presentations/2016-11-16-concurrency/2016-11-16-concurrency.key)

Note: This is the version of the talk presented at Bloomberg.

*   [PDF](presentations/2016-08-08-concurrency/2016-08-08-concurrency.pdf)
*   [Keynote](presentations/2016-08-08-concurrency/2016-08-08-concurrency.key)

Note: This is an interview from [CppCast](http://cppcast.com/) about my talk.
*   [Audio](http://cppcast.com/2015/06/sean-parent/)

Note: This is the version of the talk presented at the [Bay Area C++ User Group Meeting](http://www.meetup.com/SF-Bay-Area-Cpp/events/221680178/).

*   [PDF](presentations/2015-04-21-concurrency/concurrency.pdf)
*   [Keynote](presentations/2015-04-21-concurrency/concurrency.key)

Note: This is a video from the [C++Now 2015](http://cppnow.org/history/2015/talks/). I belive the slides are the same as those listed above.

*    [Video](https://www.youtube.com/watch?v=32f6JrQPV8c)

Note: This is the version of the talk I presented at the [C++ Russia Conference, 2015 (1)](http://cpp-russia.ru/). I presented an earlier draft at the [ACCU, Silicon Valley Chapter (2)](http://www.meetup.com/ACCU-Bay-Area/).

*   [Video (1)](https://www.youtube.com/watch?v=IYmoz7e1L5c)
*   [Video (2)](https://www.youtube.com/watch?v=BngVqHzOFX8)
*   [PDF](presentations/2015-02-27-concurrency/concurrency.pdf)
*   [Keynote](presentations/2015-02-27-concurrency/concurrency.key)
*   [C++ Source](presentations/2015-02-27-concurrency/concurrency-talk.cpp)

***

Date: 2016-12-14

### Four Tips for Junior Engineers and Senior Managers

Abstract: A short lighting talk presented at the [ACCU, Silicon Valley Chapter](http://www.meetup.com/ACCU-Bay-Area/).

*   [PDF](presentations/2016-12-14-management-tips/2016-12-14-management-tips.pdf)
*   [Keynote](presentations/2016-12-14-management-tips/2016-12-14-management-tips.key)

***

Date: 2016-08-08, 2015-09-23

### Better Code: Data Structures

Abstract: The standard library containers are often both misused and underused.
Instead of creating new containers, applications are often structured with
incidental data structures composed of objects referencing other object.
This talk looks at some of the ways the standard containers can be better
utilized and how creating (or using non-standard library) containers can
greatly simplify code. The goal is no incidental data structures.

Note: This is the version of the talk presented at Bloomberg.

*   [PDF](presentations/2016-08-08-data-structures/2016-08-08-data-structures.pdf)
*   [Keynote](presentations/2016-08-08-data-structures/2016-08-08-data-structures.key)

Note: This is the version of the talk presented at [CppCon 2015](https://cppcon.org/).

*   [Video](https://www.youtube.com/watch?v=sWgDk-o-6ZE)
*   [PDF](presentations/2015-09-23-data-structures/data-structures.pdf)
*   [Keynote](presentations/2015-09-23-data-structures/data-structures.key)

***

Date: 2014-05-14

### Goal: Implement Complete & Efficient Types

Abstract: The C++ Seasoning talk presented at GoingNative 2013 centered on 3 goals for better code. This talk provides foundational material on types, references, and basis operations. We'll look at how the definition of type, and physical nature of the machine, leads to the conclusion that all types are regular and why implementing complete types is a worthy goal.

*   [Video](https://www.youtube.com/watch?v=mYrbivnruYw)
*   [PDF](presentations/2014-04-14-goal-complete-types/goal-complete-types.pdf)
*   [Keynote](presentations/2014-04-14-goal-complete-types/goal-complete-types.key)

***

Date: 2013-09-11

### C++ Seasoning

Abstract: A look at many of the new features in C++ and a couple of old features you may not have known about. With the goal of correctness in mind, we'll see how to utilize these features to create simple, clear, and beautiful code. Just a little pinch can really spice things up.

Note: This is the ACCU, Silicon Valley Chapter version of the talk originally given at GoingNative 2013. The talk is also known as "3 Goals for Better Code." This version contains an expanded ending to the polymorphism code, starting on page 179 of the PDF, and some optional slides that did not make that talk. The extended version of the talk was also given at A9.

*   [Video](http://channel9.msdn.com/Events/GoingNative/2013/Cpp-Seasoning)
*   [Video (extended)](https://www.youtube.com/watch?v=IzNtM038JuI)
*   [PDF](presentations/2013-09-11-cpp-seasoning/cpp-seasoning.pdf)
*   [Keynote](presentations/2013-09-11-cpp-seasoning/cpp-seasoning.key)

***

Date: 2013-09-11

### Inheritance Is The Base Class of Evil

Abstract: Implementing non-intrusive runtime polymorphic objects with value-semantics, and multiple-undo in 20 minutes.

Note: This is an abbreviated version of the Value Semantics and Concept-based Polymorphism talk linked to below given at GoingNative 2013.

*   [Video](http://channel9.msdn.com/Events/GoingNative/2013/Inheritance-Is-The-Base-Class-of-Evil).

***

Date: 2013-09-06
Updated: 2013-09-24

### Value Semantics and Concept-based Polymorphism

Abstract: This talk explains why (and how) to implement polymorphism without inheritance in C++. The talk contains many C++ tips and techniques, including many new features from C++11. During the course of that talk a key feature from Photoshop will be demonstrated and implemented.

*   [Keynote](presentations/2013-09-24-value-semantics/value-semantics.key)
*   [PDF](presentations/2013-09-24-value-semantics/value-semantics.pdf)
*   [C++ Source](presentations/2013-03-06-value_semantics/value-semantics.cpp)
*   [C++ Source (unique_ptr version)](presentations/2013-09-06-inheritance/value-semantics-unique.cpp)

***

Date: 2013-03-05

### Language Delay

Abstract: A look at how programming languages are failing to keep up with a rapidly changing hardware and business landscape. Not long ago it was clear to everyone that because of the rate at which clock speeds were doubling the industry should be optimizing for programmer productivity and let the hardware solve the performance issues. More recently both the dynamics of hardware and the business landscape have thrown these assumptions out the window and left a large vacuum in the programming language landscape. This talk takes a looks at how much things have changed and how a reset in our approach is needed.

*   [Keynote](presentations/2013-03-05-language-delay/language-delay.key)
*   [PDF](presentations/2013-03-05-language-delay/language-delay.pdf)

***

Date: 2012-05-17

### Now What? A vignette in 3 parts

Abstract:
_Part 1: Beauty_
C++ has seen a resurgence in usage as efficiency has come to dominate costs. Yet many still hold their nose as they move to C++. It can be a frightening transition for a programmer coming from Java, JavaScript, or Python. As a community we need to leverage the tools provided by C++11 to write beautiful, efficient, code.

_Part 2: Truth_
On modern hardware, straight C++ code gives you access to just 0.25% of the available performance on the machine. With C++11 we unlock only a little more with thread support. One of the largest challenges for the language and libraries going forward will be how to get at the other 99.75% of the machine.

_Part 3: Goodness_
A plethora of networked devices is changing the software landscape. The infrastructure of the internet is fading into the background and increasingly customers are expecting all of their information, available on all their devices, instantly. What impact do such systems have on how we architect and write software? What is the role C++ will play in this new world.


*   [Video](https://www.youtube.com/watch?v=iGenpw2NeKQ)
*   [Keynote](presentations/2012-05-17-now-what/now-what.key)
*   [PDF](presentations/2012-05-17-now-what/now-what.pdf)

***

## Papers

***

Data: 2016-07-18

### One Way to Select Many

Authors: Jaakko Järvi, Sean Parent

Abstract: Selecting items from a collection is one of the most common tasks users perform with graphical user interfaces. Practically every application supports this task with a selection feature different from that of any other application. Defects are common, especially in manipulating selections of non-adjacent elements, and flexible selection features are often missing when they would clearly be useful. As a consequence, user effort is wasted. The loss of productivity is experienced in small doses, but all computer users are impacted. The undesirable state of support for multi-element selection prevails because the same selection features are redesigned and reimplemented repeatedly. This article seeks to establish common abstractions for multi-selection. It gives generic but precise meanings to selection operations and makes multi-selection reusable; a JavaScript implementation is described. Application vendors benefit because of reduced development effort. Users benefit because correct and consistent multi-selection becomes available in more contexts.

Note: Paper was presented at [ECOOP 2016](http://2016.ecoop.org/event/ecoop-2016-papers-one-way-to-select-many)

*   [Video](https://www.youtube.com/watch?v=i8lc0I1yoZ8)
*   [PDF](papers/2016-07-18-ecoop/ecoop16-final28.pdf)
*   [Example Library](http://hotdrink.github.io/multiselectjs/)

***

Date: 2015-10-26

### Generating Reactive Programs for Graphical User Interfaces from Multi-way Dataflow Constraint Systems

Authors: Gabriel Foust, Jaakko Järvi, Sean Parent

Abstract: For a GUI to remain responsive, it must be able to schedule lengthy tasks to be executed asynchronously. In the traditional approach to GUI implementation–writing functions to handle individual user events–asynchronous programming easily leads to defects. Ensuring that all data dependencies are respected is difficult when new events arrive while prior events are still being handled. Reactive programming techniques, gaining popularity in GUI programming, help since they make data dependencies explicit and enforce them automatically as variables’ values change. However, data dependencies in GUIs usually change along with its state. Reactive programming must therefore describe a GUI as a collection of many reactive programs, whose interaction the programmer must explicitly coordinate. This paper presents a declarative approach for GUI programming in which the programmer is relieved from coordinating asynchronous computations. The approach is based on our prior work on “property models”, where GUI state is maintained by a dataflow constraint system. A property model responds to user events by atomically constructing new data dependencies and scheduling asynchronous computations to enforce those dependencies. In essence, a property model dynamically generates a reactive program, adding to it as new events occur. The approach gives the following guarantee: the same sequence of events produces the same result, regardless of the duration of processing the events.

Note: Paper presented at [GPCE 2015](http://2015.gpce.org/event/gpce2015-generating-reactive-programs-for-graphical-user-interfaces-from-multi-way-dataflow-constraint-systems).

*   [PDF](papers/2015-10-26-gpce/gpce15main-mainid43-p-e08bb9a-25371M-final.pdf)
*   [ACM Citation](http://dl.acm.org/citation.cfm?id=2814207)

***

Date: 2011-10

### Helping Programmers Help Users

Authors: John Freeman, Jaakko Järvi, Wonseok Kim, Mat Marcus

Abstract: User interfaces exhibit a wide range of features that are designed to assist users. Interaction with one widget may trigger value changes, disabling, or other behaviors in other widgets. Such automatic behavior may be confusing or disruptive to users. Research literature on user interfaces offers a number of solutions, including interface features for explaining or controlling these behaviors. To help programmers help users, the implementation costs of these features need to be much lower. Ideally, they could be generated for free. This paper shows how several help and control mechanisms can be implemented as algorithms and reused across interfaces, making the cost of their adoption negligible. Specifically, we describe generic help mechanisms for visualizing data flow and explaining command deactivation, and a mechanism for controlling the flow of data. A reusable implementation of these features is enabled by our property model framework, where the data manipulated through a user interface is modeled as a constraint system.

Note: Paper presented at [GPCE 2011](http://program-transformation.org/GPCE11/WebHome).

*   [PDF](papers/2011-10-gpce/p177-freeman.pdf)
*   [ACM Citation](https://dl.acm.org/citation.cfm?id=2047892)

***

Date: 2010-09

### Dynamic Algorithm Selection for Runtime Concepts

Authors: Peter Pirkelbauer, Sean Parent, Mat Marcus, Bjarne Stroustrup

Abstract: A key benefit of generic programming is its support for producing modules with clean separation. In particular, generic algorithms are written to work with a wide variety of types without requiring modifications to them. The Runtime concept idiom extends this support by allowing unmodified concrete types to behave in a runtime polymorphic manner. In this paper, we describe one implementation of the runtime concept idiom, in the domain of the C++ standard template library (STL). We complement the runtime concept idiom with an algorithm library that considers both type and concept information to maximize performance when selecting algorithm implementations. We present two implementations, one in ISO C++ and one using an experimental language extension. We use our implementations to describe and measure the performance of runtime-polymorphic analogs of several STL algorithms. The tests demonstrate the effects of different compile-time vs. run-time algorithm selection choices.

*   [PDF](papers/2010-09-scp/1-s2.0-S0167642309000732-main.pdf)
*   [ACM Citation](https://dl.acm.org/citation.cfm?id=1823252)

***

Data: 2009-10-04

### Algorithms for User Interfaces

Authors: Jaakko Järvi, Mat Marcus, Sean Parent, John Freeman, and Jacob N. Smith

Abstract: User interfaces for modern applications must support a rich set of interactive features.
It is commonplace to find applications with dependencies between values manipulated by user
interface elements, conditionally enabled controls, and script record-ability and playback against
different documents. A significant fraction of the application programming effort is devoted to
implementing such functionality, and the resulting code is typically not reusable.

This paper extends our “property models” approach to programming user interfaces. Property models
allow a large part of the functionality of a user interface to be implemented in reusable libraries,
reducing application specific code to a set of declarative rules. We describe how, as a by-product
of computations that maintain the values of user interface elements, property models obtain accurate
information of the currently active dependencies among those elements. This information enables
further expanding the class of user interface functionality that we can encode as generic
algorithms. In particular, we describe automating the decisions for the enablement of user interface
widgets and activation of command widgets. Failing to disable or deactivate widgets correctly is a
common source of user-interface defects, which our approach largely removes.

We report on the increased reuse, reduced defect rates, and improved user interface design
turnarounds in a commercial software development effort as a result of adopting our approach.

Note: Paper presented at GPCE 2009.

*   [PDF](papers/2009-10-04-gpce/p147-jarvi.pdf)
*   [ACM Citation](https://dl.acm.org/citation.cfm?doid=1621607.1621630)

***

Date: 2008-10

### Property Models: From Incidental Algorithms to Reusable Components

Authors: Jaakko Järvi, Mat Marcus, Sean Parent, John Freeman, Jacob N. Smith

Abstract: A user interface, such as a dialog, assists a user in synthesising a set of values, typically parameters for a command object. Code for "command parameter synthesis" is usually application-specific and non-reusable, consisting of validation logic in event handlers and code that controls how values of user interface elements change in response to a user's actions, etc. These software artifacts are incidental - they are not explicitly designed and their implementation emerges from a composition of locally defined behaviors.

This article presents property models to capture explicitly the algorithms, validation, and interaction rules, arising from command parameter synthesis. A user interface's behavior can be derived from a declarative property model specification, with the assistance of a component akin to a constraint solver. This allows multiple interfaces, both human and programmatic, to reuse a single model along with associated validation logic and widget activation logic.

The proposed technology is deployed in large commercial software application suites. Where we have applied property models, we have measured significant reductions in source-code size with equivalent or increased functionality; additional levels of reuse are apparent, both within single applications, and across product lines; and applications are able to provide more uniform access to functionality. There is potential for wide adoption: by our measurements command parameter synthesis comprises roughly one third of the code and notably more of the defects in desktop applications.

Note: Paper presented at GPCE 2008.

*   [PDF](papers/2008-10-gpce/p89-jarvi.pdf)
*   [ACM Citation](https://dl.acm.org/citation.cfm?id=1449927)

***

Date: 2008-03

### Runtime Concepts for the C++ Standard Template Library

Authors: Peter Pirkelbauer, Sean Parent, Mat Marcus, Bjarne Stroustrup

Abstract: A key benefit of generic programming is its support for producing modules with clean separation. In particular, generic algorithms are written to work with a wide variety of unmodified types. The Runtime concept idiom extends this support by allowing unmodified concrete types to behave in a runtime polymorphic manner. In this paper, we describe one implementation of the runtime concept idiom, in the domain of the C++ standard template library (STL). We describe and measure the performance of runtime-polymorphic analogs of several STL algorithms. We augment the runtime concept idiom by employing a dispatch mechanism that considers both type and concept information to maximize performance when selecting algorithm implementations. We use our implementation to demonstrate the effects of different compile-time vs. run-time algorithm selection choices, and we indicate where improved language and compiler support would be useful.

*   [PDF](papers/2008-03-sac/p171-pirkelbauer.pdf)
*   [ACM Citation](https://dl.acm.org/citation.cfm?id=1363734)

***

## Interviews

***

Date: 2019-10-17

### cpp.chat - Episode #62 (with Sean Parent) "I’m Surprised You Brought up Rotate"

Jon Kalb and Phil Nash interview me live at [CppCon 2019](https://cppcon.org/).

*   [Video](https://www.youtube.com/watch?v=czHpJE3xIXc)

***

Date: 2017-09-14

### Meeting C++ live - "Parental Advisory" with Sean Parent

Jens Weller from [Meeting C++](http://meetingcpp.com) interviewed me leading up to my keynote.

*   [Video](https://www.youtube.com/watch?v=0ZS_-G4urko)

***

Date: 2016-08-06

### An interview with Sean Parent

Jens Weller from [Meeting C++](http://meetingcpp.com) interviewed me following my C++Now keynote.

*   [Link](http://meetingcpp.com/index.php/br/items/interview-with-sean-parent.html)

***

Date: 2015-06-01

### Better Code Concurrency with Sean Parent

An interview from [CppCast](http://cppcast.com/) about my [talk](#better-code-concurrency).

*   [Audio](http://cppcast.com/2015/06/sean-parent/)

***

## Older papers and presentation

***

Some of my older papers and presentation can be found [here](http://stlab.cc/legacy/papers-and-presentations.html).
