I'm Shawn parent for anybody who wasn't at my previous talk I'm a principal scientist at Adobe. I've had a number of roles they're good at Adobe now about 22 years depending on how you count because I was there 15 years and then I went to Google for about a year and then back to Adobe. I started my career at Adobe on the Photoshop team and later I ran Adobe's software technology lab that was a research group that spun out of our technology group. I ran that group for about eight years and now I'm actually back working with Photoshop team again so I've had a number of roles.

I mentioned this before - a few years back I gave a talk at going native conference called C++ seasoning and and it was well-received and I got a lot of requests to write a book and so since then whenever I get asked to give a talk I try to flesh out a chapter of the book so this is kind of
my working outlines you see for each
chapter here I've got a goal most of the
goals I need to redo the first one now
that one's also stated in the negative
are stated as a negative as far as their
what not to do and there's a reason
there the the word goal was chosen very
carefully because what I talked about is
is not prescriptive things they're not
rules they're things to strive for but
they can be difficult especially when
you're dealing with a large system to
achieve these goals so they're not rules
are not guidelines they're not things
that should be held up in a code review
or people that John said don't do that
what Sean said is think about that
really hard and and the reason the other
reason why they're stated as a negative
is because I can't give you all the
answers as to as to what the
alternatives are so these are things
things to try to avoid and I'll talk
some about better in different ways but
sometimes it just requires more thought
so these are all tend to be open-ended
problems 

so today we're talking about concurrency.
Alright some common themes in all my talks are our that they're all about managing relationships and what I mean about that is relationships between components and software understanding the fundamentals which i think is incredibly important. Learning how to write simple code and learning how to how to reason about code through local and equational reasoning. So local reasoning means that just looking at a piece of code you can reason about it without having to understand the whole system. And equational reasoning means that you can you can describe what something is in terms of something else a is equal to B right a plus B is equal to C all right so you're able to go through those transformations now this talk started because I was working on what became this product right here. 

This is Lightroom for web and and this is the complete Lightroom rendering engine running inside of the Safari browser with no plugins okay so for people who don't know Lightroom is a product that adobe ships that professional photography rendering engine and it's a parametric model so it's a non-destructive model you you edit your image and you can come back later and change your edit and read just it so to make this work over the web means that we had to have full parity with the desktop rendering engine okay and this is a piece of code now that's a decade old and heavily tuned on the desktop. To give you some idea this is a woman in the London subway and as the train was pulling up. I glanced backwards and snap the photo and so that's the the unedited photo and that's what it looks like edited. So this was a big problem taking a 10 year old codebase that's heavily threaded getting it to compile with the emscripten compiler which is a compiler that Mozilla puts out to compile C++ code into JavaScript removing all of the need for threading in it because in JavaScript you have no threads you have web workers which is the equivalent of having separate processes but you don't have threads and getting the code to run well and to behave in an interactive fashion so that was the challenge so this got me thinking really really hard about concurrency and the reason why I got me thinking is because going forward this code base needs to be shared so I needed to leave the system in a state where we had a code base that developers could work on that would continue to run in run performant in a heavily multi-threaded environment and the same code would run very well when you had no threads right so I had to think about what is the actual programming model for how you write threaded code.

So first some definitions here right concurrency is when tasks start run and complete in overlapping time periods where parallelism is when two or more tasks execute simultaneously right so on a single threaded system you can have concurrency and you can have threads and it just means things are time sliced okay but parallelism is when you have multiple cores right or something like a sim decore where you can do multiple things at the same time now the reason why we want concurrency is because concurrency is our first building block to enable parallelism right right 
if I can't run run with multiple threads on a single core then I certainly can't run with multiple threads with multi-core the other reason why we want concurrency is because it improves interactivity right in the previous thing there right when I'm sliding my my slider I'm receiving user events coming in and I'm updating frames and if every time I update a frame. if that slider stops that the users sliding that makes for a pretty at the user experience right so we want to be able to slice things up so that we give give good interactivity down goal. 
for this talk is no raw synchronization primitives so what are raw synchronization primitives? well there's these things they're mutexes there are atomics a lot of people don't don't like me putting Atomics on this this list but they're certainly Atomics and i'll explain more about that later but things like semaphores memory fences and conditional condition variables okay the number one reason why you don't want to use these things is you'll likely get them wrong. 

