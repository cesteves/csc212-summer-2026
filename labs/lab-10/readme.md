# Lab 10 - Graph Traversal: Adjacency List, DFS, and BFS

## Overview

This lab builds a small **graph** class from scratch and uses it to explore the two
fundamental traversal strategies: **depth-first search (DFS)** and **breadth-first
search (BFS)**. By the end you will understand how a graph is represented in memory,
how each traversal decides which vertex to visit next, and why that single decision
produces two very different exploration orders over the same graph.

The `Graph` interface is fixed for you in `graph.h`. Your job is to implement it in
`graph.cpp`, organized into three parts:

- **Part 1**: Build the adjacency list representation (constructor, `add_edge`, `vertex_count`, `neighbors`)
- **Part 2**: Implement depth-first search (`dfs`)
- **Part 3**: Implement breadth-first search (`bfs`)

`tester.cpp` is provided **complete** - you do not write or modify any tests. Compile
it against your `graph.cpp` and run it as you go to check your work.

> [!CAUTION]
> For this lab, **AVOID** using LLMs to **generate code or solutions** for the exercises. In
> particular, we ask you to explicitly **DISABLE** Copilot or similar tools in your IDE.
> You may ask specific questions to LLMs about concepts or language syntax, but we
> strongly discourage using LLMs to write code for you.

> [!IMPORTANT]
> **Read each concept section fully before writing any code.** The design decisions here are
> not arbitrary - understanding *why* each piece exists is the point of the exercise.
> Discuss every method contract with your group before writing a single line.

## Files in this lab

| File | Do you edit it? | Purpose |
|------|------------------|---------|
| `graph.h` | No | Declares the `Graph` class - its interface and contracts |
| `graph.cpp` | **Yes** | Where you implement every method declared in `graph.h` |
| `tester.cpp` | No | Complete unit tester; run it to check your `graph.cpp` |

`graph.h` and `tester.cpp` are already correct and complete. Read them closely, but
the only file you should be editing is `graph.cpp`.

## Part 1: The Adjacency List

### What is a graph?

A **graph** is a collection of **vertices** (also called nodes) connected by **edges**. In
this lab, vertices are simply labeled `0, 1, 2, ..., n-1`, and every edge is **undirected**:
if vertex `u` is connected to vertex `v`, then `v` is also connected to `u`.

### Representing a graph with an adjacency list

An **adjacency list** stores, for every vertex, the list of vertices it is directly
connected to. This lab represents the whole graph as a single `std::vector` of chains,
one chain per vertex:

```text
0 --- 1
|     |
2 --- 3 --- 4
```

```text
adj[0] -> [1, 2]
adj[1] -> [0, 3]
adj[2] -> [0, 3]
adj[3] -> [1, 2, 4]
adj[4] -> [3]
```

Looking up "who is vertex 3 connected to?" is just reading `adj[3]`, no scanning every
edge in the graph required. This is why adjacency lists are the default representation
for most graph algorithms: they use space proportional to the number of edges, and they
make "give me the neighbors of `v`" a single, cheap lookup.

> [!NOTE]
> An alternative representation is the **adjacency matrix**, an `n x n` grid of booleans
> where entry `(i, j)` is `true` if an edge connects `i` and `j`. Matrices make "are `u`
> and `v` connected?" a single lookup but waste space on graphs that don't have very
> many edges relative to the number of vertices. This lab uses the list representation.

### Containers you'll use

| Member | Type | Role |
|--------|------|------|
| `adj` | `std::vector<std::vector<int>>` | Adjacency list; `adj[i]` is the chain of neighbors for vertex `i` |

### Your work: the adjacency list

Implement every method marked `// TODO` under **Section 1** in `graph.cpp`. Their
contracts are documented in `graph.h`:

- **`Graph(int num_vertices)`**
Constructs a graph with `num_vertices` vertices and no edges. `num_vertices` is always `>= 0`.

- **`void add_edge(int u, int v)`**
Adds an undirected edge between `u` and `v`. Since the graph is undirected, `v` must be
added to `u`'s chain **and** `u` must be added to `v`'s chain. You do not need to guard
against duplicate edges or self-loops.

- **`int vertex_count() const`**
Return the number of vertices in the graph.

- **`const std::vector<int>& neighbors(int v) const`**
Return the chain of vertices adjacent to `v`.

## Part 2: DFS (Depth-First Search)

**Depth-first search** explores as far as possible along one path before backtracking.
Starting from a vertex, it picks an unvisited neighbor, moves there, and immediately
repeats the process from the new vertex - only backing up once it hits a dead end
(every neighbor already visited).

```text
0 --- 1
|     |
2 --- 3 --- 4
```

Starting a DFS at vertex `0` might visit `0, 1, 3, 2, 4`: it dives from `0` into `1`,
from `1` into `3`, then keeps diving into `3`'s unvisited neighbors before ever
returning to explore anything back near `0`.

DFS is naturally written **recursively**: visiting a vertex means marking it visited,
recording it, and then recursively visiting every unvisited neighbor.

