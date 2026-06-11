# Midterm 1 Study Guide

## 1. Asymptotic Analysis

### What it means

Big-O, Ω, and Θ describe how an algorithm's resource use *scales* as input grows. Constants and lower-order terms are ignored.

| Notation | Meaning | Informal reading |
|----------|---------|-----------------|
| f(n) = O(g(n)) | f grows **no faster** than g (upper bound) | "at most" |
| f(n) = Ω(g(n)) | f grows **no slower** than g (lower bound) | "at least" |
| f(n) = Θ(g(n)) | f and g grow at the **same rate** (tight bound) | "exactly" |

**Formal definition of O:** f(n) = O(g(n)) if there exist constants c > 0 and n₀ ≥ 0 such that f(n) ≤ c · g(n) for all n ≥ n₀.

### Common growth rates (slowest → fastest)

$$O(1) \subset O(\log n) \subset O(n) \subset O(n \log n) \subset O(n^2) \subset O(n^3) \subset O(2^n) \subset O(n!)$$

**Key comparisons to memorize:**
- log n grows slower than any polynomial: log n = O(n^ε) for any ε > 0
- Any polynomial dominates any logarithm: n^k dominates log^m(n) for k > 0
- Any exponential dominates any polynomial: 2^n grows faster than n^100
- Scalar multiples don't matter: 5n log n = Θ(n log n)

### Useful rules

| Rule | Example |
|------|---------|
| Drop constants | 100n = O(n) |
| Drop lower-order terms | n² + 999n = Θ(n²) |
| Log base doesn't matter | log₂ n = Θ(log₁₀ n) (change of base = constant) |
| Log of a power | log(nᵏ) = k log n = Θ(log n) |
| Sum rule | O(f) + O(g) = O(max(f, g)) |
| Product rule | O(f) · O(g) = O(f · g) |

### Analyzing loops

**Single loop:** Count iterations.
```
for i in range(n):   → O(n)
```

**Nested loops:** Multiply.
```
for i in range(n):
    for j in range(n):   → O(n²)
```

**Loop that halves/doubles the variable:**
```
i = 1
while i < n:
    i *= 2   → runs ⌊log₂ n⌋ times → O(log n)
```

**Nested loop where inner bound depends on outer:**
```
for i in range(n):       # outer: n times
    j = i
    while j > 0:
        j //= 3          # inner: O(log i) times
```
Total = Σᵢ O(log i) = O(n log n)

**Geometric series shortcut:** 1 + 2 + 4 + … + 2^k = 2^(k+1) − 1 = Θ(2^k). When the outer loop doubles and the inner loop runs proportional to the outer variable, the sum is Θ(n).

### Common mistakes

- **O is not equality:** f = O(g) does not mean g = O(f). Big-O is a one-way bound.
- **Best ≠ average ≠ worst:** Always specify which case you're analyzing.
- **Constant factors matter in practice:** An O(n log n) algorithm with a huge constant may be slower than O(n²) for small n.

---

## 2. Dynamic Arrays

### Core idea

A dynamic array (e.g., Python `list`, C++ `std::vector`) is a contiguous block of memory that grows automatically. It maintains:
- **data:** a pointer to the underlying array
- **size:** number of elements currently stored
- **capacity:** total allocated slots

### Operation complexities

| Operation | Time | Notes |
|-----------|------|-------|
| Access by index | O(1) worst-case | Direct pointer arithmetic |
| Append | **O(1) amortized** | Occasionally O(n) on resize |
| Insert at front | O(n) worst-case | Must shift all elements right |
| Delete at back | O(1) | Just decrement size |
| Delete at front | O(n) | Must shift all elements left |
| Delete at index i | O(n) | Shift elements after i left |
| Search (unsorted) | O(n) | Linear scan |

### Why doubling works (amortized analysis)

When the array is full, allocate a new array of **2× the current capacity** and copy all elements.

| Policy | Total copies for n appends | Amortized cost per append |
|--------|---------------------------|--------------------------|
| Add 1 each time | Θ(n²) | Θ(n) |
| Add k each time | Θ(n²/k) = Θ(n²) | Θ(n) |
| Double each time | Θ(n) | **Θ(1)** |

**Why:** Copies on resize = 1+2+4+…+n/2 = n−1 < n. Total copies for n appends ≤ 2n = Θ(n).

### Shrinking without thrashing

If you halve capacity when size drops below **½ capacity**, alternating append/remove_last near that threshold triggers a resize on every operation → O(n) per operation.