How do I know that? Because I've been doing this a very long time and I still get them wrong. So here is a class that I wrote once upon a time its renamed it's the copy-on-write class and it's abbreviated here. It's renamed as bad cow and the reason why I renamed it is because giving this as a as an example of a bug then somebody immediately when it was just named copy-on-write. somebody immediately copy and pasted the code from my slides and put it into a working system. so not actually looking at the rest of my presentation where I showed the bug so who can find the bug in this code. I'll even help you the bug is between those two lines does anybody see what's wrong here? time of check and time of use problem absolutely okay so and I'm using an atomic here right so what happens is. I check to see is my count equal to 1 what a count of one would mean is that this object is the sole owner of some shared data. if this object is the sole owner of some shared data then I know that it's safe to assign directly into it. If it's not one, then I need to make a copy here. this is my copy on write. When I make the copy then I need to to decrement the count of over this one here okay so so what's the problem? The problem is if I wasn't the sole owner on the first check then by the time the second check comes around I might be the sole owner and if now at the second at that decrement if I am the sole owner it's my responsibility to free the shared state okay and I'm not doing that here right so the six is when I do the decrement I have to say okay when I do this decrement was I the sole owner and now my count went to zero and if so delete the object. 

So this piece of code shipped in a running system and probably ran for something like five years okay and it wasn't that bad right you have a very short window there where you do that new where somebody else can release ownership and if they do then you'll leak one block okay okay but it was very vexing because occasionally we would see these blocks leak and couldn't figure out why or where they were coming from okay even with these slides I've given this talk now a couple times I've received five email messages where people tell me my fix is is wrong okay it is correct I'm quite convinced I can write the proof fix is correct the five people who wrote to me all what they believe is a bug all boils down to to one of two things in those five emails one is they assume that my bad count implementation is an atomic type it is not an atomic type right right if it were an atomic type then when I was sole ownership somebody else could come along and and claim ownership of it okay so the account could go up it's not an atomic type in that a copy-on-write integer is just as thread-safe as an integer right it just means that I can have separate threads each with their own instances with shared data okay but I can't share a single instance of the cow right the cow refers to share data. 

The other thing is people don't understand what - minus X does on an atomic and that's that it's an equivalent of effect such subtract of an atomic such a subtract which means the subtraction and the fetch of the value happen is a single atomic operation and sometimes people think that. Because I'm doing the -- and then my compare that between the subtraction and the compare something could go wrong nobody caught. I fixed it in the previous slide that my count was uninitialized. Neither did I so neither did the compiler which Frost's me so originally when I wrote the code. it was in a working system. I didn't have this problem and it's because I wasn't using SPD Atomics because the code long predates STD Atomics. I was using a Atomics with different behavior but an STD atomic of an int if you just default construct it right without giving it any value is an uninitialized variable just like an INT is okay so I actually introduced a bug and nobody caught that.

Even atomics can be very difficult to reason about why do we want performance through parallelism.


This is a slide that one of my colleagues Russell Williams on the Photoshop team put together awhile ago but it's very true today so this is your typical desktop computer right and this is looking at where the nips are where the gigaflops in a desktop computer okay that's the bar that's the GPU right right so so this is looking at an AMD Radeon which is a little outdated that bar these days is getting bigger if you're running on mobile it's a little bit smaller okay to get into that you need to get into OpenGL OpenCL cuda direct compute c++ am DirectX metal pick your thing all of those things tend to be C or C++ derived programming environments with constraints tailored towards parallelism okay that's how much of your performance is in the vectorization unit on the machine okay so to get to that you're going to have to be writing with Auto vectorization or like the Intel compiler we'll just some degree of that but you have to know how to structure your code to utilize it OpenCL can target it or your writing with intrinsics which is where most people end up if you want to unlock multi-core as of C++ 11 we actually at least have a threading model there that's how much of your processor is tied up with multi-core and if you're talking about just one core the scalar unit that's the slice right there that's what most people think of a straight C++ and if you're writing in JavaScript or Java or something like else something else divide that by a factor of 2 to 5 these days okay so that red line at the
end there is point 0 to 5 percent of the performance of your machine

okay now this is Armdahl's law Kevlin mentioned this in his last talks you were in here what AM does law says is is basically if some amount of your machine is serialized okay then you cannot accede to that in overall performance by adding parallelism okay so in this graph here you can see if the the Purple Line is at ninety percent so if just 10% of our app is serialized we can never perform better than 1010 X increase in performance because now that will be the bottleneck is that that cereal is serialized piece okay but the interesting thing about this graph is that you don't hit something approximating that 10x speed up until you get out to a thousand cores okay the bottom scale on this thing is logarithmic where the top scale is linear and I hate the graph for that reason I think this is a very misleading graph okay so but this is what you'll find on Wikipedia or in most textbooks is people draw the graph this way because otherwise you can't really see the shape of the curve okay but the more telling graph is to just look on linear scale down at the access so each line here represents just 10% of serialisation so if I just have 10% of my application that's serialized and I'm on 16 cores I'm only getting a little better than a 6x speed-up okay so Amdahl's law sucks right right it bites us okay and serialization doesn't mean locking on mutex utilization can just mean sharing the same memory right or sharing the same address bus for them for the memory if I don't have a Numa architecture okay or sharing the same cache line right anything that's shared within the processor starts to bend that curve down and it bends down rapidly any synchronization anatomic bends that curve down.



