#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Graph {
private:
    unordered_map<string, unordered_map<string, double>> adj;

public:
    Graph& add_vertex(string label);
    Graph& add_edge(string src, string dest, double w);

    double get_weight(string src, string dest);

    vector<string> dfs(string starting_vertex);
    vector<string> bfs(string starting_vertex);

    vector<string> dsp(string src, string dest, double& cost);
    unordered_map<string, vector<string>> dsp_all(string src);

    string str();
};

ostream& operator<<(ostream& os, Graph& g);

#endif