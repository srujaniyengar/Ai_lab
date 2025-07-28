/*
================================================================================
Graph and Node Classes with TWO Print Functions
================================================================================

class Graph
    - Graph(int n)
    - void addEdge(int u, int v)
    - void printAdjacencyList() // shows all edges (including cycles)
    - void printGraph(int start = 0) // pretty DFS print, shows one path per
node

struct Node
    - int data
    - vector<int> neighbors

================================================================================
*/

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

struct Node {
  int data;
  vector<int> neighbors;
  Node(int val) : data(val) {}
};

class Graph {
public:
  vector<Node *> nodes;

  Graph(int n) {
    for (int i = 0; i < n; ++i)
      nodes.push_back(new Node(i));
  }

  void addEdge(int u, int v) {
    nodes[u]->neighbors.push_back(v);
    nodes[v]->neighbors.push_back(u); // undirected
  }

  // Print adjacency list: shows all connections including cycles
  void printAdjacencyList() {
    cout << "\nAdjacency List:\n";
    for (auto node : nodes) {
      cout << char('a' + node->data) << ": ";
      for (int nb : node->neighbors)
        cout << char('a' + nb) << " ";
      cout << endl;
    }
  }

  // Pretty DFS print (shows only first encounter of each node)
  void printGraph(int start = 0) {
    vector<bool> visited(nodes.size(), false);
    printGraphPretty(start, visited, "", true);
  }

private:
  void printGraphPretty(int idx, vector<bool> &visited, string prefix,
                        bool isLast) {
    visited[idx] = true;
    cout << prefix << (isLast ? "└── " : "├── ") << char('a' + nodes[idx]->data)
         << endl;
    int childPrinted = 0;
    int total = 0;
    for (int nb : nodes[idx]->neighbors)
      if (!visited[nb])
        ++total;
    for (int nb : nodes[idx]->neighbors) {
      if (!visited[nb]) {
        printGraphPretty(nb, visited, prefix + (isLast ? "    " : "│   "),
                         ++childPrinted == total);
      }
    }
  }
};
