#include "util.h"
using namespace std;

// Returns true if node's data matches the goal value.
bool GoalTest(Tree::Node *node, int goal) { return node->data == goal; }

// Returns all children of the given node.
vector<Tree::Node *> MoveGen(Tree::Node *node) { return node->child; }

// Sample Algorithm 1: Search for a goal value. If found, return the path to it.
bool sample_algo_1(Tree *tree, int goal, std::vector<int> &path) {
  set<Tree::Node *> OPEN;
  map<Tree::Node *, Tree::Node *> parent; // To reconstruct path
  OPEN.insert(tree->root);
  parent[tree->root] = nullptr;

  while (!OPEN.empty()) {
    auto it = OPEN.begin();
    Tree::Node *n = *it;
    OPEN.erase(it);

    if (GoalTest(n, goal)) {
      // Reconstruct path from root to goal
      std::vector<int> temp;
      for (Tree::Node *cur = n; cur != nullptr; cur = parent[cur])
        temp.push_back(cur->data);
      std::reverse(temp.begin(), temp.end());
      path = temp;
      return true;
    } else {
      vector<Tree::Node *> children = MoveGen(n);
      for (Tree::Node *c : children) {
        if (parent.count(c) == 0) {
          OPEN.insert(c);
          parent[c] = n;
        }
      }
    }
  }
  return false;
}

int main() {
  Graph g(10);
  g.addEdge(0, 1);
  g.addEdge(0, 2);
  g.addEdge(1, 3);
  g.addEdge(1, 4);
  g.addEdge(2, 5);
  g.addEdge(2, 6);
  g.addEdge(3, 7);
  g.addEdge(3, 8);
  g.addEdge(8, 9);

  Tree *tree = g.toTree(0);

  cout << "Full tree structure:\n";
  tree->printTree(tree->root);

  int goal1 = 7;
  std::vector<int> path1;
  cout << "\nSearch for goal node 'h':\n";
  if (sample_algo_1(tree, goal1, path1)) {
    cout << "Goal node found. Path: ";
    for (int i = 0; i < path1.size(); ++i) {
      cout << char('a' + path1[i]);
      if (i < path1.size() - 1)
        cout << " -> ";
    }
    cout << endl;
  } else {
    cout << "Search failed: Goal node not found." << endl;
  }

  int goal2 = 15;
  std::vector<int> path2;
  cout << "\nSearch for goal node 'p':\n";
  if (sample_algo_1(tree, goal2, path2)) {
    cout << "Goal node found. Path: ";
    for (int i = 0; i < path2.size(); ++i) {
      cout << char('a' + path2[i]);
      if (i < path2.size() - 1)
        cout << " -> ";
    }
    cout << endl;
  } else {
    cout << "Search failed: Goal node not found." << endl;
  }

  return 0;
}
