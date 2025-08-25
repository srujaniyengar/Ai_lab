/*
    algo.h

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

// Graph structure for char-based algorithms (heuristic-based)
struct graph {
  std::unordered_map<char, std::vector<char>> adjlist;
  std::unordered_map<char, int> heuristic;
};

// BFS (forward/reverse)
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

// DFS (forward/reverse)
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

// Bidirectional BFS
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

// British Museum Search (all paths)
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

// Hill Climbing (greedy, heuristic-based)
inline void hillclimbing(char start, char goal, graph &g,
                         std::ostream &out = std::cout) {
  auto current = start;
  std::unordered_map<char, bool> visited;
  std::vector<char> path;
  path.push_back(current);
  visited[current] = true;
  while (true) {
    out << "Current Node: " << current << " (h=" << g.heuristic[current]
        << ")\n";
    if (goal == current) {
      out << "Goal has been found\nPath: ";
      printPathChar(path, out);
      out << "Complexity (Hill Climbing): Time = O(b*h), Space = O(h)\n";
      return;
    }
    std::vector<char> neighbours = g.adjlist[current];
    std::vector<char> unvisited;
    for (auto node : neighbours) {
      if (!visited[node])
        unvisited.push_back(node);
    }
    if (unvisited.empty()) {
      out << "No unvisited neighbours. Dead end reached\nPath: ";
      printPathChar(path, out);
      out << "Complexity (Hill Climbing): Time = O(b*h), Space = O(h)\n";
      return;
    }
    char best = unvisited[0];
    for (auto node : unvisited) {
      if (g.heuristic[node] < g.heuristic[best]) {
        best = node;
      }
    }
    out << "Moving from " << current << " -> " << best << "\n";
    current = best;
    visited[current] = true;
    path.push_back(current);
  }
}

// Beam Search (heuristic-based, configurable width)
struct NodePath {
  char node;
  std::string path;
};

inline void beamsearch(char start, char goal, graph &g, int beamwidth,
                       std::ostream &out = std::cout) {
  std::vector<NodePath> front = {{start, std::string(1, start)}};
  std::unordered_set<char> visited;
  visited.insert(start);
  while (!front.empty()) {
    for (auto &np : front) {
      out << "Front Node: " << np.node << " (h=" << g.heuristic[np.node]
          << ")\n";
      if (np.node == goal) {
        out << "Goal has been found. Path: ";
        for (char c : np.path)
          out << c << " ";
        out << "\n";
        out << "Complexity (Beam Search): Time = O(b^m), Space = O(b*m)\n";
        return;
      }
    }
    std::vector<NodePath> successors;
    for (auto &np : front) {
      for (char neigh : g.adjlist[np.node]) {
        if (visited.find(neigh) == visited.end()) {
          successors.push_back({neigh, np.path + neigh});
        }
      }
    }
    if (successors.empty()) {
      out << "No successors. Search failed\n";
      out << "Complexity (Beam Search): Time = O(b^m), Space = O(b*m)\n";
      return;
    }
    std::sort(successors.begin(), successors.end(), [&](auto &a, auto &b) {
      return g.heuristic[a.node] < g.heuristic[b.node];
    });
    if ((int)successors.size() > beamwidth)
      successors.resize(beamwidth);
    for (auto &np : successors)
      visited.insert(np.node);
    front = successors;
    out << "Next front: ";
    for (auto &np : front)
      out << np.node << " ";
    out << "\n";
  }
  out << "Search failed\n";
  out << "Complexity (Beam Search): Time = O(b^m), Space = O(b*m)\n";
}

// Best First Search (priority queue, heuristic-based)
inline void bestFirstSearch(char start, char goal, graph &g,
                            std::ostream &out = std::cout) {
  struct PQNode {
    char node;
    std::vector<char> path;
    int h;
    bool operator>(const PQNode &other) const { return h > other.h; }
  };

  std::priority_queue<PQNode, std::vector<PQNode>, std::greater<PQNode>> pq;
  std::unordered_set<char> visited;
  pq.push({start, {start}, g.heuristic[start]});
  int expanded = 0;

  while (!pq.empty()) {
    auto curr = pq.top();
    pq.pop();
    expanded++;
    if (visited.count(curr.node))
      continue;
    visited.insert(curr.node);

    out << "Visiting " << curr.node << " (h=" << curr.h << ")\n";
    if (curr.node == goal) {
      out << "Goal found!\nPath: ";
      printPathChar(curr.path, out);
      out << "Nodes expanded: " << expanded << std::endl;
      out << "Complexity (Best First Search): Time = O(b log n), Space = "
             "O(n)\n";
      return;
    }
    for (char neigh : g.adjlist[curr.node]) {
      if (!visited.count(neigh)) {
        std::vector<char> newPath = curr.path;
        newPath.push_back(neigh);
        pq.push({neigh, newPath, g.heuristic[neigh]});
      }
    }
  }
  out << "No path found.\n";
  out << "Complexity (Best First Search): Time = O(b log n), Space = O(n)\n";
}

// Run all algorithms and print output
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
