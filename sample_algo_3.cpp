#include "util.h"
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool sample_algo_3(int start, const vector<vector<int>>& graph, int target) {
    vector<int> OPEN = {start};                      // Nodes to expand
    vector<vector<int>> CLOSED = {{start}};          // Paths to each visited node
    int count = 0;

    while (!OPEN.empty()) {
        count++;
        int current = OPEN.back();
        OPEN.pop_back();

        // Find the path to this node in CLOSED
        vector<int> current_path;
        for (const vector<int>& path : CLOSED) {
            if (path.back() == current) {
                current_path = path;
                break;
            }
        }

        if (current == target) {
            printf("count:%d\n", count);
            cout << "Path: ";
            for (int node : current_path)
                cout << node << " ";
            cout << endl;
            return true;
        }

        // Generate successors of current
        for (int i = 0; i < graph.size(); ++i) {
            if (graph[current][i] == 1) {
                // Check if this node is already in any path in CLOSED
                bool already_visited = false;
                for (const auto& path : CLOSED) {
                    if (path.back() == i) {
                        already_visited = true;
                        break;
                    }
                }

                if (!already_visited) {
                    OPEN.push_back(i);
                    
                    // Create new path and add to CLOSED
                    vector<int> new_path = current_path;
                    new_path.push_back(i);
                    CLOSED.push_back(new_path);
                }
            }
        }
    }

    printf("count:%d\n", count);
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


  if (sample_algo_3(0,g.return_graph(),7))
      cout << "Node " << 7 << " found!\n";
  else
      cout << "Node not found.\n";

    return 0;
}
