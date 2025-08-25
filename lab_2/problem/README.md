## Scenario

A house cleaning robot navigates a home represented as a graph, where each room is a node and each door is an edge. The robot's objective is to find a path from its starting location (source room) to a target location (destination room) using various search algorithms. The robot must handle both standard navigation and cases requiring heuristic estimates (such as minimizing distance or energy).

## Approach

1. **Graph Representation**:  
   - Rooms are modeled as nodes.
   - Doors are modeled as edges.
   - Heuristic values (e.g., estimated cost to goal) are assigned to each node for heuristic searches.

2. **Algorithm Implementation**:  
   - **Uninformed Search**: BFS, DFS, Bidirectional BFS, British Museum Search.
   - **Heuristic Search**: Hill Climbing, Beam Search (configurable beam width).
   - Each algorithm finds and prints the path from source to destination, along with complexity details.

3. **Visualization**:  
   - The graph is printed in ASCII, showing connections between rooms.
   - A pretty tree view is available for easy understanding of structure.

4. **User Interaction**:  
   - Menu-driven interface: select algorithms and beam width.
   - Results can be displayed or saved to a file.

5. **Output**:  
   - Paths found, nodes expanded, and complexity analysis for each algorithm.
   - Option to export results to `output.txt` for review or submission.
