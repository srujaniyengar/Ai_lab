#include "util.h"
using namespace std;

int main() {
  // Create a graph with 10 nodes labeled 0 to 9
  Graph g(10);

  // Add edges to form a tree structure
  g.addEdge(0, 1); // 1-2
  g.addEdge(0, 2); // 1-3
  g.addEdge(1, 3); // 2-4
  g.addEdge(1, 4); // 2-5
  g.addEdge(2, 5); // 3-6
  g.addEdge(2, 6); // 3-7
  g.addEdge(3, 7); // 4-8
  g.addEdge(3, 8); // 4-9
  g.addEdge(8, 9); // 9-10

  // Convert graph to tree with root at node 0 (i.e., "1" if you label from 1)
  Tree *tree = g.toTree(0);

  // Print the tree structure
  tree->printTree(tree->root);

  return 0;
}
