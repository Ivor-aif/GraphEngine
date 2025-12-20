# GraphEngine
Simply process graphs/networks.

# Project Description
****
Now, you can use this project to create a simple graph. You can choose unidirected or undirected mode, add nodes or edges. And even visit all the neighbors in index order. See examples:
```c++
Graph graph(false, 3); // Create a new undirected graph with 3 nodes.
std::cout << "The graph is directed? " << graph.isDirected() << ". It should be FALSE." << std::endl; // Confirm type of graph.
std::cout << "There are " << graph.getNodeCount() << " nodes, it should be 3." << std::endl; // Show the node count.
std::cout << "There are " << graph.getEdgeCount() << " edges, it should be 0." << std::endl; // A empty graph, i.e. without any edges.
std::vector<std::pair<int, int>> validEdges = {{0, 1}, {1, 2}, {2, 0}};
std::vector<std::pair<int, int>> invalidEdges = {{0, 3}, {4, 5}};
graph.addEdges(validEdges); // Create a triangle structure.
for (int i = 0; i < graph.getNodeCount(); i++) {
    std::cout << "The " << i << "th node has " << graph.getNeighbors(i).size() << " neighbor(s)." << std::endl;
    std::cout << "They are ";
    for (int index: graph.getNeighbors(i)) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
} // Show all neighbors of everyone.
std::cout << "Then you will see some errors that invalid edges cannot be insert." << std::endl;
graph.addEdges(invalidEdges); // Invalid value test.
graph.addNode(); // Add one node.
graph.addEdge(invalidEdges[0]); // It is valid now.
graph.addNodes(2); // Add extra two nodes.
graph.addEdge(invalidEdges[1]); // It is valid, too.
for (int i = 0; i < graph.getNodeCount(); i++) {
    std::cout << "The " << i << "th node has " << graph.getNeighbors(i).size() << " neighbor(s)." << std::endl;
    std::cout << "They are ";
    for (int index: graph.getNeighbors(i)) {
        std::cout << index << " ";
    }
    std::cout << std::endl;
} // Finally, there are 6 nodes and 5 undirected edges.
```

# Coming soon
****
Graph _**WILL BE**_ support weight, and repeating edges will be merged by weight.

# Configration
cmake minimum required: V3.25\
cmake standard: C++ 20\
Open Source License: [MIT LICENSE](LICENSE)

# Development Log
****
**2025-12-20 first**: Created the *Graph* class with attributes and method.\
**2025-12-20 second**: Tested the *Graph* class and _**fix bugs**_, such as edge count synchronization, add edges, and adjacency list.