most people think about threading at least most people you know coming from kind of a C or C++ background in this horrible way they say I've got this object that I'm sharing and I got a bunch of threads that are banging on it and so I'm going to stick a mutex on there and we're going to keep rotating that around. That is a horrible horrible way to think about threading. Don't do that! We don't want to stop! No stops, minimize locks! 


Now a lot of people that I find when I talk about mutex and I asked them what is a mutex right can't give me a good answer say give me an equivalency to a mutex. Give me a mental model for a mutex? I added this section to the top because somebody who I work with came to me and said "I'm trying to optimize this piece of code where I've got this mutex and I've got this lock over this big block of code and I've tried to minimize the amount of work I'm doing under the lock to the absolute minimum and it's still huge. So now I'm trying to optimize the locks and I'm playing with like adaptive spin locks and stuff." 


And I'm like ah you're thinking about it all wrong. 
So let's take a look at a traditional little piece of code here. I've got some registry class that I'm going to write and it's going to be shared so I want the operations to behave in an atomic fashion so I'm going to have a set and a get right where I can set a key and a value and it's just going to store it into a map and because I'm sharing my map I've got that under my mutex and I'm going to get it okay and so I'm going to lock lock my mute my mutex and just pull it out at the same time okay so I've got this this little piece of code right now even here although it looks like I'm doing just a little bit of work under the mutex right and I'm using an unordered map which is a fairly efficient data structure an unordered map is a hash map the amount of time it takes me to hash my key depends on the length of the string so the amount of work that's being done under the locks here is actually fairly unbounded right it depends completely on the lengths of length of my string so it's probably typically small but it could be big okay 



there's this line of the C++ standard and you'd find it in a bunch of text books but I pulled it from there because I think you know it's a good way to think about it it can be showing the programs that correctly use mutexes and memory ordered sequential sorry I'm drawing a blank it's a memory very a sequentially consistent memory barrier and a memory order sequentially consistent memory barrier operations to prevent all data races and use no other synchronization operations behave as if the operations executed by their constituent threads were simply interleaved with each value computation of an object being taken from the last side-effect of that object in that interleaving this is normally referred to as sequential consistency so why is this an important sentence well it means that we can always think about mutexes as if we had some set of operations and those operations are interleaved right that we have op and I messed up the numbers here so I can have operation 3 repeated multiple times right right and and what an operation is is that's the code that's executing onto the lock and its operating on some value what's that value it's the set of all objects that are modified by that operation ok so by all operations okay so so X is everything that gets modified within my lock ok by all the pieces of code that acquire that lock on the mutex ok and each of those can yield out some values some result right those operations can say it's a read-modify-write operation they can read modify write the data under that yielding out some results from the read okay so the same is true of all atomic operations right so there's really not a lot of difference between an STD atomic in fact there is a call on STD atomic that says is this lock free so what that means is is their processor support to do that as an atomic item within the processor or is there not processor support and the compiler has to generate a mutex pair to to lock make the change on the atomic operation and do the the unlock right so all that mutexes and locks are our way to construct atomic operations right so if you look you know fetch subtract follows from my previous example for the bad cow you're doing a section subtract that's a read modify write operation on X to the atomic so what we have here then is that we can take any piece of code that has mutexes and we can transform it into a queued model okay so if we assume that I've got a serial q and my serial q has just one operation on it which is go a sync execute this thing and it's the same same calling conventions as STD async here except it guarantees that that the next item being processed in that queue doesn't start until the previous one completed okay then I can rewrite my set string to go do QA sync okay and I can rewrite my get string but I've got a little bit of a problem there in that I need the result back out and I need it paired with that particular gift okay so I'm going to use a future there and we'll talk more about futures okay and I've got the Q async here why is this important to understand because any place I have a mutex in my code I can always make this transformation I can always trance form it into a serialize queue model and that means that within the serialized queue model now anytime somebody comes along and says set here regardless of the amount of work that set takes the time it takes for set to return back to the caller is constant. okay so that means that I can add something like set an arbitrary set of value the whole vector of key value pairs okay and to the caller that set will take just as much time as the previous set. it's a no block. okay so so this puts an upper bound. now there's overhead in this right. I've got to queue an item. I've got to DQ the item. I've got to deal with futures if I've got results coming in if I'm calling this set as opposed to calling just set string set sync set string. 


