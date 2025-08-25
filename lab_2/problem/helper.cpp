#include <iostream>
#include <ostream>
#include <string>
#include <vector>

void printPath(const std::vector<std::string> &path,
               std::ostream &out = std::cout) {
  for (const std::string &node : path)
    out << node << " ";
  out << std::endl;
}

void printPathChar(const std::vector<char> &path,
                   std::ostream &out = std::cout) {
  for (char node : path)
    out << node << " ";
  out << std::endl;
}

void helperFunction() { std::cout << "Helper function called\n"; }

struct Node {
  int data;
  std::vector<int> neighbors;
  Node(int d) : data(d) {}
};

struct Graph {
  std::vector<Node *> nodes;
  void addNode(int data) { nodes.push_back(new Node(data)); }

  void addEdge(int idx1, int idx2) {
    nodes[idx1]->neighbors.push_back(idx2);
    nodes[idx2]->neighbors.push_back(idx1);
  }

  void printGraphPretty(int idx, std::vector<bool> &visited, std::string prefix,
                        bool isLast) {
    visited[idx] = true;
    std::cout << prefix << (isLast ? "└── " : "├── ")
              << char('a' + nodes[idx]->data) << std::endl;
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
