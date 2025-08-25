#include <iostream>
#include <map>
#include <ostream>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

void helperFunction() { std::cout << "Helper function called\n"; }

void printAdjacencyList(
    const std::unordered_map<char, std::vector<char>> &adjlist) {
  std::cout << "\nAdjacency List (ASCII):\n";
  for (const auto &[node, neighbors] : adjlist) {
    std::cout << node << " : ";
    for (char nb : neighbors)
      std::cout << nb << " ";
    std::cout << "\n";
  }
}

void printEdgeList(const std::unordered_map<char, std::vector<char>> &adjlist) {
  std::set<std::pair<char, char>> printed;
  std::cout << "\nEdge List (ASCII):\n";
  for (const auto &[node, neighbors] : adjlist) {
    for (char nb : neighbors) {
      auto e = std::minmax(node, nb);
      if (printed.count(e))
        continue;
      std::cout << e.first << " --- " << e.second << "\n";
      printed.insert(e);
    }
  }
}

void printSimpleAsciiGraph(
    const std::unordered_map<char, std::vector<char>> &adjlist) {
  std::cout << "\nSimple ASCII Diagram:\n";
  std::cout << "Nodes: ";
  for (const auto &[node, _] : adjlist) {
    std::cout << node << " ";
  }
  std::cout << "\nConnections:\n";
  for (const auto &[node, neighbors] : adjlist) {
    std::cout << node << ": ";
    for (char nb : neighbors)
      std::cout << "--" << nb << " ";
    std::cout << "\n";
  }
}
