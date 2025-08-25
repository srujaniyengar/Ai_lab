/*
================================================================================
Uninformed Search Algorithms and Heuristic-Based Search for House Cleaning Robot
================================================================================

Uninformed searchers:
    - BFS: Breadth-First Search (forward and reverse)
    - DFS: Depth-First Search (forward and reverse)
    - British Museum Search: Enumerates all possible paths (forward and reverse)
    - Bidirectional BFS: Simultaneous search from start and goal

Heuristic-based searchers:
    - Hill Climbing
    - Beam Search

Helper functions:
    - Path printing utility (in helper.cpp)
    - Generic helper function (in helper.cpp)

================================================================================
*/

#include "helper.cpp"
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

struct graph {
  std::unordered_map<char, std::vector<char>> adjlist;
  std::unordered_map<char, int> heuristic;
};

void input(graph &g, std::string filename) {
  std::ifstream fin(filename);
  if (!fin.is_open()) {
    std::cout << "Could not open input file.\n";
    return;
  }
  int edges;
  fin >> edges;
  for (int i = 0; i < edges; i++) {
    char u, v;
    fin >> u >> v;
    g.adjlist[u].push_back(v);
    if (g.adjlist.find(v) == g.adjlist.end()) {
      g.adjlist[v] = {};
    }
  }
  int nodes;
  fin >> nodes;
  for (int i = 0; i < nodes; i++) {
    char node;
    int h;
    fin >> node >> h;
    g.heuristic[node] = h;
  }
  fin.close();
}

void printAsciiGraph(const graph &g) {
  std::cout << "\nGraph (Adjacency List):\n";
  for (const auto &[u, neighbors] : g.adjlist) {
    std::cout << u << " -> ";
    for (char v : neighbors)
      std::cout << v << " ";
    std::cout << std::endl;
  }
  std::cout << "\nGraph (Edges):\n";
  for (const auto &[u, neighbors] : g.adjlist) {
    for (char v : neighbors) {
      std::cout << u << " --- " << v << std::endl;
    }
  }
  std::cout << std::endl;
}

// Uninformed Search algorithms (operate on string-based graph for room names)
void bfs(const std::string &src, const std::string &dest,
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

void dfs(const std::string &src, const std::string &dest,
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

void britishMuseum(const std::string &src,
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

void biBFS(const std::string &src, const std::string &dest,
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

void hillclimbing(char start, char goal, graph &g,
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

struct NodePath {
  char node;
  std::string path;
};

void beamsearch(char start, char goal, graph &g, int beamwidth,
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

void run_all_algorithms(
    const std::string &src, const std::string &dest,
    const std::map<std::string, std::vector<std::string>> &adj, char hstart,
    char hgoal, graph &G, int beamwidth, std::ostream &out) {
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
}

int main() {
  std::map<std::string, std::vector<std::string>> adj = {
      {"X", {"K", "L"}},
      {"K", {"X", "L", "B"}},
      {"L", {"X", "K", "B", "Y"}},
      {"B", {"K", "L", "Y"}},
      {"Y", {"L", "B"}}};
  std::string src = "X";
  std::string dest = "Y";

  graph G;
  input(G, "input.txt");

  printAsciiGraph(G);

  int beamwidth;
  std::cout << "Enter beam width: ";
  std::cin >> beamwidth;

  char hstart = 'X', hgoal = 'Y';

  int ch;
  do {
    std::cout << "\nHouse Cleaning Robot (Graph Search)\n";
    std::cout << "Algorithms:\n";
    std::cout << "1. BFS (forward)\n2. BFS (reverse)\n3. Bidirectional BFS\n";
    std::cout << "4. DFS (forward)\n5. DFS (reverse)\n";
    std::cout << "6. British Museum (forward)\n7. British Museum (reverse)\n";
    std::cout << "8. Hill Climbing\n9. Beam Search\n";
    std::cout << "10. Run ALL algorithms\n11. Helper Function\n12. Save output "
                 "to output.txt\n13. Exit\nChoice: ";
    std::cin >> ch;
    switch (ch) {
    case 1:
      bfs(src, dest, adj, false);
      break;
    case 2:
      bfs(src, dest, adj, true);
      break;
    case 3:
      biBFS(src, dest, adj);
      break;
    case 4:
      dfs(src, dest, adj, false);
      break;
    case 5:
      dfs(src, dest, adj, true);
      break;
    case 6:
      britishMuseum(src, adj, dest, false);
      break;
    case 7:
      britishMuseum(src, adj, dest, true);
      break;
    case 8:
      hillclimbing(hstart, hgoal, G);
      break;
    case 9:
      beamsearch(hstart, hgoal, G, beamwidth);
      break;
    case 10: {
      run_all_algorithms(src, dest, adj, hstart, hgoal, G, beamwidth,
                         std::cout);
      std::ofstream fout("output.txt");
      if (fout.is_open()) {
        run_all_algorithms(src, dest, adj, hstart, hgoal, G, beamwidth, fout);
        fout.close();
        std::cout << "Output written to output.txt\n";
      } else {
        std::cout << "Failed to open output.txt\n";
      }
      break;
    }
    case 11:
      helperFunction();
      break;
    case 12: {
      std::ofstream fout("output.txt");
      if (!fout.is_open()) {
        std::cout << "Failed to open output.txt\n";
        break;
      }
      run_all_algorithms(src, dest, adj, hstart, hgoal, G, beamwidth, fout);
      fout.close();
      std::cout << "Output written to output.txt\n";
      break;
    }
    case 13:
      break;
    default:
      std::cout << "Invalid choice.\n";
    }
  } while (ch != 13);
  return 0;
}