maybe I have to construct my vector pairs I can't just pass iterators in there because my whatever I'm passing iterators in to might go away okay so there's overhead in this but it gives me an upper bound so if I'm looking at a piece of code or I'm like I've got this big hoo Creek and piece of code with a bunch of mutexes around it. I'm like you're totally in the weeds right the place is enough abound on both synchronization overhead and time to issue operations. 
Now couple more definitions:
  threads: a thread is an execution environment consisting of a stack and processor state running in parallel or concurrent plugged in my slides to other threads okay a task is a unit of work off often a function that you want to issue on a thread so tasks are scheduled they can be scheduled onto a thread pool top to Mai's machine utilization I mean threads tend to be relatively heavy weights because they're carrying around processor state and and stacks and if you saw cat Kevin's talk previous to this you had a great graph showing just how expensive it is to do a task switch from thread to thread. You can do an awful lot of processing on the 10s which now unfortunately C++ 14 doesn't really have a task system. we have threads okay and we have futures okay but we don't have a task system now. worse it's in its implementation to find whether or not STD async spins up a thread or execute it on a thread pool. in C++ 11 it was actually defined that it's spun up a thread but Microsoft's complained because under Windows 10 Microsoft doesn't want to expose developers to threads. they want everything to go through the windows thread pool. so they didn't want STD async to spin up threads. so in C++ 14 they changed the standard wording to say well it's undefined maybe it's a thread maybe it's executed on a thread pool.
  
I'm going to show you in a few slides why that was a very problematic decision. okay now I'm going to walk you guys through building a tasking system. We're going to build kind of a portable reference implementation and C++ 14 but don't do it on your own right not unless you have to. I actually had to do this because in the previous slides getting Lightroom web to run one of the places where I had
to make it run was inside the Chrome
browser using pinnacle which is Google's
portable Native Client and that's
basically in an environment where you
can run threaded C++ code inside of the
browser so long as it's a Chrome browser
or or on a Chromebook but they didn't
have a stock tasking system for it and I
couldn't find a reasonable one that was
open sourced apples Grand Central
Dispatch is open sourced but it has a
lot of dependency on Lib kernel and I
don't have a kernel inside of Chrome
unfortunately so I couldn't make that
work
but there are lots of good ones here
right there's windows has thread pool
and ppl Apple has Grand Central Dispatch
which can run on Linux and Android
there's until TBB there's HP X I'm sure
that you can find others but those would
be the ones that I would I would
recommend okay this is one that I would
not recommend.

So there's a little link in the URL there Oracle has this paper that they published years ago which is how to write high-performance multi-threaded server code and and as an appendix they have how to build a tasking system in there for a thread pool this is their model. 

You've got a issue tasks into the queue, the tasks get dequeues by multiple threads, you have one queue per piece of hardware that you have and their implementation is about 20 pages. 

But we can build with c++ better one pretty quickly so we're going to build a notification queue. Here we're just going to build it out of a deck of STD functions with the mutex and a conditioner variable around it we're going to have a pop operation which is just going to pull one item off of the queue okay in our function we're going to have a push operation to just push one item in the queue and notify anybody who might be waiting on the queue okay and then we can build a tasking system where we have count which is the number of course we have and our vector of threads okay and our notification queue a run function for our our threads which is just going to to create a function pop the function from our queue and execute it okay and then our constructor here for our test system will spin up one thread binding it with a lambda against our run when we destruct we need to join all of our threads and then we're going to have an async operation where we just take a function and we push it okay so pretty primitive the one thing we would hang on destruction right there because we didn't have a way to signal done so we'll add that real quick we're going to have a bool that's done when you call done we're going to set the bool to true and notify everybody okay in our pop function when we're waiting if we get woken up from our condition of variable then we're also going to be checking done right returning false and that's it that's the complete thing so how do you guys think this performs I already told you right right badly kind of barely get off the mark horrible why is this horrible makes a good example right it's easy to write nothing fundamentally wrong about it it's exactly this model right we've got a single Q and we've got a bunch of threads and they're banging on that Q okay and we've got locks understanding that I can transform those locks into a queue model doesn't help me because all I have is a Q it locks around it okay so I need to come up with a better way.

