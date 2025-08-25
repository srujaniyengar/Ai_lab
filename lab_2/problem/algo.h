/*

    Contains all graph search algorithm implementations for the House Cleaning
   Robot project. Each algorithm solves the problem of finding a path from a
   source node to a destination node in a connected, undirected graph
   representing rooms in a house.

    Algorithms included:
    - Breadth-First Search (BFS) [forward & reverse]
    - Depth-First Search (DFS) [forward & reverse]
    - Bidirectional BFS
    - British Museum Search (all paths, exhaustive)
    - Hill Climbing (greedy, heuristic-based)
    - Beam Search (heuristic-based, configurable width)
    - Best First Search (priority queue, heuristic-based)

    Input graph:
      Nodes represent rooms.
      Edges represent doors.
      Heuristic values estimate cost from each room to the goal (for heuristic
   algorithms).

    Usage:
      - Each function prints the path found, nodes expanded, and complexity
   analysis.
      - All algorithms assume the input graph is loaded in either adjacency list
   or edge list form.
*/

#pragma once
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

inline void printPath(const std::vector<std::string> &path,
                      std::ostream &out = std::cout) {
  for (const std::string &node : path)
    out << node << " ";
  out << std::endl;
}

inline void printPathChar(const std::vector<char> &path,
                          std::ostream &out = std::cout) {
  for (char node : path)
    out << node << " ";
  out << std::endl;
}

struct graph {
  std::unordered_map<char, std::vector<char>> adjlist;
  std::unordered_map<char, int> heuristic;
};

inline void bfs(const std::string &src, const std::string &dest,
                const std::map<std::string, std::vector<std::string>> &adj,
                bool reverse = false, std::ostream &out = std::cout) {
  out << (reverse ? "Reverse BFS:\n" : "Forward BFS:\n");
  std::string start = reverse ? dest : src;
  std::string goal = reverse ? src : dest;
  std::queue<std::vector<std::string>> q;
  std::set<std::string> visited;
  q.push({start});
  int expanded = 0;
  while (!q.empty()) {
    auto path = q.front();
    q.pop();
    std::string curr = path.back();
    expanded++;
    if (curr == goal) {
      out << "Path: ";
      printPath(path, out);
      out << "Nodes expanded: " << expanded << std::endl;
      out << "Time: O(V+E), Space: O(V)\n";
      return;
    }
    if (visited.count(curr))
      continue;
    visited.insert(curr);
    for (const std::string &nbr : adj.at(curr)) {
      if (!visited.count(nbr)) {
        auto newPath = path;
        newPath.push_back(nbr);
        q.push(newPath);
      }
    }
  }
  out << "No path found.\n";
}

inline void dfs(const std::string &src, const std::string &dest,
                const std::map<std::string, std::vector<std::string>> &adj,
                bool reverse = false, std::ostream &out = std::cout) {
  out << (reverse ? "Reverse DFS:\n" : "Forward DFS:\n");
  std::string start = reverse ? dest : src;
  std::string goal = reverse ? src : dest;
  std::stack<std::vector<std::string>> stk;
  std::set<std::string> visited;
  stk.push({start});
  int expanded = 0;
  while (!stk.empty()) {
    auto path = stk.top();
    stk.pop();
    std::string curr = path.back();
    expanded++;
    if (curr == goal) {
      out << "Path: ";
      printPath(path, out);
      out << "Nodes expanded: " << expanded << std::endl;
      out << "Time: O(V+E), Space: O(V)\n";
      return;
    }
    if (visited.count(curr))
      continue;
    visited.insert(curr);
    for (const std::string &nbr : adj.at(curr)) {
      if (!visited.count(nbr)) {
        auto newPath = path;
        newPath.push_back(nbr);
        stk.push(newPath);
      }
    }
  }
  out << "No path found.\n";
}

