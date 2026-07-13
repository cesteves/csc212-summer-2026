// graph.cpp
// Lab 10 - Graph Traversal: Adjacency List, DFS, and BFS
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
// Section 2: DFS (Depth-First Search)
// ---------------------------------------------------------------------------

void Graph::_dfs_visit(int v, std::vector<bool>& visited, std::vector<int>& order) const {
    // TODO:
    //   1. Mark `v` as visited.
    //   2. Record `v` in `order`.
    //   3. For every neighbor of `v` that has not been visited yet,
    //      recursively call _dfs_visit on that neighbor.
}

std::vector<int> Graph::dfs(int start) const {
    // TODO:
    //   1. Create a `visited` vector sized to vertex_count(), all false.
    //   2. Create an empty `order` vector to collect the traversal order.
    //   3. Kick off the traversal with _dfs_visit(start, visited, order).
    //   4. Return `order`.
    std::vector<int> order;
    return order;
}

// ---------------------------------------------------------------------------
// Section 3: BFS (Breadth-First Search)
// ---------------------------------------------------------------------------

std::vector<int> Graph::bfs(int start) const {
    // TODO:
    //   1. Create a `visited` vector sized to vertex_count(), all false.
    //   2. Create an empty `order` vector to collect the traversal order.
    //   3. Create a std::queue<int>, mark `start` visited, and push it onto
    //      the queue.
    //   4. While the queue is not empty:
    //        - pop the front vertex `v`, append it to `order`
    //        - for every unvisited neighbor of `v`: mark it visited and
    //          push it onto the queue
    //   5. Return `order`.
    std::vector<int> order;
    return order;
}
