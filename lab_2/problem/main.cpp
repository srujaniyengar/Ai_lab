#include "algo.h"
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

int main() {
  graph G;
  input(G, "input.txt");

  printAdjacencyList(G.adjlist);
  printEdgeList(G.adjlist);
  printSimpleAsciiGraph(G.adjlist);

  std::map<std::string, std::vector<std::string>> adj = {
      {"X", {"K", "L"}},
      {"K", {"X", "L", "B"}},
      {"L", {"X", "K", "B", "Y"}},
      {"B", {"K", "L", "Y"}},
      {"Y", {"L", "B"}}};
  std::string src = "X";
  std::string dest = "Y";

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
    std::cout << "8. Hill Climbing\n9. Beam Search\n10. Best First Search\n";
    std::cout << "11. Run ALL algorithms\n12. Helper Function\n13. Save output "
                 "to output.txt\n14. Exit\nChoice: ";
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
    case 10:
      bestFirstSearch(hstart, hgoal, G);
      break;
    case 11: {
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
    case 12:
      helperFunction();
      break;
    case 13: {
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
    case 14:
      break;
    default:
      std::cout << "Invalid choice.\n";
    }
  } while (ch != 14);
  return 0;
}
