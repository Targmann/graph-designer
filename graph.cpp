#include "graph.h"
#include <stdexcept>
#include <limits>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

// Add vertex
Graph& Graph::add_vertex(string label) {
    // Create new vertex if key doesn't exist
    adj.insert({label, unordered_map<string, double>()});
    // Return graph
    return *this;
}
// Add edge
Graph& Graph::add_edge(string src, string dest, double w) {
    /*
    if a key is not found in adj or negative weight
        throw invalid_argument
    add edge with w weight
    return graph
    */
    // Check for missing vertex or negative weight
    if (adj.find(src) == adj.end() || adj.find(dest) == adj.end() || w < 0) {
        throw invalid_argument("Missing vertex or negative weight");
    }
    // Create new edge
    adj[src][dest] = w;
    // Return graph
    return *this;
}
// Get weight
double Graph::get_weight(string src, string dest) {
    /*
    if src or dest not in graph
        throw invalid_argument
    if no edge from src to dest
        return +inf
    return edge
    */
    // Check missing vertex
    if (adj.find(src) == adj.end() || adj.find(dest) == adj.end()) {
        throw invalid_argument("Missing vertex");
    }
    // Check for no edge
    if (adj[src].find(dest) == adj[src].end()) {
        return numeric_limits<double>::infinity();
    }
    // Return edge weight otherwise
    return adj[src][dest];
}
// Depth-first traversal
vector<string> Graph::dfs(string starting_vertex) {
    /*
    if start vertex not in graph
        throw invalid_argument
    create new vector stack
    create new vector visited
    create variable for currVertex
    add start vertex to stack
    while stack not empty
        currVertex = pop stack
        if currVertex not visited yet
            add currVertex to visited
            create empty vector neighbors
            for each (neighbor, weight) in adj[currVertex]
                add neighbor to neighbors
            sort neighbors alphabetically
            for each neighbor in neighbors
                push neighbor onto stack
    return vertices
    */
    // Check missing start vertex
    if (adj.find(starting_vertex) == adj.end()) {
        throw invalid_argument("Starting vertex missing");
    }
    // Create stack and visited
    vector<string> stack;
    vector<string> visited;
    // Initialize variable for current vertex
    string currVertex;
    // Push start vertex onto stack
    stack.push_back(starting_vertex);
    while (!stack.empty()) {
        // Pop stack and set currVertex
        currVertex = stack.back();
        stack.pop_back();
        // Ensure currVertex hasn't been visited yet
        if (find(visited.begin(), visited.end(), currVertex) == visited.end()) {
            // Add to visited
            visited.push_back(currVertex);
            // Sort neighbors alphabetically (reverse)
            vector<string> neighbors;
            for (const auto& [neighbor, weight] : adj[currVertex]) {
                neighbors.push_back(neighbor);
            }
            sort(neighbors.begin(), neighbors.end(), greater<string>());
            // Push sorted neighbors onto stack
            for (auto& neighbor : neighbors) {
                if (find(visited.begin(), visited.end(), neighbor) == visited.end()) {
                    stack.push_back(neighbor);
                }
            }
        }
    }
    return visited;
}
// Breadth-first traversal
vector<string> Graph::bfs(string starting_vertex) {
    /*
    if start vertex not in graph
        throw invalid_argument
    create new vector frontier
    create new vector discovered
    create variable for currVertex
    add start vertex to frontier
    add start vertex to discovered
    while frontier not empty
        currVertex = front of frontier
        remove first element from frontier
        create empty vector neighbors
        for each (neighbor, weight) in adj[currVertex]
            add neighbor to neighbors
        sort neighbors alphabetically
        for each neighbor in neighbors
            if neighbor not discovered yet
                push neighbor to frontier
                push neighbor to discovered
    return discovered
    */
    // Check missing start vertex
    if (adj.find(starting_vertex) == adj.end()) {
        throw invalid_argument("Starting vertex missing");
    }
    // Create frontier and discovered
    vector<string> frontier;
    vector<string> discovered;
    // Initialize variable for current vertex
    string currVertex;
    // Push start vertex onto vectors
    frontier.push_back(starting_vertex);
    discovered.push_back(starting_vertex);
    while (!frontier.empty()) {
        // Set currVertex and remove first element of frontier
        currVertex = frontier.front();
        frontier.erase(frontier.begin());
        // Sort neighbors alphabetically
        vector<string> neighbors;
        for (const auto& [neighbor, weight] : adj[currVertex]) {
            neighbors.push_back(neighbor);
        }
        sort(neighbors.begin(), neighbors.end());
        // Add each neighbor to frontier and discovered
        for (const auto& neighbor : neighbors) {
            if (find(discovered.begin(), discovered.end(), neighbor) == discovered.end()) {
                frontier.push_back(neighbor);
                discovered.push_back(neighbor);
            }
        }
    }
    return discovered;
}
// Dijkstra's shortest path
vector<string> Graph::dsp(string src, string dest, double& cost) {
    /*
    create new unordered_map<string, double> dist
    create new unordered_map<string, string> pred
    create unvisited vector
    create shortestPath vector
    for each vertex in graph
        dist[vertex] = infinity
        pred[vertex] = null
        add vertex to unvisited
    dist[src] = 0
    while unvisited not empty
        currVertex = vertex in unvisited with smallest dist
        remove currVertex from unvisited
        if currVertex == dest
            break
        for each neighbor of currVertex
            if neighbor not in unvisited
                continue
            altDist = dist[currVertex] + weight(currVertex, neighbor)
            if altDist < dist[neighbor]
                dist[neighbor] = altDist
                pred[neighbor] = currVertex
    */
    // Distances, predecessor, and univisted
    unordered_map<string, double> dist;
    unordered_map<string, string> pred;
    vector<string> unvisited;
    vector<string> shortestPath;
    // Initialize dist and unvisited
    for (const auto& vertex : adj) {
        dist[vertex.first] = numeric_limits<double>::infinity();
        unvisited.push_back(vertex.first);
    }
    // Update src distance
    dist[src] = 0;
    while (!unvisited.empty()) {
        // Assign currVertex to vertex with smallest dist
        string currVertex = unvisited[0];
        for (const string& vertex : unvisited) {
            if (dist[vertex] < dist[currVertex]) {
                currVertex = vertex;
            }
        }
        // Remove currVertex from unvisited
        unvisited.erase(find(unvisited.begin(), unvisited.end(), currVertex));
        // Break early if done
        if (currVertex == dest) {
            break;
        }
        // Loop through neighbors
        for (const auto& neighbor : adj[currVertex]) {
            // Restart if neighbor has already been visited
            if (find(unvisited.begin(), unvisited.end(), neighbor.first) == unvisited.end()) {
                continue;
            }
            double altDist = dist[currVertex] + get_weight(currVertex, neighbor.first);
            if (altDist < dist[neighbor.first]) {
                dist[neighbor.first] = altDist;
                pred[neighbor.first] = currVertex;
            }
        }
    }
    // Update cost
    cost = dist[dest];
    // Return empty vector if no path exists
    if (dist[dest] == numeric_limits<double>::infinity()) {
        return {};
    }
    // Build shortest path
    string curr = dest;
    while (curr != src) {
        shortestPath.push_back(curr);
        curr = pred[curr];
    }
    // Add the source as last vertex
    shortestPath.push_back(src);
    // Reverse to get proper order
    reverse(shortestPath.begin(), shortestPath.end());
    // Return the shortest path
    return shortestPath;
}
// DSP all
unordered_map<string, vector<string>> Graph::dsp_all(string src) {
    unordered_map<string, vector<string>> allPaths;
    for (const auto& destVertex : adj) {
        double cost;
        allPaths[destVertex.first] = dsp(src, destVertex.first, cost);
    }
    return allPaths;
}
// str()
string Graph::str() {
    string output = "digraph G {\n";
    // Sort the vertices
    vector<string> vertices;
    for (const auto& [src, neighbors] : adj) {
        vertices.push_back(src);
    }
    sort(vertices.begin(), vertices.end());
    for (const auto& src : vertices) {
        // Sort neighbors
        vector<string> neighbors;
        for (const auto& [dest, weight] : adj[src]) {
            neighbors.push_back(dest);
        }
        sort(neighbors.begin(), neighbors.end());
        // Build the string
        for (const auto& dest : neighbors) {
            ostringstream oss;
            oss << fixed << setprecision(1) << get_weight(src, dest);
            output += "   " + src + " -> " + dest + " [label=\"" + oss.str() + "\",weight=\"" + oss.str() + "\"];\n";
        }
    }
    output += "}";
    return output;
}
// Stream
ostream& operator<<(ostream& os, Graph& g) {
    os << g.str();
    return os;
}