inline void biBFS(const std::string &src, const std::string &dest,
                  const std::map<std::string, std::vector<std::string>> &adj,
                  std::ostream &out = std::cout) {
  out << "Bidirectional BFS:\n";
  std::queue<std::vector<std::string>> q1, q2;
  std::map<std::string, std::vector<std::string>> paths1, paths2;
  std::set<std::string> visited1, visited2;
  q1.push({src});
  paths1[src] = {src};
  visited1.insert(src);
  q2.push({dest});
  paths2[dest] = {dest};
  visited2.insert(dest);
  int expanded = 0;
  while (!q1.empty() && !q2.empty()) {
    auto expand =
        [&](std::queue<std::vector<std::string>> &q,
            std::set<std::string> &visitedThis,
            std::set<std::string> &visitedOther,
            std::map<std::string, std::vector<std::string>> &pathsThis,
            std::map<std::string, std::vector<std::string>> &pathsOther)
        -> bool {
      int sz = q.size();
      while (sz--) {
        auto path = q.front();
        q.pop();
        std::string curr = path.back();
        expanded++;
        if (visitedOther.count(curr)) {
          auto otherPath = pathsOther[curr];
          if (&pathsThis == &paths1)
            std::reverse(otherPath.begin(), otherPath.end());
          path.insert(path.end(), otherPath.begin() + 1, otherPath.end());
          out << "Path: ";
          printPath(path, out);
          out << "Nodes expanded: " << expanded << std::endl;
          out << "Time: O(b^(d/2)), Space: O(b^(d/2))\n";
          return true;
        }
        for (const std::string &nbr : adj.at(curr)) {
          if (!visitedThis.count(nbr)) {
            visitedThis.insert(nbr);
            auto newPath = path;
            newPath.push_back(nbr);
            pathsThis[nbr] = newPath;
            q.push(newPath);
          }
        }
      }
      return false;
    };
    if (expand(q1, visited1, visited2, paths1, paths2))
      return;
    if (expand(q2, visited2, visited1, paths2, paths1))
      return;
  }
  out << "No path found.\n";
}

inline void
britishMuseum(const std::string &src,
              const std::map<std::string, std::vector<std::string>> &adj,
              std::string dest, bool reverse = false,
              std::ostream &out = std::cout) {
  out << (reverse ? "Reverse BMS:\n" : "Forward BMS:\n");
  std::string start = reverse ? dest : src;
  std::string goal = reverse ? src : dest;
  std::vector<std::string> path;
  std::set<std::string> visited;
  std::vector<std::vector<std::string>> allPaths;
  std::function<void(std::string)> dfs = [&](std::string node) {
    visited.insert(node);
    path.push_back(node);
    if (node == goal) {
      allPaths.push_back(path);
    } else {
      for (const std::string &nbr : adj.at(node)) {
        if (!visited.count(nbr)) {
          dfs(nbr);
        }
      }
    }
    path.pop_back();
    visited.erase(node);
  };
  dfs(start);
  if (allPaths.empty()) {
    out << "No path found.\n";
    return;
  }
  for (const auto &p : allPaths) {
    printPath(p, out);
  }
  out << "Paths found: " << allPaths.size() << std::endl;
  out << "Time: O(b^d), Space: O(b^d)\n";
}

// --- Heuristic algorithms (corrected) ---

inline void hillclimbing(char start, char goal, graph &g,
                         std::ostream &out = std::cout) {
  char current = start;
  std::vector<char> path;
  path.push_back(current);

  while (current != goal) {
    char best_neighbor = '\0';                 // no neighbor chosen yet
    int best_heuristic = g.heuristic[current]; // current node's heuristic

    for (char neighbor : g.adjlist[current]) {
      out << neighbor << current << ".\n"; // debug: neighbor and current
      if (g.heuristic[neighbor] < best_heuristic) {
        best_heuristic = g.heuristic[neighbor];
        best_neighbor = neighbor;
      }
    }
    if (best_neighbor == '\0') {
      out << "Stuck at local optimum. No path found.\n";
      out << "Path: ";
      printPathChar(path, out);
      return;
    }
    current = best_neighbor;
    path.push_back(current);
  }
  out << "Path: ";
  printPathChar(path, out);
}
struct NodePath {
  char node;
  int heuristic_val;
  std::vector<char> path;
  NodePath() : node(0), heuristic_val(0), path() {} // default constructor
  NodePath(char n, int h, std::vector<char> p)
      : node(n), heuristic_val(h), path(std::move(p)) {}
};

