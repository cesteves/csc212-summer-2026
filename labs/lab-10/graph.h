// graph.h
// Lab 10 - Graph Traversal: Adjacency List, DFS, and BFS
// CSC 212 Data Structures
//
// ===========================================================================
// Do NOT change anything in this file. It declares the Graph interface you
// must implement in graph.cpp; the autograder relies on these exact
// declarations. Write your implementation only in graph.cpp.
//
// Compile the unit tester
//   g++ -std=c++17 -Wall -Werror -o tests graph.cpp tester.cpp
// ===========================================================================

#pragma once

#include <vector>   // std::vector - adjacency list storage

class Graph {
public:
    // Constructs a graph with `num_vertices` vertices, labeled 0..num_vertices-1,
    // and no edges.
    // Precondition:  num_vertices >= 0
    // Postcondition: vertex_count() == num_vertices
    //                neighbors(v) is empty for every valid vertex v
    explicit Graph(int num_vertices);

    // Adds an undirected edge between vertices `u` and `v`.
    // Precondition:  0 <= u < vertex_count() and 0 <= v < vertex_count()
    // Postcondition: v appears in neighbors(u) and u appears in neighbors(v)
    // NOTE: you do not need to guard against duplicate edges or self-loops -
    //       simply append to both adjacency lists.
    void add_edge(int u, int v);

    // Returns: the number of vertices in the graph
    // Does not modify the graph.
    int vertex_count() const;

    // Returns: the vertices adjacent to `v`, in the order their edges were added
    // Precondition:  0 <= v < vertex_count()
    // Does not modify the graph.
    const std::vector<int>& neighbors(int v) const;

    // Performs a depth-first traversal starting at `start`, visiting every
    // vertex reachable from `start` exactly once.
    // Precondition:  0 <= start < vertex_count()
    // Returns:       the vertices in the order they were first visited
    //                (start is always first)
    // Does not modify the graph.
    std::vector<int> dfs(int start) const;

    // Performs a breadth-first traversal starting at `start`, visiting every
    // vertex reachable from `start` exactly once, nearest vertices first.
    // Precondition:  0 <= start < vertex_count()
    // Returns:       the vertices in the order they were first visited
    //                (start is always first)
    // Does not modify the graph.
    std::vector<int> bfs(int start) const;

private:
    std::vector<std::vector<int>> adj;  // adj[i] holds the neighbors of vertex i

    // Recursive helper used by dfs(). Visits `v`, marks it visited in
    // `visited`, records it in `order`, then recurses into every neighbor
    // of `v` that has not yet been visited. [private helper]
    void _dfs_visit(int v, std::vector<bool>& visited, std::vector<int>& order) const;
};
