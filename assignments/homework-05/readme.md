# Homework Assignment 5

In this assignment, you will practice tracing, analyzing priority queues and binary heaps.

## Learning Objectives

By the end of this assignment, you should be able to:

1. Trace how common dynamic array and binary heaps operations change the data structure's internal state.
2. Trace the execution of programs that use stacks, queues, deques and priority queues.
3. Analyze the asymptotic time complexity of common dynamic array and binary heap operations.
4. Analyze the asymptotic time complexity of programs that use stacks, queues, deques, and priority queues.
5. Select between stacks, queues, deques, or priority queues to efficiently solve computational problems.

## Grading

This assignment is graded out of $$100$$ points. However, we provide $$145$$ points' worth of problems.

**You are not expected to complete every problem!**

## Answer Formats

**Short answer questions are autograded. You must follow the format specified below to receive credit.** 

If the problem asks for a single integer, provide your answer as a single integer without whitespace. For example, `10`.

If the problem asks for a comma-delimited list of integers, provide your answer as a comma-delimited list without whitespace. For example, `3,1,4,0,12`.

## Standard Library

For some of the programming problems, we used special data structures in place of the standard library data structures.
These data structures operate the same as the corresponding standard library data structure, but offer some additional
information about their implementation:

* `min_priority_queue` is a min-priority queue implemented with a binary min-heap. The underlying array is fixed capacity and never grows or shrinks. Otherwise, it works the same as `std::priority_queue`.
* `max_priority_queue` is a max-priority queue implemented with a binary max-heap. The underlying array is fixed capacity and never grows or shrinks. Otherwise, it works the same as `std::priority_queue`.
* `make_min_heap` builds a binary min-heap in $$\Theta(n)$$ time. Otherwise, it works the same as `std::make_heap`.
* `make_max_heap` builds a binary max-heap in $$\Theta(n)$$ time. Otherwise, it works the same as `std::make_heap`.
* `pop_min_heap` pops the minimum element of a binary min-heap. Otherwise, it works the same as `std::pop_heap`.
* `pop_max_heap` pops the maximum element of a binary max-heap. Otherwise, it works the same as `std::pop_heap`.
* `top_min_heap` returns the top element of a binary min-heap.
* `top_max_heap` returns the top element of a binary max-heap.
* `queue` is a queue implemented with a fixed size array, with prepend to the front and append to the back. Otherwise,
   it works the same as `std::queue`.
* `stack` is a stack implemented with a fixed size array. Otherwise, it works the same as `std::stack`.
* `deque` is a deque implemented with fixed size array. Otherwise, it works the same as `std::deque`.

**The performance of these data structures is defined by their description here, not the C++ documentation.**

## Submission

**Before you submit, attach a PDF containing your work for every question you answered:**

|files|

It doesn't need to be neat, we just want to be able to understand your process.

Note that you can submit as many times as you want before the due date!

## Confirmation

I have read and understand the above.

(X) Yes
( ) No