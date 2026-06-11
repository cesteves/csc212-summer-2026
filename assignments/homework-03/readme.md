# Homework Assignment 3

In this assignment, you will practice tracing and analyzing dynamic arrays, stacks, queues, and deques.

## Learning Objectives

By the end of this assignment, you should be able to:

1. Trace the execution of programs that use stacks, queues, and deques.
2. Analyze the asymptotic time complexity of common dynamic array operations.
3. Analyze the asymptotic time complexity of programs that use stacks, queues, and deques.
4. Select between stacks, queues, and deques to efficiently solve computational problems.

## Grading

This assignment is graded out of $$100$$ points.

## Answer Formats

**Short answer questions are autograded. You must follow the format specified below to receive credit.** 

If the problem asks for a single integer, provide your answer as a single integer without whitespace. For example, `10`.

If the problem asks for a comma-delimited list of integers, provide your answer as a comma-delimited list without 
whitespace. For example, `3,1,4,0,12`.

## Standard Library

For some of the programming problems, we used special data structures in place of the standard library data structures.
These data structures operate the same as the corresponding standard library data structure, but offer some additional
information about their implementation:

* `grow_by_one_vector` is a grow-by-one dynamic array with initial capacity $$1$$. The capacity never decreases. 
   Otherwise, it works the same as `std::vector`.
* `grow_by_doubling_vector` and `vector` are dynamic arrays with a growth factor of $$2$$ and initial capacity $$1$$. 
   The capacity never decrease. Otherwise, they work the same as `std::vector`.
* `queue` is a queue implemented with a fixed size array, with prepend to the front and append to the back. Otherwise,
   it works the same as `std::queue`.
* `stack` is a stack implemented with a fixed size array. Otherwise, it works the same as `std::stack`.
* `deque` is a deque implemented with fixed size array. Otherwise, it works the same as `std::deque`.

**The performance of these data structures is defined by their description here, not the C++ documentation.**