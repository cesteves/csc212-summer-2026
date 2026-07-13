// tester.cpp
// Lab 10 - Graph Traversal Unit Tester
// CSC 212 Data Structures
//
// This tester is complete - you do not need to modify it. Implement Graph in
// graph.cpp, then compile and run this file to check your work.
//
// Compile:
//   g++ -std=c++17 -Wall -Werror -o tests graph.cpp tester.cpp
// Run:
//   ./tests
//
#include "graph.h"

#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

// Minimal test framework
static int tests_run     = 0;
static int tests_passed  = 0;
static int points_earned = 0;
static int points_total  = 0;

#define CHECK(name, pts, condition)                                           \
    do {                                                                      \
        ++tests_run;                                                          \
        points_total += (pts);                                                \
        if (condition) {                                                      \
            std::cout << "[PASS] " << (name)                                  \
                  << " (" << (pts) << "/" << (pts) << " pts)\n";              \
            ++tests_passed;                                                   \
            points_earned += (pts);                                           \
        } else {                                                              \
            std::cout << "[FAIL] " << (name)                                  \
                      << " (0/" << (pts) << " pts)\n";                        \
        }                                                                     \
    } while (false)

// Helper: does `v` contain `x`?
bool has(const std::vector<int>& v, int x) {
    return std::find(v.begin(), v.end(), x) != v.end();
}

// Helper: index of `x` within `v` (used to compare traversal order)
long pos(const std::vector<int>& v, int x) {
    return std::find(v.begin(), v.end(), x) - v.begin();
}

// Builds the diamond-shaped graph used throughout this tester:
//
//   0 --- 1
//   |     |
//   2 --- 3 --- 4
//
// Every vertex is reachable from vertex 0.
Graph make_diamond() {
    Graph g(5);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    g.add_edge(3, 4);
    return g;
}

// Same diamond, plus an isolated vertex 5 with no edges at all.
Graph make_diamond_with_isolated_vertex() {
    Graph g(6);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    g.add_edge(3, 4);
    return g;
}

// ===========================================================================
// Section 1: adjacency list  (10 pts)
//
// Tests that the graph is constructed with the right number of vertices,
// that add_edge() updates both endpoints (the graph is undirected), that a
// vertex with no edges reports empty neighbors, and that repeated calls to
// add_edge() on the same vertex all accumulate.
void test_adjacency_list() {
    Graph g(5);
    CHECK("vertex_count: matches constructor argument", 2,
          g.vertex_count() == 5
    );

    Graph g2(3);
    g2.add_edge(0, 1);
    CHECK("add_edge: v appears in neighbors(u)", 2,
          has(g2.neighbors(0), 1)
    );
    CHECK("add_edge: u appears in neighbors(v) (undirected)", 2,
          has(g2.neighbors(1), 0)
    );

    CHECK("neighbors: vertex with no edges is empty", 2,
          !g2.neighbors(0).empty() && g2.neighbors(2).empty()
    );

    Graph g3(4);
    g3.add_edge(0, 1);
    g3.add_edge(0, 2);
    g3.add_edge(0, 3);
    CHECK("add_edge: multiple edges accumulate correctly", 2,
          g3.neighbors(0).size() == 3
    );
}

// ===========================================================================
// Section 2: DFS  (10 pts)
//
// Tests that a depth-first traversal starts at the requested vertex, visits
// every vertex reachable in a connected graph, visits each vertex only once,
// and never visits a vertex that isn't reachable from the start.
void test_dfs() {
    Graph g = make_diamond();
    std::vector<int> order = g.dfs(0);

    CHECK("dfs: start vertex is visited first", 2,
          !order.empty() && order.front() == 0
    );
    CHECK("dfs: visits every vertex in a connected graph", 3,
          order.size() == 5
    );

    std::set<int> unique_visited(order.begin(), order.end());
    CHECK("dfs: each vertex appears exactly once", 2,
          order.size() == 5 && unique_visited.size() == order.size()
    );

    Graph g_with_island = make_diamond_with_isolated_vertex();
    std::vector<int> order2 = g_with_island.dfs(0);
    CHECK("dfs: unreachable vertices are excluded", 3,
          order2.size() == 5 && !has(order2, 5)
    );
}

// ===========================================================================
// Section 3: BFS  (10 pts)
//
// Same shape of tests as DFS: correct start, full coverage of a connected
// component, no duplicate visits, and unreachable vertices excluded.
void test_bfs() {
    Graph g = make_diamond();
    std::vector<int> order = g.bfs(0);

    CHECK("bfs: start vertex is visited first", 2,
          !order.empty() && order.front() == 0
    );
    CHECK("bfs: visits every vertex in a connected graph", 3,
          order.size() == 5
    );

    std::set<int> unique_visited(order.begin(), order.end());
    CHECK("bfs: each vertex appears exactly once", 2,
          order.size() == 5 && unique_visited.size() == order.size()
    );

    Graph g_with_island = make_diamond_with_isolated_vertex();
    std::vector<int> order2 = g_with_island.bfs(0);
    CHECK("bfs: unreachable vertices are excluded", 3,
          order2.size() == 5 && !has(order2, 5)
    );
}

// ===========================================================================
// Section 4: edge cases  (10 pts)
//
// Checks the smallest possible graph (a single vertex), confirms dfs() and
// bfs() always agree on *which* vertices are reachable even though they may
// disagree on order, and confirms BFS's defining property: vertices closer
// to the start are visited before vertices farther away.
void test_edge_cases() {
    Graph single(1);
    CHECK("edge: single-vertex graph, dfs visits only itself", 2,
          single.dfs(0).size() == 1 && single.dfs(0).front() == 0
    );
    CHECK("edge: single-vertex graph, bfs visits only itself", 2,
          single.bfs(0).size() == 1 && single.bfs(0).front() == 0
    );

    Graph g = make_diamond();
    std::vector<int> d = g.dfs(0);
    std::vector<int> b = g.bfs(0);
    std::set<int> d_set(d.begin(), d.end());
    std::set<int> b_set(b.begin(), b.end());
    CHECK("edge: dfs and bfs visit the same set of vertices", 3,
          d_set == b_set && d_set.size() > 0
    );

    // In the diamond graph, vertices 1 and 2 are distance 1 from vertex 0,
    // vertex 3 is distance 2, and vertex 4 is distance 3. BFS must visit
    // every vertex at distance 1 before any vertex at distance 2, and so on.
    CHECK("edge: bfs respects distance order (closer before farther)", 3,
          pos(b, 1) < pos(b, 3) && pos(b, 2) < pos(b, 3) && pos(b, 3) < pos(b, 4)
    );
}

// ===========================================================================
// main
int main() {
    std::cout << "adjacency list tests\n";
    test_adjacency_list();

    std::cout << "\nDFS tests\n";
    test_dfs();

    std::cout << "\nBFS tests\n";
    test_bfs();

    std::cout << "\nedge case tests\n";
    test_edge_cases();

    std::cout << "\n";
    std::cout << tests_passed << "/" << tests_run << " tests passed.\n";
    std::cout << "Score: " << points_earned << "/" << points_total << " pts\n";

    return (tests_passed == tests_run) ? 0 : 1;
}