**Fix:** Only halve capacity when size drops below **¼ capacity**. Then after shrinking (to ½ capacity), the array must lose half its elements before shrinking again — no thrashing.

### When to use a dynamic array

- Need O(1) random access by index  
- Mostly append at the back  
- Size is unknown at construction  
- Cache-friendly memory layout matters

---

## 3. Linked Lists

### Variants

| Variant | Extra pointers | Extra cost |
|---------|---------------|------------|
| Singly linked list (SLL) | `next` only | — |
| Singly linked list + tail | `next`, `tail` | O(1) append |
| Doubly linked list (DLL) | `prev`, `next` | O(1) delete at pointer |
| DLL + head + tail | `prev`, `next`, `head`, `tail` | O(1) at both ends |

### Operation complexities

| Operation | SLL (head only) | SLL + tail | DLL + head + tail |
|-----------|:-:|:-:|:-:|
| Insert at front | O(1) | O(1) | O(1) |
| Insert at back | O(n) | **O(1)** | **O(1)** |
| Delete at front | O(1) | O(1) | O(1) |
| Delete at back | O(n) | O(n) | **O(1)** |
| Delete at pointer | O(n)* | O(n)* | **O(1)** |
| Access by index k | O(n) | O(n) | O(n) |
| Search | O(n) | O(n) | O(n) |
| Find middle | O(n) | O(n) | O(n) |

*SLL delete at pointer requires traversal to find the predecessor.

### When to use a linked list

- Need O(1) insert/delete at both ends
- Need O(1) insert/delete at a known pointer
- Need frequent insert/delete/swaps without O(n) shifting

---

## 4. Stacks

### Definition

A stack is a **Last-In, First-Out (LIFO)** container. Only the top element is accessible.

| Operation | Description | Time |
|-----------|-------------|------|
| `push(x)` | Add x to the top | O(1) |
| `pop()` | Remove and return the top | O(1) |
| `peek()` | Return the top without removing | O(1) |
| `is_empty()` | True if no elements | O(1) |

### Implementations

| Backing structure | Notes |
|------------------|-------|
| Dynamic array (push/pop at back) | O(1) amortized push; O(1) worst-case pop; contiguous memory |
| Singly linked list (push/pop at head) | O(1) worst-case for both; pointer overhead |

### The call stack

When function A calls B which calls C:
- C's frame is **pushed** on top
- C returns → C's frame is **popped** (first out)
- B returns → B's frame is **popped**
- A returns → A's frame is **popped**

This is why recursion naturally maps to stacks, and why deeply recursive programs can cause stack overflow.

### When to use a stack

- Need LIFO access (most recent item first)  
- Parsing nested structures (brackets, HTML, arithmetic expressions)  
- Depth-first search / backtracking  
- Call stack simulation / recursion elimination  
- Undo history  

---

## 5. Queues

### Definition

A queue is a **First-In, First-Out (FIFO)** container. Elements enter at the back and leave from the front.

| Operation | Description | Time |
|-----------|-------------|------|
| `enqueue(x)` | Add x to the back | O(1) |
| `dequeue()` | Remove and return the front | O(1) |
| `peek()` | Return the front without removing | O(1) |
| `is_empty()` | True if no elements | O(1) |

### Implementations

| Backing structure | Notes |
|------------------|-------|
| Circular array | Fixed max capacity; O(1) worst-case enqueue and dequeue; no allocation overhead |
| Doubly linked list + head/tail | Unbounded; O(1) worst-case; pointer overhead |

### When to use a queue

- Need FIFO access (process in arrival order)  
- Breadth-first search / level-order traversal  
- Task/request scheduling  
- Buffering streams of data  

---

## 6. Quick Complexity Reference Card

| Structure | Index | Front insert | Back insert | Front delete | Back delete | Delete (pointer) | Search |
|-----------|:-----:|:---:|:---:|:---:|:---:|:---:|:---:|
| Dynamic array | **O(1)** | O(n) | O(1)* | O(n) | O(1) | O(n) | O(n) |
| SLL (head only) | O(n) | O(1) | O(n) | O(1) | O(n) | O(n) | O(n) |
| SLL + tail | O(n) | O(1) | **O(1)** | O(1) | O(n) | O(n) | O(n) |
| DLL + head + tail | O(n) | O(1) | O(1) | O(1) | **O(1)** | **O(1)** | O(n) |
| Stack | — | — | O(1) push | — | O(1) pop | — | — |
| Queue | — | O(1) enqueue | — | O(1) dequeue | — | — | — |

\* Amortized