okay so that's X so here is the common
answer I put a Q on each thread that
minimizes my contention okay so let's
write that we can do this pretty quickly
we're just going to have a vector of
notification Q's we get to reuse or same
notification q okay one for each thread
so now our run is going to take an index
into into which q it's actually bound
against when we join now we need to tell
all the Q's they're done
next one and when we're going to do our
push we're going to keep a little atomic
index so we can be pushing from multiple
threads and we're just going to keep
incrementing it but then we're going to
modulo it by our count and so we're just
going to round-robin going through
pushing into into it there okay and if
you notice that's unsigned so wrapping
around on that index is not a problem
well-defined behavior on our overflow
it's a modulo arithmetic okay so how do
you guys think we did on our speedometer
twice as fast is it going to go half
halfway up we're just going to go nobody
so so I actually did profile all this
code I wrote this all profiled it it's
about ten times better okay
barely get this movie okay so and we
have two problems here one is is I can
get a long-running task in one of my Q's
and it can cause a bubble and I can have
my other course sitting there idle okay
so the other is is even in a fully
loaded system I can end up with a fair
amount of contention right where one of
those mutexes blocks and that's you know
upwards of a million cycles to do a full
task switch right in overhead so having
keeping one of those mutexes and
actually having it locked kills
performance okay so here's the next
solution right called task stealing okay
now there are lots of sophisticated
algorithms that are heavily tuned for
doing task stealing but we're just going
to code it very simply here and the idea
with task stealing is is if your queue
you being a thread is busy because
somebody's pushing in the other end okay
ah or it's empty
either you don't really care see if you
can go pull something out to somebody
else's queue
okay so let's do that so and we're going
to take our pop and we're going to have
tri pop here okay so our tri pop is
going to take a tri lock on our mutex
okay so it's going to try to lock will
either get the lock or not if we don't
get the lock we just return false okay
or if the queue is empty we return false
by the way
so we'll do that we're going to do the
same thing on our push just for good
measure okay
so if I'm trying to push something into
a queue and that queue is busy I
shouldn't wait on it okay cause hold a
mutex cause a whole process or cast
square to even do a spin lock okay what
I'm going to do is if it's busy I'm just
going to return false
okay couldn't push it okay so now our
caste system our constructor and
destructor stay the same okay but when
we do the run now what we're going to do
is we're going to do try pop and we'll
just run once round all the queues okay
and then if we don't get anything once
around then we'll just wait where we
were
okay now
on the push side we'll spin around and
it turns out on the push if all the cues
are busy right right you have to push
okay so so what are you going to do well
you could just sit and wait on one cue
but there's no guarantee that that's the
first cue that frees up okay so it's
actually better on the push to spin a
little more okay so I've got a que there
right right and there's a trade-off
there you're you're improving overall
throughput of the system versus
sacrificing a single core to burn it in
the spin loop so whether or not it's a
win in your system depends on what the
load is on your system okay whether or
not you have that core to burn so but
you can tune it quite a bit so now how
do you think we did
okay so way better okay so we got way up
into there now at this point we're about
a hundred times better than when we
started from Oracle's example code so
this is a pretty huge improvement okay
now what I was benchmarking these at was
Apple's Grand Central Dispatch
that's my one point oh right that's my
mindful thing apples Grand Central
Dispatch is a very nice piece of work if
you actually go and read the code and it
even has hooked into Lib colonel and the
reason why it has looks into Lib colonel
is because it understands when one task
is blocked and how to go just ignore
that task and go take care of other
things so you cannot match it with the
primitives that you have in standard C++
okay but we actually came within about
15%
okay of it with some straightforward
code so that's pretty good okay now if
you happen to have live dispatch this is
a standard STD async compatible function
I'm not going to walk through the whole
thing here that sits on top of dispatch
async F which is a call into Lib
dispatch so this is is is fully STD
async compliant including allowing you
to move arguments through the whole
thing and stuff written in terms of
dispatch async so if you're on a machine
where you can use Lib dispatch by all
means steal this code and do
okay now the first time I gave this talk
somebody went and bent my ear for about
three hours and said well if you're
doing anything you should be writing and
writing it in terms of a CEO and a do
tasks because that's been proposed to
the standard and a CEO being a
synchronous i/o and it has been as an
experimental library and I think a zero
is a beautiful piece of work so don't
get me wrong here
but a CEO is not a tasking system so in
fact a CEO doesn't even have tasking
primitives so is this person spending my
ears and why didn't I use a CEO I'm like
well what are you even talking about
okay but you can build a tasking system
out of a CEO and and this is what it
looks like it's fairly straightforward
and that's how it performs almost
identically to my first piece of code
for exactly the same reasons in here we
have a single single service queue that
we are posting work to and it's under
heavy contention and it's getting
pounded upon it is actually slightly
faster by about 10% then my
implementation with a single queue okay
but that doesn't show up as a pixel even
on my speedometer so so that's where a
CEO is so don't let people tell you as
you as a tasking system it's not okay so
now we have a system where we can issue
tasks
so what happens we issue a task which is
some function some work bound to it it
goes and executes we've got another task
that task yielded some object and we
want to communicate that result to
something else that's executing right
how do we do that what's our what's our
form of communication between tasks
all right well this is where futures
come into play okay so we're going to
calculate Fibonacci of a million CPP
inte is a boost a infinite precision
math type so that's an infinite
precision integer okay anybody you have
any idea of how big fibonacci of a
million is so it's big
subin achi is also is often used as an
example in in parallel algorithms please
stop
almost every talk I go to on parallelism
uses Fibonacci numbers uh here's why so
there's a slow way to calculate
Fibonacci which is what most people
demonstrate and there's a proper way to
calculate Fibonacci to calculate
Fibonacci what you need is a power
function so this power function will
take X to the power of n for some
operation so if my operation here is
addition then this does multiplication
okay if my operation is multiplication
this does exponentiation okay so this is
a generic form of it unfortunately power
isn't in the standard library but it
should be if you want more information
about how this works there's a great
write-up on it in Alec Stepanov and Dan
Rose's book for mathematics to generic
programming kind of the full history of
this algorithm it's also known as
Egyptian multiplication or the Russian
peasant algorithm okay so what I want to
do then is I want to take a particular
matrix which oh there it is I want to
take that matrix which is 1 1 1 0 which
is known as the Fibonacci matrix and I
want to raise it to this let me back up
didn't mean to go that far that's
Fibonacci of a million
slow okay
so I want to take a Fibonacci matrix
raised to the nth power and that will
give me the Fibonacci number of n plus 1
in the top left Fibonacci n on the two
diagonals and n minus on the bottom
right ok so I can actually take
Fibonacci here of n minus 1 and pull it
from impulsive iñaki n from the top left
okay so with that code and now you see
the delay here it takes a key note a
minute to prep this this is the result
of Fibonacci of a million okay it took
point seven two seconds to calculate on
my old on this laptop on my old 2013 or
so MacBook Pro it's two hundred and
eight almost two hundred nine thousand
digits and and it calculates and and
that's just running single threaded okay
so I can calculate way bigger Fibonacci
numbers then then people do in the
parallel examples the reason why bugs me
is because I've actually seen people use
the bad algorithm that people use to
demonstrate with parallel algorithms in
actual code okay stop so try to only
give real examples so we'll calculate
seven oxy of a million with our future
so what's our mental model of futures
okay what we're doing is we're taking a
function and we're slicing off the
results okay
so what I end up with is a function okay
still take some arguments but has no
result and a future which is a token for
the result and that future will appear
sometime later okay so that's the model
the futures it's separating the function
from the results of the function and
having a communication channel before
them now the nice thing about futures is
they allow minimum code transformations
to express dependencies
okay so if I have a future and oh the
other nice thing about futures and C++
is they can marshal exceptions right so
if I have a future and I fail this will
end up printing failure right so across
threads my exception can be thrown in
one thread and caught in the other
go there and then I call future get and
let's just stop right we just block that
thread while we wait for the results STD
futures and then the result comes over I
hate stops right unfortunately C++ even
14 lacks continuations with the
continuation instead of saying dot get
and stop you say when the results for
this is available then do the next thing
if you've done JavaScript programming
with with promises it's the same idea
okay
so along with that C++ 14 lacks joins
which are win all and win any when all
these features are available to
something it'll act splits which we'll
talk about more cancellation and
progress monitoring in C++ 14 features
don't compose particularly easily to add
these things okay so what's the model
for continuations well we've got is a
function yielding a result which we want
change to the next function yielding
some results and so on
okay now STD future has two additional
problems
so first is that it consumes one
resource right which we talked about
increasing contention but it possibly
deadlocks if you have a tasking system
and any non subsequent non-dependent
calculations on the task girls are
blocked
now even though C++ 14 doesn't have the
primitives boost futures has been with
TS concurrency which is following to the
standard has been TVB has flow graphs
ppl has chain tasks CCD has serialized
queues and groups these are all things
to solve the same problem as
continuation
okay so how can get caused a deadlock
let's say I have a task and it issued
another task and then it waits on a gift
that other task will never complete
because it's not coming out of the queue
so if I'm on a single threaded system
okay then a future get is an immediate
deadlock this is why promises in
JavaScript do not have git or dot wait
they only have continuations in a single
threaded system it's an immediate
deadlock okay well what if I have two
threads that guy issues two tasks one
task goes into the queue he waits for it
he's deadlocked well the other task a
task steal unless he issues a task and
waits for it and then we're deadlocked
okay and so on right
it's very difficult to use future like
it is very difficult to use safely in
the presence of a thread pool an STD a
sink it's undefined whether or not it's
a thread pool or not okay that's a
problem it's not just yet it's any
conditional blocking right such as
condition variables weight or
problematic in a task system they're
exactly the same thing
somehow I dropped a negative here this
should say do not call s to be future
get or STD future wait when the
originating task or any subordinate task
is on the same queue even if it's a
concurrent queue or a thread pool that's
the rule
for using them safely okay which means
that I can issue a task within C++ with
async from my main thread and call yet
on the future okay but I can't call get
on a future from anything that was
queued through STD async
okay so you see this problem in every
tasking system it will have some kind of
warning like this so I could have put a
whole list of them up there but this is
from lib dispatch I've kind of been
using live dispatch as an example you
should never call dispatch sinc what's
just batch sync it's the equivalent of
queueing something into the queue and
then waiting for it immediately right
never call dispatch synch from a task
that is executing in the same queue that
you are planning to pass to the function
this is particularly important for
serial queues which are guaranteed
deadlock but should also be avoided for
concurrent queues okay
why because depending on how the stars
align it will be a deadlock now how did
I discover this well taking a heavily
threaded piece of code and taking it
down to one thread it was very apparent
where all the dead locks were right
there but then we also realized that hey
if we took it to mobile where I also
took Lightroom engine to mobile with two
cores we were seeing a lot more dead
locks
okay when we actually finally figured
out what in the world the problem was we
realized that even on eight cores and
sixteen core machines the system under
heavy load would occasionally deadlock
it was just rare enough that you
wouldn't see it okay so so but in our
bug backlog when we went back through it
was like oh yeah this this is that
problem this deadlock is that problem
okay so a huge thing okay so this is
what a dot then continuation looks like
right we can calculate Fibonacci and
then once the Fibonacci gets we can
serialize it we're going to go ahead and
put our weight in here because we're
executing this on the main thread not
over over on our async it's useful at
least for debugging so so now we can do
it and then print it
okay so bins are are the equivalent of
completion handlers okay and in the
standard committee a lot of times
there's debate over whether or not the
futures are the right model or whether
or not completion handlers are the right
model okay so so conceptually the two
are interchangeable okay so the
advantage with the completion handler is
that there's no need to synchronize
between invocation and and and setting
it because it was set in advance the
completion handler gets passed into the
function okay whereas a future there's a
little bit of synchronization that
happens because you don't know well your
asynchronous task is setting when you're
setting the value is somebody attaching
a continuation at that exact moment in
time okay so there's a small amount of
synchronization that has to happen but
the advantage with the future is that
you don't have to know where the results
are going in advance right so so those
are the trade-offs so you can actually
express one in terms of the other and
vice versa if if you if you build build
a system correctly so my answer is
always when people say do we want
futures or completion routines I'm like
yes that is my answer I want both okay
now the other thing you find when you
start to construct these things is we
need joins okay so what I want to do is
I have two futures and when both of
those futures are available then I want
to issue the nest nest next task now a
lot of people have the wrong mental
model about this what they think is is
then I will have my continuation I get
get one result in and then I will block
and wait for the other result but that's
not how joins work at all
what happens if you keep a count of how
many things that you're waiting for that
are pending okay and when I get one
result in I issue the task or when I get
one result in I'm okay when I might when
my count goes to zero then it's the it's
the setting the result decrement
the count to zero that causes the next
task in the chain to fire so you're
never blocking and waiting you're just
counting down
okay so this is what it looks like with
this is using boost when all I really do
not like the syntax here and that a win
all right I'm taking a win all of two
futures and then a continuation and my
argument F coming in there is a tuple is
a future of a tuple two futures which is
why you get the in order to print my end
I've got get get x get get right right F
is a future tuple futures and the result
of this is a bunch of digits we're not
going to print it so the other thing you
want to be able to do is split
so here now this is still the boost
slash proposed C++ 17 model so it seems
like the way you would do a split is
just attach to continuations to your
future so I've got X and then I want to
say then go multiply it by two and also
when I have X divide it by five that
looks like a split
yeah just abort okay the reason is the
first then actually consumes X so trying
to add hacks a second X a second then is
a cig abort that's not a particularly
friendly thing now a shared future lets
me copy them around okay and do multiple
get operations but it still doesn't let
me do multiple event operations okay
well that's horrible so now if I've got
a shared future in my hand I don't know
whether or not I can attach a
continuation to it okay so my opinion
the future should behave as a regular
type it's a token to the value so you
should be able to to copy them to get a
split but in absence of that we could
write a split so let's see how we would
do that okay so this is what it would
look like to call what we're going to do
is we're going to take X we're going to
split it okay which is going to leave
our X as a future and give us a new one
to which we can attach our dot then okay
so we can split it and then we can
attach it and if we wanted to keep going
we keep splitting okay we could just
keep splitting X
so and then we can write this and that
will print that right the first one beam
times two and the second one divided by
fifteen okay so let's see how we would
build that well first we've been talking
about futures but we haven't been
talking about the other in the futures
we can c++ is known as a promise okay so
a promise is the sending side of a
future so if we create a promise X then
we can get a future out of it and if we
set the value 42 on our promise then we
will get the value out of our future
okay so so that's the pairing there so
internally async STD async is is
creating a promise that we'll turn our
42 okay so here's how we're going to
write our split we'll walk through it
here okay when we initially come in
there is a promise somewhere that's in
gray a gray circle okay which is going
to set our future which is X okay so
we've got a promise somewhere and we're
hanging on to X so we're passing in a
reference to X now the first thing we're
going to do is we're going to move that
future out of X to get it out of the way
okay so the promise still points to the
move future now which is in temp then
we're going to construct a new future
and get a new promise for it and put the
new promise into X okay so now we've
rewritten X to be our new future okay
then what we're going to do is we're
going to take our temp
okay which is our future we're going to
put it in to our lambda here so now it's
underscore temp we're going to attach a
continuation to it which is going to
return our results we're binding P right
we are moving the promise we created in
the dotted line there eight so we we
took P we moved it over into underscore
P okay
that's the result we have the original
promise which points into our underscore
temp future okay which has a
continuation which is going to set the
value on T my underscore P set value
okay and it's going to return the value
right so there's the copy right there so
that's how you implement the splits okay
then we do the same thing to split the
exception right so we want the exception
to also go down both paths okay so if we
have an exception then we're going to
send an exception down both paths you're
not sure we write a split
okay now here's a tough thing this is
something I want with futures but
they're not in C++ 17 and I don't know
of a good way to compose them into the
sequence plus 17 futures basically what
I want is now that I can split split my
futures right right and I can join them
so I can build these complicated graphs
is when the last piece of code see I
think I have a graph here when the last
piece of code lets go of a future like
the top right one there okay then if the
task associated with that piece of code
hasn't started to execute then I want it
to not execute and to free the resources
okay which then leaves two futures which
are no longer needed and so those
futures go away and I'm left with this
right so I eliminated one task from my
system that's executing now when you're
developing interactive software this
comes up a lot the user does something
and then the user does something else
and you're like okay well forget about
that old thing you don't want to keep
doing it so a cancellation model is nice
because that wasn't there and because
even the boost futures with
continuations will deadlock on a single
threaded system which I consider a bug
they shouldn't but they do it's a part
of their their their implementation I
ended up writing this experimental
library which is available on that
website and I've got futures in there
that support multiple continuations and
they can be copied they're optimized for
our values so you can have move only
types with them it supports joins for
winning and win all it has cancellation
on destruction as well as an explicit
recess reset and it has a detach so you
can detach a future meaning that but now
I can destruct this but I still want
whatever it's associated with to
continue processing right just like you
would detach a thread and say keep going
even though though I'm destructing my my
toe
and hold on to it so I put this library
out there and I have to thank Felix
petrosini who's in Germany who's been
just a massive contributor and so when I
wrote it I wrote it to the extent that I
needed it and going to show it to the
standard folks and say here's some
issues with the direction you're going
and a Felix is really kind of flushed it
out and and our goal now is to release
kind of a 1.0 version sometime earlier
this year that's there but futures
aren't enough right so the next thing is
to talk about channels and I am probably
way over time aren't I at this point I'm
not seen any hands
since this took me a little longer to go
through then I wanted to I will stop
here Kevin's already talked some about
the basic concepts of channels channels
are also in my library so if you want me
to to discuss them just see me outside
of this and the last thing I will just
flip through slides very quickly but I
wanted to finish on that slide won't let
me go too quick all these great
animations
channels it is this what we're building
here is dependency graphs within the
code and it turns out there's a lot of
information in the graph right when
you're building channels and you're
building futures you're building
dependency graphs and there's a lot of
information that's in the graph but
these are are what I call incidental
data structures right there isn't is no
place within your code where it's like
here's my dependency graph okay which
makes it makes it nearly impossible to
mine that graph for values especially if
you want information about all the
possible states of that graph
so maybe constructing these dependency
graphs is is not the right approach
instead maybe we should be actually
writing the graph to script descriptions
and and compiling those and statically
validate them and I've got a whole body
of work and a bunch of published papers
on something called property models
which is exactly that which is how do
you describe dependency graphs and flows
between states and reason about them
outside so if anybody wants to talk on
that way outside the scope of this talk
but I'd be happy to and there are some
slides there for my papers and
presentations and the library and what
we're talking about here largely is
communicating sequential processes which
is which is from Tony Hoare and his book
is now out of publication but freely
available online and so that's the link
to CSP so thank you and sorry for not
paying attention to the time and running