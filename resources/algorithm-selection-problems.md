# Algorithm Selection Problems
Note: This is not an exhaustive sample of the problem types that will apear on your final exam. The exam is cumulative and will contain many problems similar to those on Midterm Exams 1 & 2. 

### Problem 1: The Browser History Cache
You're designing an LRU (Least Recently Used) cache for a browser that stores the last N visited pages. You need `get(key)` and `put(key, value)` to run in O(1) average time, and eviction must remove the least-recently-used item.

**Which data structure(s) would you use?**

<details>
<summary>Solution</summary>

**Answer:** A hash map + doubly linked list combination.

**Reasoning:**
- The hash map gives O(1) lookup from key → node.
- The doubly linked list maintains recency order: most-recently-used at the head, least-recently-used at the tail. Doubly linked (not singly) because you need O(1) removal of an arbitrary node (you need the `prev` pointer).
- A singly linked list would force O(n) removal since you'd have to walk from the head to find the previous node.
- An array-based structure would require O(n) shifting on every access to maintain order.
- A plain hash map alone has no notion of order, so it can't support eviction by recency.

**Complexity:** O(1) for both `get` and `put`; O(n) space for n cached items.
</details>

### Problem 2: The Live Leaderboard
A game server needs to repeatedly report the current top score, insert new scores as they come in, and occasionally remove a player's score. There can be millions of insertions per minute.

**Which data structure fits best?**

<details>
<summary>Solution</summary>

**Answer:** A binary heap (max-heap), possibly paired with a hash map if removal-by-player-id is required.

**Reasoning:**
- You need repeated access to the minimum/maximum with fast insert - that's the signature use case for a heap: O(log n) insert, O(1) peek at the extreme value, O(log n) extract.
- A sorted array/list would give O(1) peek but O(n) insert - too slow at the stated insertion rate.
- A balanced BST (e.g., a red-black tree) would also work (O(log n) insert, O(log n) min/max) but has more overhead and complexity than needed since you don't need full ordered traversal or range queries - just the extreme value.
- If you need to *remove an arbitrary player's score* (not just the max) in better than O(n), you augment the heap with a hash map from player ID → heap index, since a plain heap doesn't support fast arbitrary deletion.

**Complexity:** O(log n) insert/delete, O(1) find-max.
</details>

### Problem 3: Static Dictionary Lookup
You're given a fixed, unchanging list of 2 million valid English words at startup. Your program must repeatedly check whether a query string is a valid word, as fast as possible, and memory is not a major constraint.

**Which data structure fits best?**

<details>
<summary>Solution</summary>

**Answer:** A hash set (hash table with no values, just keys).

**Reasoning:**
- The data is static (no updates after load) and you only need membership queries (not ordering, not prefix queries) - this is exactly what a hash set optimizes for: O(1) average lookup.
- A sorted array + binary search would give O(log n) lookup - correct but strictly slower than O(1) hashing when insertion order/updates aren't a concern.
- A trie would also answer membership queries and would be the better choice if you *also* needed prefix search (autocomplete) - see Problem 14. Since the problem only asks for exact-match lookup, the trie's extra memory overhead (a node per character) isn't justified.
- A balanced BST is unnecessary since no ordered traversal or range query is needed.

**Complexity:** O(1) average lookup, O(n) space.
</details>

### Problem 4: Autocomplete Search Bar
You're building a search bar that, as the user types each character, must instantly return all stored strings that start with the characters typed so far. There are 500,000 stored strings.

**Which data structure fits best?**

<details>
<summary>Solution</summary>

**Answer:** A trie (prefix tree).

**Reasoning:**
- The core operation - "give me everything with this prefix" - is exactly what a trie is built for: you walk down the trie following the typed characters (O(length of prefix)), then all completions live in the subtree rooted there.
- A hash set can check exact membership in O(1) but has no efficient way to enumerate all strings sharing a prefix - you'd have to scan all 500,000 entries.
- A sorted array with binary search can find the *range* of strings matching a prefix in O(log n + k) (k = number of matches), which is a reasonable alternative, but insertion into a sorted array is O(n), whereas a trie supports O(length) insertion. If the dictionary changes often, the trie wins; if it's fully static, a sorted array is a lighter-weight alternative.

