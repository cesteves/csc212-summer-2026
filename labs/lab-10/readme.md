# Lab 10 - Graphs: Adjacency List, Save/Load, Cycle Detection, and Shortest Path

## Overview

This lab builds a small **graph** class from scratch and uses it to explore four
core graph skills: representing a graph in memory, persisting it to disk, detecting
cycles with **depth-first search (DFS)**, and finding a shortest path with
**breadth-first search (BFS)**.

The `Graph` interface is fixed for you in `graph.h`. Your job is to implement it in
`graph.cpp`, organized into four parts:

- **Part 1**: Build the adjacency list representation (constructor, `add_edge`, `vertex_count`, `neighbors`)
- **Part 2**: Save a graph to a text file and load one back (`save_to_file`, `load_from_file`)
- **Part 3**: Use DFS to detect whether a graph contains a cycle (`has_cycle`, `_has_cycle_visit`)
- **Part 4**: Use BFS to find a shortest path between two vertices (`shortest_path`)

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

## Part 2: Saving and Loading a Graph

A graph you build in memory disappears the moment your program exits. To hand a
graph off between programs (or just to save one for later), you'll write it to a
plain text file as an **edge list**, and read it back the same way.

### File format

```text
5
0 1
0 2
1 3
2 3
3 4
```

- The **first line** is the number of vertices.
- Every line after that is a single undirected edge, written as two integers `u v`
  separated by a space.
- Each undirected edge appears **exactly once** in the file (never both `u v` and
  `v u` for the same edge).

The diamond graph from Part 1 would be written exactly as shown above.

### Your work: save / load

Implement the two methods marked `// TODO` under **Section 2** in `graph.cpp`.

- **`void save_to_file(const std::string& filename) const`**
Opens an `std::ofstream` on `filename`, writes `vertex_count()` on the first line,
then writes one `u w` line for every edge - for each vertex `u`, for every neighbor
`w` of `u` with `w >= u`. The `w >= u` check is what keeps you from writing the same
edge twice.

- **`static Graph load_from_file(const std::string& filename)`**
Opens an `std::ifstream` on `filename`, reads the vertex count from the first line
to construct the `Graph`, then reads `u v` integer pairs for the rest of the file -
calling `add_edge(u, v)` for each one - until the stream is exhausted.

> [!NOTE]
> `while (fin >> u >> v)` is a clean way to read pairs of integers until end-of-file:
> the `>>` extraction fails (and the loop stops) once there's nothing left to read.

> [!WARNING]
> `load_from_file` is `static` - it doesn't run on an existing `Graph`, it *builds and
> returns* a new one. Call it as `Graph::load_from_file("path.txt")`, not on an
> instance.

## Part 3: Cycle Detection (DFS)

A **cycle** is a path that starts and ends at the same vertex without reusing any
edge along the way. Detecting cycles matters for all sorts of practical questions:
does this dependency graph have a circular dependency? Does this course
prerequisite chart contain a contradiction?

```text
0 --- 1          0 --- 1
      |          |     |
2 --- 3 --- 4    2 --- 3 --- 4
(no cycle)       (cycle: 0-1-3-2-0)
```

### The idea

DFS naturally finds cycles because of one simple rule: while exploring, if you ever
reach a vertex that is **already visited** through an edge that **isn't the one you
just arrived on**, you've found a cycle. Reaching your own parent back through the
same edge you came from doesn't count - that's just backtracking, not a cycle.

This is why `_has_cycle_visit` needs to know its `parent`: without it, DFS could not
tell the difference between "I looped back to an ancestor" (a cycle) and "I stepped
back to the vertex I came from" (completely normal).

> [!NOTE]
> A graph doesn't have to be fully connected. `has_cycle()` must check **every**
> vertex, not just the ones reachable from vertex `0` - a graph can be cycle-free
> near vertex `0` but contain a cycle in some entirely separate component.

### Your work: cycle detection

Implement the two methods marked `// TODO` under **Section 3** in `graph.cpp`.

