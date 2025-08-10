#include "util.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> ReconstructPath(pair<int, int> nodePair, const vector<pair<int, int>>& CLOSED) {
    vector<int> path;
    int node = nodePair.first;
    int parent = nodePair.second;

    path.push_back(node);

    while (parent != -1) {
        path.insert(path.begin(), parent);
        // Find the parent nodePair
        auto it = find_if(CLOSED.begin(), CLOSED.end(),
                          [parent](const pair<int, int>& p) {
                              return p.first == parent;
                          });
        if (it == CLOSED.end()) break;
        parent = it->second;
    }

    return path;
}

bool sample_algo_41(int start, const vector<vector<int>>& graph, int target) {
    vector<pair<int, int>> OPEN = {{start, -1}};  // (node, parent)
    vector<pair<int, int>> CLOSED;
    int count = 0;

    while (!OPEN.empty()) {
        count++;
        pair<int, int> currentPair = OPEN.back();
        OPEN.pop_back(); //BFS (stack behavior)
        
        //pair<int, int> currentPair = OPEN.front();
        //OPEN.erase(OPEN.begin()); //BFS (queue behavior)


        int current = currentPair.first;

        // Check if already visited
        bool visited = false;
        for (const auto& p : CLOSED) {
            if (p.first == current) {
                visited = true;
                break;
            }
        }
        if (visited) continue;

        // Add to CLOSED
        CLOSED.push_back(currentPair);

        if (current == target) {
            printf("count: %d\n", count);
            vector<int> path = ReconstructPath(currentPair, CLOSED);
            cout << "Path: ";
            for (int node : path)
                cout << node << " ";
            cout << endl;
            return true;
        }

        // Generate neighbors
        for (int i = 0; i < graph.size(); ++i) {
            if (graph[current][i] == 1) {
                OPEN.push_back({i, current});
            }
        }
    }

    printf("count: %d\n", count);
    return false;
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
    g.printGraph();


  if (sample_algo_41(0,g.return_graph(),7))
      cout << "Node " << 7 << " found!\n";
  else
      cout << "Node not found.\n";

    return 0;
}