**Complexity:** O(L) per character typed (L = prefix length), independent of the number of stored strings.
</details>

### Problem 5: Sorting a Nearly-Sorted Log File
You have an array of 10,000 timestamped log entries that arrived almost in order, except a few entries are out of place by at most 3 positions (e.g., due to network jitter).

**Which sorting algorithm is the best choice?**

<details>
<summary>Solution</summary>

**Answer:** Insertion sort.

**Reasoning:**
- Insertion sort's runtime is O(n + d), where d is the number of inversions (out-of-order pairs). When the array is nearly sorted, d is small, so insertion sort runs close to O(n) - much faster in practice than its O(n²) worst case.
- Merge sort and quicksort are O(n log n) regardless of existing order, so they can't exploit the "nearly sorted" structure the way insertion sort can.
- Selection sort is always O(n²) regardless of input order (it always scans for the minimum), so it does *not* benefit from near-sortedness - it's a common trap to think "it's simple, so it's fine here."

**Complexity:** O(n + d) ≈ close to O(n) here, where d is small.
</details>

### Problem 6: Sorting an Array of Strings
You need to sort 50,000 arbitrary-length strings once, and then perform millions of binary searches against that sorted list. You have no special constraints on memory or stability.

**Which sorting algorithm is the best choice?**

<details>
<summary>Solution</summary>

**Answer:** Any solid general-purpose O(n log n) comparison sort - in practice, introsort/quicksort-hybrid (what most standard libraries use, e.g., C++'s `std::sort`), or mergesort if stability mattered.

**Reasoning:**
- This is a case where there's no special structure to exploit (not nearly sorted, not small key range, not a linked list, not too big for memory) - so the general-purpose default is correct, and reaching for a specialized algorithm (counting sort, radix sort) would be *wrong* here since string keys don't have a small fixed range.
- Since you sort once and then query millions of times, sort *time* barely matters relative to correctness and simplicity - this is a signal that "use the standard library sort" is the right engineering answer, not a hand-rolled algorithm.
- Stability isn't required here since no secondary key is mentioned, so unstable but fast quicksort variants are perfectly fine.

**Complexity:** O(n log n), one-time cost, dominated in practice by the O(log n) cost of each of the millions of subsequent binary searches.
</details>

### Problem 7: Shortest Hop Count in a Social Network
Given a social network graph (unweighted, undirected), find the minimum number of "hops" (friend connections) between two users.

**Which graph algorithm is the best choice?**

<details>
<summary>Solution</summary>

**Answer:** Breadth-First Search (BFS).

**Reasoning:**
- BFS explores the graph in layers of increasing distance from the source, so the first time it reaches the target, it has necessarily found the shortest path in terms of edge count - this only works because all edges are unweighted (weight 1).
- DFS explores as deep as possible along one branch before backtracking; it will find *a* path but has no guarantee it's the shortest one.
- Dijkstra's algorithm would also give the correct answer (it reduces to BFS when all weights are equal) but its priority-queue overhead (O(E log V)) is unnecessary machinery when plain BFS solves it in O(V + E).

**Complexity:** O(V + E).
</details>

### Problem 8: Fastest Route on a Road Network
Given a road network where edges are weighted by driving time (all non-negative), find the fastest route from A to B.

**Which graph algorithm is the best choice?**

<details>
<summary>Solution</summary>

**Answer:** Dijkstra's algorithm.

**Reasoning:**
- Edge weights are non-negative, which is exactly Dijkstra's precondition - it greedily finalizes the shortest known distance to the closest unvisited node each step, and that greedy choice is only guaranteed correct when weights can't be negative (a later negative edge could otherwise undercut an already-finalized "shortest" distance).
- BFS won't work directly since edges are weighted - BFS assumes all edges cost the same.
- Bellman-Ford would also give a correct answer since it handles non-negative weights as a special case, but it's O(VE), strictly slower than Dijkstra's O(E log V) with a binary heap - so it's the wrong choice when you *know* there are no negative edges. Bellman-Ford's extra generality (handling negative edges) isn't needed here, so it's paying an unnecessary cost.
- (If you needed a single source-destination query on a huge map, A* with a good heuristic like straight-line distance would be even better in practice, but plain Dijkstra is the right baseline answer.)

**Complexity:** O(E log V) with a binary heap.
</details>
