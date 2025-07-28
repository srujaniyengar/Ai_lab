#include "util.h"
using namespace std;

Tree::Node* search(Tree::Node* root, int target) {
    vector<Tree::Node*> OPEN = { root };
    vector<Tree::Node*> CLOSED;

    while (!OPEN.empty()) {
        Tree::Node* n = OPEN.back();
        OPEN.pop_back();

        CLOSED.push_back(n);

        if (n->data == target) 
          return n;

        // Push unvisited children into OPEN
        for (Tree::Node* child : n->child) {
            bool visited = false;

            // Check in CLOSED
            for (Tree::Node* closedNode : CLOSED) {
                if (closedNode == child) {
                    visited = true;
                    break;
                }
            }

            // Check in OPEN (optional to avoid duplicate visits)
            for (Tree::Node* openNode : OPEN) {
                if (openNode == child) {
                    visited = true;
                    break;
                }
            }

            if (!visited)
                OPEN.push_back(child);
        }
    }

    return nullptr; // Not found
}


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
  cout << "\nSearching for node 9...\n";
  Tree::Node* result = search(tree->root, 2);

  if (result)
      cout << "Node " << result->data << " found!\n";
  else
      cout << "Node not found.\n";

  // Print the tree structure
  tree->printTree(tree->root);
  return 0;
}
