#include "graph.h"
#include <iostream>

using namespace std;

int main() {
    Graph G;

    // vertices
    G.add_vertex("A").add_vertex("B").add_vertex("C")
     .add_vertex("D").add_vertex("E").add_vertex("F");

    // edges (from spec)
    G.add_edge("A","B",2.0);
    G.add_edge("A","F",9.0);
    G.add_edge("B","C",8.0);
    G.add_edge("B","D",15.0);
    G.add_edge("B","F",6.0);
    G.add_edge("C","D",1.0);
    G.add_edge("E","C",7.0);
    G.add_edge("E","D",3.0);
    G.add_edge("F","B",6.0);
    G.add_edge("F","E",3.0);

    // print graph
    cout << G << endl;

    // BFS
    string start = "A";
    vector<string> bfs_res = G.bfs(start);
    cout << "Starting BFS with vertex " << start << " ";
    for (auto& v : bfs_res) cout << v;
    cout << endl;

    // DFS
    vector<string> dfs_res = G.dfs(start);
    cout << "Starting DFS with vertex " << start << " ";
    for (auto& v : dfs_res) cout << v;
    cout << endl;

    // shortest path A -> F
    double cost;
    vector<string> path = G.dsp("A","F",cost);

    cout << "Shortest path A->F: ";
    for (auto& v : path) cout << v;
    cout << " (cost=" << cost << ")" << endl;

    // all shortest paths
    auto all = G.dsp_all("A");
    cout << "All shortest paths from A:\n";
    for (auto& p : all) {
        cout << "A -> " << p.first << ": ";
        for (auto& v : p.second) cout << v;
        cout << endl;
    }

    return 0;
}