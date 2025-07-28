#include "util.h"
#include <cctype>
#include <cstdlib>
#include <ctime>
#include <map>
#include <unordered_set>
#include <vector>

// Converts a lowercase character ('a'-'j') to node index (0-9)
int char_to_index(char ch) {
  ch = std::tolower(ch);
  if (ch >= 'a' && ch <= 'j')
    return ch - 'a';
  return -1;
}

// Converts node index (0-9) to lowercase character
char index_to_char(int idx) {
  return (idx >= 0 && idx <= 9) ? ('a' + idx) : '?';
}

// Implements your described random walk with loop counting (OPEN, abort if node
// in path)
bool sample_algo_1(Graph &graph, int start, int goal,
                   std::map<char, int> &loop_counts) {
  std::vector<int> OPEN;
  std::unordered_set<int> in_path;

  OPEN.push_back(start);
  loop_counts.clear();
  std::srand((unsigned)std::time(nullptr));
  int steps = 0, max_steps = graph.nodes.size() * 10; // avoid infinite loop

  while (!OPEN.empty() && steps < max_steps) {
    // Pick a random node from OPEN
    int idx = std::rand() % OPEN.size();
    int node_idx = OPEN[idx];
    // Remove node from OPEN
    OPEN.erase(OPEN.begin() + idx);

    // If node already in path, count and abort (cycle detected)
    if (in_path.count(node_idx)) {
      loop_counts[index_to_char(node_idx)]++;
      return false;
    }

    // Add to path
    in_path.insert(node_idx);

    // Goal test
    if (node_idx == goal) {
      return true;
    }

    // Explore neighbors: add all neighbors to OPEN, even if already present
    for (int nb : graph.nodes[node_idx]->neighbors) {
      // If neighbor is already in path, increment loop count
      if (in_path.count(nb)) {
        loop_counts[index_to_char(nb)]++;
      }
      OPEN.push_back(nb);
    }

    steps++;
  }
  return false;
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
  g.addEdge(0, 1); // a-b
  g.addEdge(0, 2); // a-c
  g.addEdge(1, 3); // b-d
  g.addEdge(1, 4); // b-e
  g.addEdge(2, 5); // c-f
  g.addEdge(2, 6); // c-g
  g.addEdge(3, 7); // d-h
  g.addEdge(3, 8); // d-i
  g.addEdge(8, 9); // i-j
  g.addEdge(9, 1); // j-b (cycle)

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
      char goal_ch;
      std::cout << "Enter goal node (a-j): ";
      std::cin >> goal_ch;
      int goal = char_to_index(goal_ch);
      if (goal == -1) {
        std::cout << "Invalid node.\n";
        continue;
      }
      std::map<char, int> loop_counts;
      std::cout << "\nSearch for goal node '" << index_to_char(goal)
                << "' (random walk, aborts on cycle):\n";
      bool success = sample_algo_1(g, 0, goal, loop_counts);
      if (success) {
        std::cout << "Search success.\n";
      } else {
        std::cout << "Search failed or encountered a cycle (aborted).\n";
      }
      std::cout << "Loop/cycle counts from each movegen:\n";
      for (const auto &entry : loop_counts) {
        std::cout << entry.first << ": " << entry.second << "\n";
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
