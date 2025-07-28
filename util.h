#include <iostream>
#include <vector>
using namespace std;

class Graph {
private:
  vector<vector<int>> adjMatrix;
  int size;

public:
  Graph(int n) {
    size = n;
    adjMatrix = vector<vector<int>>(n, vector<int>(n, 0));
  }

  void addEdge(int u, int v) {
    if (u >= 0 && v >= 0 && u < size && v < size) {
      adjMatrix[u][v] = 1;
      adjMatrix[v][u] = 1;
    }
  }

  void printGraph() {
    for (int i = 0; i < size; ++i) {
      for (int j = 0; j < size; ++j) {
        cout << adjMatrix[i][j] << " ";
      }
      cout << endl;
    }
  }
};

class Tree {
public:
  struct Node {
    int data;
    vector<Node *> child;
    Node(int val) : data(val) {}
  };

  Node *root;

  Tree(int rootVal) { root = new Node(rootVal); }

  void addChild(Node *parent, int childVal) {
    Node *newChild = new Node(childVal);
    parent->child.push_back(newChild);
  }

  void printTree(Node *node, int depth = 0) {
    if (!node)
      return;
    for (int i = 0; i < depth; ++i)
      cout << "  ";
    cout << node->data << endl;
    for (Node *c : node->child) {
      printTree(c, depth + 1);
    }
  }
};
