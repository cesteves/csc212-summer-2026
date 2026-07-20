// tester.cpp
// Lab 10 - Graphs Unit Tester
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
#include <cstdio>
#include <fstream>
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

// Helper: sorted copy, for comparing neighbor lists without caring about order
std::vector<int> sorted_copy(std::vector<int> v) {
    std::sort(v.begin(), v.end());
    return v;
}

// Helper: is `path` a genuine walk through `g` - i.e. is every consecutive
// pair of vertices actually connected by an edge?
bool is_valid_walk(const Graph& g, const std::vector<int>& path) {
    if (path.empty()) return false;
    for (std::size_t i = 0; i + 1 < path.size(); ++i) {
        if (!has(g.neighbors(path[i]), path[i + 1])) return false;
    }
    return true;
}

// Builds the diamond-shaped graph used throughout this tester:
//
//   0 --- 1
//   |     |
//   2 --- 3 --- 4
//
// Every vertex is reachable from vertex 0, and 0-1-3-2-0 is a cycle.
Graph make_diamond() {
    Graph g(5);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(2, 3);
    g.add_edge(3, 4);
    return g;
}

// A 6-vertex tree (no cycles): 0 is connected to 1 and 2; 1 is connected to
// 3 and 4; 2 is connected to 5. There is exactly one path between any pair
// of vertices, so no cycle exists anywhere in this graph.
Graph make_tree() {
    Graph g(6);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    g.add_edge(1, 4);
    g.add_edge(2, 5);
    return g;
}

// Two disconnected trees: {0,1,2} and {3,4}. No cycles anywhere.
Graph make_disconnected_no_cycle() {
    Graph g(5);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(3, 4);
    return g;
}

// Two disconnected components: a triangle {0,1,2} (a cycle) and a tree {3,4,5}.
Graph make_disconnected_with_cycle() {
    Graph g(6);
    g.add_edge(0, 1);
    g.add_edge(1, 2);
    g.add_edge(2, 0);
    g.add_edge(3, 4);
    g.add_edge(4, 5);
    return g;
}

// ===========================================================================
// Section 1: adjacency list  (10 pts)
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
// Section 2: save / load  (10 pts)
void test_save_load() {
    const std::string path = "test_save_load_tmp.txt";

    // Round trip: build a graph (with an isolated vertex), save it, load it
    // back, and confirm the structure survived intact.
    Graph g(5);
    g.add_edge(0, 1);
    g.add_edge(0, 2);
    g.add_edge(1, 3);
    // vertex 4 stays isolated - no edges at all
    g.save_to_file(path);
    Graph g2 = Graph::load_from_file(path);

    CHECK("save/load: round trip preserves vertex_count", 2,
          g2.vertex_count() == g.vertex_count() && g.vertex_count() > 0
    );

    bool neighbors_match = true;
    for (int v = 0; v < g.vertex_count() && v < g2.vertex_count(); ++v) {
        if (sorted_copy(g.neighbors(v)) != sorted_copy(g2.neighbors(v))) {
            neighbors_match = false;
        }
    }
    CHECK("save/load: round trip preserves every vertex's neighbor set", 3,
          neighbors_match && g.vertex_count() > 0 && g2.vertex_count() > 0
    );

    CHECK("save/load: isolated vertex stays isolated after round trip", 2,
          g2.vertex_count() > 4 && g2.neighbors(4).empty()
    );

    std::remove(path.c_str());

    // Load a hand-written file directly, independent of save_to_file, to
    // confirm the parser itself is correct.
    const std::string hand_path = "test_load_handwritten_tmp.txt";
    {
        std::ofstream out(hand_path);
        out << "3\n";
        out << "0 1\n";
        out << "1 2\n";
    }
    Graph g3 = Graph::load_from_file(hand_path);
    CHECK("load: hand-written edge list parses correctly", 3,
          g3.vertex_count() == 3 &&
          has(g3.neighbors(0), 1) && has(g3.neighbors(1), 0) &&
          has(g3.neighbors(1), 2) && has(g3.neighbors(2), 1)
    );
    std::remove(hand_path.c_str());
}

// ===========================================================================
// Section 3: cycle detection  (10 pts)
// void test_cycle_detection() {
//     Graph tree = make_tree();
//     CHECK("has_cycle: tree has no cycle", 2,
//           tree.has_cycle() == false && tree.vertex_count() > 0
//     );

//     Graph diamond = make_diamond();
//     CHECK("has_cycle: diamond graph has a cycle", 2,
//           diamond.has_cycle() == true && tree.vertex_count() > 0
//     );

