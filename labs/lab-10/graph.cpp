// graph.cpp
// Lab 10 - Graphs: Adjacency List, Save/Load, Cycle Detection, and Shortest Path
// CSC 212 Data Structures
//
// ===========================================================================
// Write your implementation only inside the TODO method bodies below.
// Do NOT change any signature - graph.h declares the interface the autograder
// expects, and this file must implement it exactly as declared.
//
// Compile the unit tester
//   g++ -std=c++17 -Wall -Werror -o tests graph.cpp tester.cpp
// ===========================================================================

#include "graph.h"

#include <fstream>   // std::ifstream, std::ofstream - used in Section 2
#include <queue>     // std::queue - used in Section 4

// ---------------------------------------------------------------------------
// Section 1: Creating the adjacency list
// ---------------------------------------------------------------------------

Graph::Graph(int num_vertices) {
    // TODO: size `adj` so that it holds exactly `num_vertices` empty chains,
    // one per vertex.
}

void Graph::add_edge(int u, int v) {
    // TODO: this graph is undirected, so `v` must become a neighbor of `u`
    // AND `u` must become a neighbor of `v`.
}

int Graph::vertex_count() const {
    // TODO: return the number of vertices (how many chains does `adj` hold?)
    return 0;
}

const std::vector<int>& Graph::neighbors(int v) const {
    // TODO: return the adjacency chain that belongs to vertex `v`.
    static std::vector<int> placeholder;
    return placeholder;
}

// ---------------------------------------------------------------------------
// Section 2: Save / Load
//
// A graph is written to disk as a simple edge list:
//   line 1:            vertex_count()
//   every line after:   "u v" for one undirected edge
// ---------------------------------------------------------------------------

void Graph::save_to_file(const std::string& filename) const {
    // TODO:
    //   1. Open an std::ofstream on `filename`.
    //   2. Write vertex_count() on its own line.
    //   3. For every vertex u (0 .. vertex_count()-1), for every neighbor w
    //      of u with w >= u, write "u w" on its own line. The `w >= u` check
    //      keeps each undirected edge from being written twice (once as
    //      "u w" and once as "w u").
}

Graph Graph::load_from_file(const std::string& filename) {
    // TODO:
    //   1. Open an std::ifstream on `filename`.
    //   2. Read the vertex count from the first line and construct a Graph
    //      with that many vertices.
    //   3. Read "u v" integer pairs for the rest of the file (e.g. with
    //      `while (fin >> u >> v)`), calling add_edge(u, v) for each pair.
    //   4. Return the constructed graph.
    return Graph(0);
}

// ---------------------------------------------------------------------------
// Section 3: Cycle detection (DFS)
//
// A graph contains a cycle if, while exploring depth-first, you reach a
// vertex that is already visited through some edge OTHER than the one you
// just came from. Reaching your own parent back through the same edge you
// arrived on doesn't count - that's not a cycle, just backtracking.
// ---------------------------------------------------------------------------

bool Graph::_has_cycle_visit(int v, int parent, std::vector<bool>& visited) const {
    // TODO:
    //   1. Mark `v` as visited.
    //   2. For every neighbor `n` of `v`:
    //        - if `n` has not been visited yet, recurse into it with `v` as
    //          the new parent; if that recursive call returns true, a cycle
    //          was found deeper in the search, so return true here too.
    //        - otherwise (n HAS been visited): if `n` is not `parent`, you've
    //          found an edge back to some ancestor that isn't the vertex you
    //          just came from - that's a cycle, so return true.
    //   3. If the loop finishes without ever returning true, return false.
    return false;
}

bool Graph::has_cycle() const {
    // TODO:
    //   1. Create a `visited` vector sized to vertex_count(), all false.
    //   2. The graph may be disconnected, so loop over every vertex 0 ..
    //      vertex_count()-1; for any vertex not yet visited, call
    //      _has_cycle_visit(vertex, -1, visited). If it returns true, the
    //      graph contains a cycle - return true immediately.
    //   3. If every component finishes without finding a cycle, return false.
    return false;
}

// ---------------------------------------------------------------------------
// Section 4: Shortest path (BFS)
//
// BFS explores level by level, so the first time it reaches `end` is
// guaranteed to be via a shortest path (fewest edges) from `start`.
// ---------------------------------------------------------------------------

std::vector<int> Graph::shortest_path(int start, int end) const {
    // TODO:
    //   1. If start == end, return a vector containing just that one vertex.
    //   2. Create a `visited` vector and a `parent` vector, both sized to
    //      vertex_count(); initialize every entry of `parent` to -1.
    //   3. Run a standard BFS starting from `start`: mark a vertex visited
    //      the moment it is pushed onto the queue, and whenever you discover
    //      an unvisited neighbor `n` while processing vertex `v`, record
    //      parent[n] = v before pushing `n`.
    //   4. If `end` is never visited by the time the queue empties, there is
    //      no path - return an empty vector.
    //   5. Otherwise, reconstruct the path by starting at `end` and
    //      repeatedly following `parent` backwards until you reach `start`,
    //      collecting each vertex you visit along the way. Reverse the
    //      collected vertices (so the path reads start -> ... -> end) before
    //      returning them.
    std::vector<int> path;
    return path;
}
