#include "util.h"
#include <unordered_set>

bool GoalTest(Node *node, int goal) { return node->data == goal; }

std::vector<Node *> MoveGen(Graph &graph, Node *node) {
  std::vector<Node *> result;
  for (int nb : node->neighbors)
    result.push_back(graph.nodes[nb]);
  return result;
}

// Random walk: breaks if it revisits a node in the CURRENT path (cycle)
bool sample_algo_1(Graph &graph, int start, int goal, std::vector<int> &path) {
  Node *current = graph.nodes[start];
  std::unordered_set<int> visited; // track nodes in current path
  path.clear();

  std::srand((unsigned)std::time(nullptr));
  int steps = 0, max_steps = graph.nodes.size() * 10; // avoid infinite loop

  while (current && steps < max_steps) {
    path.push_back(current->data);
    visited.insert(current->data);

    if (GoalTest(current, goal))
      return true;

    // Get unvisited neighbors (w.r.t. current path)
    std::vector<Node *> candidates;
    for (int nb : current->neighbors) {
      if (visited.count(nb) == 0)
        candidates.push_back(graph.nodes[nb]);
    }

    if (candidates.empty())
      return false; // no way forward

    // Randomly pick one neighbor
    Node *next = candidates[std::rand() % candidates.size()];

    // If next is already in path (shouldn't happen due to filtering), break
    // anyway
    if (visited.count(next->data))
      return false; // cycle detected

    current = next;
    steps++;
  }
  return false; // max steps exceeded
}

void print_menu() {
  std::cout << "\n========== GRAPH MENU ==========\n";
  std::cout << "1. Print graph (pretty DFS)\n";
  std::cout << "2. Print adjacency list\n";
  std::cout << "3. Search for goal node (random walk, aborts on cycle)\n";
  std::cout << "4. Exit\n";
  std::cout << "Enter your choice: ";
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
  g.addEdge(9, 1); // Cycle

  int choice = 0;
  while (true) {
    print_menu();
    std::cin >> choice;

    if (choice == 1) {
      std::cout << "\nGraph structure (pretty DFS print):\n";
      g.printGraph();
    } else if (choice == 2) {
      g.printAdjacencyList();
    } else if (choice == 3) {
      int goal;
      std::cout << "Enter goal node index (0 for 'a', ..., 9 for 'j'): ";
      std::cin >> goal;
      std::vector<int> path;
      std::cout << "\nSearch for goal node '" << char('a' + goal)
                << "' (random walk, aborts on cycle):\n";
      if (sample_algo_1(g, 0, goal, path)) {
        std::cout << "Goal node found. Path: ";
        for (size_t i = 0; i < path.size(); ++i) {
          std::cout << char('a' + path[i]);
          if (i < path.size() - 1)
            std::cout << " -> ";
        }
        std::cout << std::endl;
      } else {
        std::cout << "Search failed or encountered a cycle (aborted)."
                  << std::endl;
      }
    } else if (choice == 4) {
      std::cout << "Exiting. Goodbye!\n";
      break;
    } else {
      std::cout << "Invalid choice. Please try again.\n";
    }
  }
  return 0;
}