> [!NOTE]
> Because recursion uses the **call stack** to remember where to backtrack to, DFS is
> sometimes described as "stack-based" even when written recursively - the call stack
> *is* the stack.

### Your work: DFS

Implement the two methods marked `// TODO` under **Section 2** in `graph.cpp`.

- **`void _dfs_visit(int v, std::vector<bool>& visited, std::vector<int>& order) const`** *(private helper)*
Marks `v` visited, appends it to `order`, then recursively calls itself on every
neighbor of `v` that has not yet been visited.

- **`std::vector<int> dfs(int start) const`**
Sets up a `visited` vector (all `false`, sized to `vertex_count()`) and an empty
`order` vector, kicks off the traversal with `_dfs_visit(start, visited, order)`, and
returns `order`. `start` is always a valid vertex.

## Part 3: BFS (Breadth-First Search)

**Breadth-first search** explores level by level: it visits every neighbor of the
start vertex before visiting any of *their* neighbors, then every vertex two edges
away before any vertex three edges away, and so on.

Starting a BFS at vertex `0` in the same graph visits `0`, then both of `0`'s direct
neighbors (`1` and `2`, in whatever order they appear in `adj[0]`), then `3` (reachable
through either of them), and finally `4`. Every vertex at distance `1` from the start
is visited before any vertex at distance `2`.

BFS is naturally written **iteratively** using a `std::queue<int>` as the frontier:
vertices are visited in the order they are discovered, first-in-first-out.

> [!NOTE]
> `std::queue` supports `push`, `front`, and `pop`. Pushing a vertex adds it to the
> back of the line; `front()` and `pop()` together let you process and remove the
> vertex currently at the head of the line. Remember to `#include <queue>` at the
> top of `graph.cpp` - it isn't included for you.

> [!WARNING]
> Mark a vertex visited **the moment you push it onto the queue**, not when you pop
> it. If you wait until popping to mark a vertex visited, the same vertex can be
> pushed onto the queue multiple times before it's ever processed.

### Your work: BFS

Implement the method marked `// TODO` under **Section 3** in `graph.cpp`.

- **`std::vector<int> bfs(int start) const`**
Sets up a `visited` vector, an empty `order` vector, and a `std::queue<int>`. Marks
`start` visited and pushes it onto the queue. While the queue is not empty: pops the
front vertex, appends it to `order`, and pushes every one of its unvisited neighbors
onto the queue (marking each visited as it's pushed). Returns `order`.

## Running the tester

`tester.cpp` is already complete. Compile it together with your `graph.cpp` and run it
after implementing each method, not only at the end.

```bash
# Compile
$ g++ -std=c++17 -Wall -Werror -o tests graph.cpp tester.cpp

# Run
$ ./tests
```

> [!WARNING]
> The autograder compiles its **own protected copies** of `graph.h` and the tester
> against **your submitted `graph.cpp`**. Editing `graph.h` or `tester.cpp` locally is
> fine for experimenting, but only `graph.cpp` is graded - any changes you make to the
> other two files have no effect on your score. Make sure your implementation satisfies
> the contracts documented in `graph.h`, not just the specific checks in `tester.cpp`.

**Expected output (all passing):**

```
adjacency list tests
[PASS] vertex_count: matches constructor argument (2/2 pts)
[PASS] add_edge: v appears in neighbors(u) (2/2 pts)
[PASS] add_edge: u appears in neighbors(v) (undirected) (2/2 pts)
[PASS] neighbors: vertex with no edges is empty (2/2 pts)
[PASS] add_edge: multiple edges accumulate correctly (2/2 pts)

DFS tests
[PASS] dfs: start vertex is visited first (2/2 pts)
[PASS] dfs: visits every vertex in a connected graph (3/3 pts)
[PASS] dfs: each vertex appears exactly once (2/2 pts)
[PASS] dfs: unreachable vertices are excluded (3/3 pts)

BFS tests
[PASS] bfs: start vertex is visited first (2/2 pts)
[PASS] bfs: visits every vertex in a connected graph (3/3 pts)
[PASS] bfs: each vertex appears exactly once (2/2 pts)
[PASS] bfs: unreachable vertices are excluded (3/3 pts)

edge case tests
[PASS] edge: single-vertex graph, dfs visits only itself (2/2 pts)
[PASS] edge: single-vertex graph, bfs visits only itself (2/2 pts)
[PASS] edge: dfs and bfs visit the same set of vertices (3/3 pts)
[PASS] edge: bfs respects distance order (closer before farther) (3/3 pts)

17/17 tests passed.
Score: 40/40 pts
```

## Submission

Submit **`graph.cpp`** to Gradescope. That is the only file you need to hand in - `graph.h`
and `tester.cpp` are provided by the course and are not graded even if you include them.
Your submission must compile cleanly with `-std=c++17 -Wall -Werror` against the
course's `graph.h` and tester.

Code should be well-formatted and easy to read. Full credit requires **attendance**,
**collaboration with your group**, and **active participation**. No remote
submissions will be accepted. If you have questions, ask the instructor or TA.