- **`bool _has_cycle_visit(int v, int parent, std::vector<bool>& visited) const`** *(private helper)*
Marks `v` visited. For every neighbor `n` of `v`: if `n` hasn't been visited, recurse
into it (with `v` as the new parent) and return `true` if that recursive call finds
a cycle; otherwise, if `n` *has* been visited and `n != parent`, a cycle has been
found - return `true`. Returns `false` if nothing triggers a cycle.

- **`bool has_cycle() const`**
Sets up a `visited` vector (all `false`, sized to `vertex_count()`), then loops over
every vertex; for any vertex not yet visited, calls `_has_cycle_visit(vertex, -1,
visited)`. Returns `true` the moment any call reports a cycle, and `false` if every
component finishes cycle-free.

## Part 4: Shortest Path (BFS)

Once a graph is loaded into memory, one of the most common questions to ask is:
what's the fewest number of edges I need to cross to get from vertex `A` to vertex
`B`? Because BFS explores level by level - every vertex at distance 1 before any
vertex at distance 2, and so on - the first time it reaches a vertex is guaranteed
to be by a shortest path.

```text
0 --- 1
|     |
2 --- 3 --- 4
```

The shortest path from `0` to `4` has to pass through `3` (there's no shorter route),
so it looks like `0, 1, 3, 4` or `0, 2, 3, 4` - either is correct, since both are
length 4.

### The idea

BFS with a `std::queue<int>` visits vertices in the order they're discovered. To
recover the *path*, not just the *distance*, track a `parent` array alongside
`visited`: whenever you discover vertex `n` for the first time while processing
vertex `v`, record `parent[n] = v`. Once BFS finishes, walk backwards from `end`
through `parent` until you reach `start`, then reverse what you collected.

> [!WARNING]
> Mark a vertex visited **the moment you push it onto the queue**, not when you pop
> it. If you wait until popping to mark a vertex visited, the same vertex can be
> pushed onto the queue multiple times before it's ever processed - and the first
> `parent` you recorded for it (which is what makes the path shortest) can get
> overwritten incorrectly.

### Your work: shortest path

Implement the method marked `// TODO` under **Section 4** in `graph.cpp`.

- **`std::vector<int> shortest_path(int start, int end) const`**
If `start == end`, returns a one-element vector containing just that vertex. Otherwise,
sets up `visited` and `parent` vectors (sized to `vertex_count()`, with every entry of
`parent` starting at `-1`) and a `std::queue<int>`. Marks `start` visited and pushes it.
Runs a standard BFS, recording `parent[n] = v` for every newly-discovered vertex `n`.
If `end` was never visited, returns an empty vector - there's no path. Otherwise,
reconstructs the path by following `parent` backwards from `end` to `start`, then
reverses it before returning.

## Running the tester

`tester.cpp` is already complete. Compile it together with your `graph.cpp` and run it
after implementing each part, not only at the end.

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

save/load tests
[PASS] save/load: round trip preserves vertex_count (2/2 pts)
[PASS] save/load: round trip preserves every vertex's neighbor set (3/3 pts)
[PASS] save/load: isolated vertex stays isolated after round trip (2/2 pts)
[PASS] load: hand-written edge list parses correctly (3/3 pts)

cycle detection tests
  [case] tree has no cycle: correct
  [case] diamond graph has a cycle: correct
  [case] single vertex, no edges, has no cycle: correct
  [case] disconnected forest has no cycle: correct
  [case] cycle in one component of a disconnected graph is found: correct
[PASS] has_cycle: correctly classifies every case above (all must be correct) (10/10 pts)

shortest path tests
[PASS] shortest_path: start == end returns just that vertex (2/2 pts)
[PASS] shortest_path: diamond graph 0->4 has correct length and endpoints (3/3 pts)
[PASS] shortest_path: every step of the returned path follows a real edge (2/2 pts)
[PASS] shortest_path: distinguishes an unreachable vertex from a reachable one in the same graph (3/3 pts)

14/14 tests passed.
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
