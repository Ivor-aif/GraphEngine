# GraphEngine
Simply process graphs/networks.

# Project Description
****
&emsp;&emsp;You can use this project to create a simple graph. You may choose unidirected or undirected mode, add nodes or edges. And even visit all the neighbors in index order. Edge has changeable weight, and whether node or edge can be added or removed.\
&emsp;&emsp;- **Graph(int nodeCount = 0, bool directed = false)** Create a new graph with `nodeCount` nodes(`default: 0`), and `directed` whether the edge is directed or not(`default: undirected`).\
&emsp;&emsp;- **bool isDirected() const** To get whether the edge is directed or not.\
&emsp;&emsp;- **int getNodeCount() const** To get the number of node(s) in the whole graph.\
&emsp;&emsp;- **int getEdgeCount() const** To get the number of edge(s) in the whole graph, every two nodes have no more than one undirected edge or two unidirected edge.\
&emsp;&emsp;- **std::vector<std::pair<int, double>> getNeighbors(int nodeIndex)** To get all the (outer) neighbor(s) of `nodeindex`.\
&emsp;&emsp;- **void addNode(); void addNodes(int count)** To append new (`count`) node(s).\
&emsp;&emsp;- **void addEdge(std::tuple<int, int, double> edge); void addEdges(const std::vector<std::tuple<int, int, double>>& edges)** To insert new edge(s) into graph. If the edge existed, it will only add the weight into existed edge.\
&emsp;&emsp;- **void void updateEdgeWeight(std::tuple<int, int, double> newEdge); void updateEdgeWeights(const std::vector<std::tuple<int, int, double>>& newEdges)** To change edge(s)' weight if it has existed.\
&emsp;&emsp;- **void removeNode(int index); void removeNodes(std::vector<int> index)** Remove node(s) of `index` with all the related edges. _**Notice: This operate will change post-deleted nodes' index.**_\
&emsp;&emsp;- **void removeEdge(std::pair<int, int> neighborPair); void removeEdges(const std::vector<std::pair<int, int>>& neighborPairs)** Remove edge(s) of `neighborPair(s)` if existed.
#### Want to know more? See [Examples](Examples.md#examples-for-graphh---1).
****
&emsp;&emsp;Now, you may use three classical algorithms (_**DFS, BFS and Floyd Warshall**_) to traversal graph.\
&emsp;&emsp;- **bool dfs(Graph graph)** To check whether the whole `graph` is *well-connected* or not.\
&emsp;&emsp;- **bool bfs(Graph graph, std::pair<int, int> objPair)** To search whether the `objPair`'s first node can reach the second one in `graph` or not.\
&emsp;&emsp;- **std::vector<std::vector<double>> floyd(Graph graph)** To calculate the minimal distance of every pair nodes in `graph` use *Floyd Warshall* algorithm. This algorithm is base on *Dynamic Programming* method, and can solve negative distance situation. **Negative loop is forbidden**. 
#### Want to know more? See [Examples](Examples.md#examples-for-algorithmh---1).
****
&emsp;&emsp;Network's generator methods are _**Not yet tested**_.

# Coming Soon
****
Random or regular networks' generator _**WILL COME SOON**_.

# Configuration
cmake minimum required: V3.25\
cmake standard: C++ 20\
Open Source License: [MIT LICENSE](LICENSE)

# Development Log
****
**2025-12-20 first**: Created the *Graph* class with attributes and method.\
**2025-12-20 second**: Tested the *Graph* class and _**fix bugs**_, such as edge count synchronization, add edges, and adjacency list.\
**2025-12-21 first**: Graph support the **edge's weight** now, the weight can be changed arbitrarily. Edge and node can be added or removed. When add an existed edge, it will be combined into weight with add operate.\
**2025-12-22 first**: **DFS, BFS and Floyd Warshall** algorithms are supported.\
**2025-12-24 first**: [_**Not yet tested**_]Network's generator, regular structure, Erdös-Rényi random network and Watts-Strogatz small world network are supported.
