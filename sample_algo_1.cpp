#include "util.h"
#include <stdio.h>
using namespace std;

bool sample_algo_1(int start, const vector<vector<int>>& graph, int target) {
    vector<int> OPEN = {start};
    int count = 0;
    const int limit = 1000; // Set a limit on number of expansions

    while (!OPEN.empty() && count < limit) {
        count++;
        int n = OPEN.back();
        printf("visited:%d\n", n);
        OPEN.pop_back();

        if (n == target) {
            printf("count:%d\n", count); 
            return true;
        }

        // Push all connected nodes (no check for revisits)
        for (int i = 0; i < graph[n].size(); ++i) {
            if (graph[n][i]) {
                OPEN.insert(OPEN.begin(),i);
            }
        }
    }

    // Not found or limit reached
    printf("count:%d\n", count);
    return false;
}

int main() {
    // Create a graph with 10 nodes labeled 0 to 9
    Graph g(10);

    // Add edges to form a tree structure
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(1, 3);
    g.addEdge(1, 4);
    g.addEdge(2, 5);
    g.addEdge(2, 6);
    g.addEdge(3, 7);
    g.addEdge(3, 8);
    g.addEdge(8, 9);

    g.printGraph();

    if (sample_algo_1(0, g.return_graph(), 7))
        cout << "Node 7 found!\n";
    else
        cout << "Node not found.\n";

    return 0;
}
