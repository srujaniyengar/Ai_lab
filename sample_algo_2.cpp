#include "util.h"
#include <stdio.h>
using namespace std;

bool sample_algo_2(int start,const vector<vector<int>>& graph,int target){
    vector<int> OPEN ={start},CLOSED;
    int count=0;

    while (!OPEN.empty()) {
        count++;
        int n = OPEN.back();
        OPEN.pop_back();

        CLOSED.push_back(n);

        if (n == target){
            printf("count:%d\n",count); 
            return true;
        }
        // Push unvisited children into OPEN
        for (int i = 0; i < graph[n].size(); ++i) {
            if (graph[n][i]){
                bool visited = false;

                // Check in CLOSED
                for (int closedNode : CLOSED) {
                    if (closedNode == i) {
                        visited = true;
                        break;
                    }
                }

                /* Check in OPEN (optional to avoid duplicate visits)
                for (int openNode : OPEN) {
                    if (openNode == child) {
                        visited = true;
                        break;
                    }
                }
                */
                if (!visited)
                    OPEN.push_back(i);
            }
        }
    }

     // Not found
    printf("counf:%d\n",count);
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


  if (sample_algo_2(0,g.return_graph(),7))
      cout << "Node " << 7 << " found!\n";
  else
      cout << "Node not found.\n";

    return 0;
}
