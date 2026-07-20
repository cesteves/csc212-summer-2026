// graph.h
// Lab 10 - Graphs: Adjacency List, Save/Load, Cycle Detection, and Shortest Path
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

#include <string>
#include <vector>

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

    // Writes this graph to a text file as an edge list.
    //
    // File format:
    //   line 1:            the number of vertices
    //   every line after:  two integers "u v" naming one undirected edge
    //
    // Each undirected edge must be written exactly once (never both "u v"
    // and "v u" for the same edge).
    // Precondition:  `filename` names a path this program can write to.
    // Does not modify the graph.
    void save_to_file(const std::string& filename) const;

    // Reads a graph from a text file in the format written by save_to_file()
    // (or any file following that same format).
    // Precondition:  `filename` names a readable file in the correct format.
    // Returns: a new Graph containing every vertex and edge described by the file.
    static Graph load_from_file(const std::string& filename);

    // Returns: true if this graph (which may be disconnected) contains at
    // least one cycle, false otherwise.
    // Does not modify the graph.
    bool has_cycle() const;

    // Finds a shortest path (fewest edges) between `start` and `end`.
    // Precondition:  0 <= start < vertex_count() and 0 <= end < vertex_count()
    // Returns: the vertices on a shortest path from `start` to `end`, in
    //          order, including both endpoints. If start == end, returns a
    //          single-element vector containing just that vertex. If no
    //          path exists, returns an empty vector.
    // Does not modify the graph.
    std::vector<int> shortest_path(int start, int end) const;

private:
    std::vector<std::vector<int>> adj;  // adj[i] holds the neighbors of vertex i

    // Recursive helper used by has_cycle(). Visits `v` (arriving from
    // `parent`, or -1 if `v` is the root of this traversal), marking it in
    // `visited`. Returns true if a cycle is discovered anywhere in `v`'s
    // connected component. [private helper]
    bool _has_cycle_visit(int v, int parent, std::vector<bool>& visited) const;
};