inline void beamsearch(char start, char goal, graph &g, int beamwidth,
                       std::ostream &out = std::cout) {
  std::vector<NodePath> current_level;
  current_level.emplace_back(start, g.heuristic[start],
                             std::vector<char>{start});
  while (!current_level.empty()) {
    std::vector<NodePath> next_level;
    for (auto &np : current_level) {
      if (np.node == goal) {
        out << "Path: ";
        printPathChar(np.path, out);
        return;
      }
      for (char neighbor : g.adjlist[np.node]) {
        if (std::find(np.path.begin(), np.path.end(), neighbor) ==
            np.path.end()) {
          std::vector<char> new_path = np.path;
          new_path.push_back(neighbor);
          next_level.emplace_back(neighbor, g.heuristic[neighbor], new_path);
        }
      }
    }
    std::sort(next_level.begin(), next_level.end(),
              [](const NodePath &a, const NodePath &b) {
                return a.heuristic_val < b.heuristic_val;
              });
    if (next_level.size() > (size_t)beamwidth)
      next_level.resize(beamwidth);
    current_level = std::move(next_level);
  }
  out << "No path found\n";
}

inline void bestFirstSearch(char start, char goal, graph &g,
                            std::ostream &out = std::cout) {
  struct PQNode {
    char node;
    int heuristic_val;
    std::vector<char> path;
    PQNode(char n, int h, std::vector<char> p)
        : node(n), heuristic_val(h), path(std::move(p)) {}
    bool operator>(const PQNode &other) const {
      return heuristic_val > other.heuristic_val;
    }
  };

  std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;
  pq.push(PQNode(start, g.heuristic[start], {start}));
  std::unordered_set<char> visited;

  while (!pq.empty()) {
    auto current = pq.top();
    pq.pop();
    if (visited.count(current.node))
      continue;
    visited.insert(current.node);

    if (current.node == goal) {
      out << "Path: ";
      printPathChar(current.path, out);
      return;
    }

    for (char neighbor : g.adjlist[current.node]) {
      if (std::find(current.path.begin(), current.path.end(), neighbor) ==
          current.path.end()) {
        std::vector<char> new_path = current.path;
        new_path.push_back(neighbor);
        pq.push(PQNode(neighbor, g.heuristic[neighbor], new_path));
      }
    }
  }
  out << "No path found\n";
}

inline void
run_all_algorithms(const std::string &src, const std::string &dest,
                   const std::map<std::string, std::vector<std::string>> &adj,
                   char hstart, char hgoal, graph &G, int beamwidth,
                   std::ostream &out) {
  out << "==============================\n";
  out << "BFS (forward):\n";
  bfs(src, dest, adj, false, out);
  out << "==============================\n";
  out << "BFS (reverse):\n";
  bfs(src, dest, adj, true, out);
  out << "==============================\n";
  out << "Bidirectional BFS:\n";
  biBFS(src, dest, adj, out);
  out << "==============================\n";
  out << "DFS (forward):\n";
  dfs(src, dest, adj, false, out);
  out << "==============================\n";
  out << "DFS (reverse):\n";
  dfs(src, dest, adj, true, out);
  out << "==============================\n";
  out << "British Museum (forward):\n";
  britishMuseum(src, adj, dest, false, out);
  out << "==============================\n";
  out << "British Museum (reverse):\n";
  britishMuseum(src, adj, dest, true, out);
  out << "==============================\n";
  out << "Hill Climbing:\n";
  hillclimbing(hstart, hgoal, G, out);
  out << "==============================\n";
  out << "Beam Search:\n";
  beamsearch(hstart, hgoal, G, beamwidth, out);
  out << "==============================\n";
  out << "Best First Search:\n";
  bestFirstSearch(hstart, hgoal, G, out);
  out << "==============================\n";
}