//     Graph single(1);
//     CHECK("has_cycle: single vertex, no edges, has no cycle", 2,
//           single.has_cycle() == false && tree.vertex_count() > 0
//     );

//     Graph disc_no_cycle = make_disconnected_no_cycle();
//     CHECK("has_cycle: disconnected forest has no cycle", 2,
//           disc_no_cycle.has_cycle() == false && tree.vertex_count() > 0
//     );

//     Graph disc_with_cycle = make_disconnected_with_cycle();
//     CHECK("has_cycle: cycle in one component of a disconnected graph is found", 2,
//           disc_with_cycle.has_cycle() == true && tree.vertex_count() > 0
//     );
// }

// ===========================================================================
// Section 3: cycle detection  (10 pts)
//
// This is graded as a single all-or-nothing check rather than five separate
// ones. Splitting it into independent checks would let a stub that always
// returns true (or always returns false) bank partial credit, since roughly
// half of the cases below expect each answer. Requiring every case to be
// correct at once closes that loophole: a constant function fails at least
// one case and scores 0, not ~50%. Each case is still logged individually so
// you can see exactly which one is wrong.
void test_cycle_detection() {
    Graph tree             = make_tree();
    Graph diamond           = make_diamond();
    Graph single(1);
    Graph disc_no_cycle     = make_disconnected_no_cycle();
    Graph disc_with_cycle   = make_disconnected_with_cycle();

    bool tree_ok  = (tree.has_cycle() == false);
    bool diamond_ok = (diamond.has_cycle() == true);
    bool single_ok  = (single.has_cycle() == false);
    bool disc_no_cycle_ok   = (disc_no_cycle.has_cycle() == false);
    bool disc_with_cycle_ok = (disc_with_cycle.has_cycle() == true);

    std::cout << "  [case] tree has no cycle: "
              << (tree_ok ? "correct" : "WRONG") << "\n";
    std::cout << "  [case] diamond graph has a cycle: "
              << (diamond_ok ? "correct" : "WRONG") << "\n";
    std::cout << "  [case] single vertex, no edges, has no cycle: "
              << (single_ok ? "correct" : "WRONG") << "\n";
    std::cout << "  [case] disconnected forest has no cycle: "
              << (disc_no_cycle_ok ? "correct" : "WRONG") << "\n";
    std::cout << "  [case] cycle in one component of a disconnected graph is found: "
              << (disc_with_cycle_ok ? "correct" : "WRONG") << "\n";

    CHECK("has_cycle: correctly classifies every case above (all must be correct)", 10,
          tree_ok && diamond_ok && single_ok && disc_no_cycle_ok && disc_with_cycle_ok
    );
}
#include <sstream>
// ===========================================================================
// Section 4: shortest path  (10 pts)
void test_shortest_path() {
    Graph g = make_diamond();

    CHECK("shortest_path: start == end returns just that vertex", 2,
          g.shortest_path(2, 2).size() == 1 && g.shortest_path(2, 2).front() == 2
    );

    std::vector<int> path = g.shortest_path(0, 4);
    CHECK("shortest_path: diamond graph 0->4 has correct length and endpoints", 3,
          path.size() == 4 && path.front() == 0 && path.back() == 4 
    );

    CHECK("shortest_path: every step of the returned path follows a real edge", 2,
          is_valid_walk(g, path) && g.vertex_count() > 0
    );

    // Graph disc = make_disconnected_no_cycle();
    // CHECK("shortest_path: no path across disconnected components returns empty", 3,
    //       disc.shortest_path(0, 4).empty() && disc.vertex_count() > 0
    // );
    Graph disc = make_disconnected_no_cycle();
    std::vector<int> unreachable_path = disc.shortest_path(0, 4);
    std::vector<int> reachable_path   = disc.shortest_path(0, 2);
    CHECK("shortest_path: distinguishes an unreachable vertex from a reachable one in the same graph", 3,
          unreachable_path.empty() &&
          reachable_path.size() == 3 &&
          reachable_path.front() == 0 && reachable_path.back() == 2 &&
          is_valid_walk(disc, reachable_path)
    );
}

// ===========================================================================
// main
int main() {
    std::cout << "adjacency list tests\n";
    test_adjacency_list();

    std::cout << "\nsave/load tests\n";
    test_save_load();

    std::cout << "\ncycle detection tests\n";
    test_cycle_detection();

    std::cout << "\nshortest path tests\n";
    test_shortest_path();

    std::cout << "\n";
    std::cout << tests_passed << "/" << tests_run << " tests passed.\n";
    std::cout << "Score: " << points_earned << "/" << points_total << " pts\n";

    return (tests_passed == tests_run) ? 0 : 1;
}
