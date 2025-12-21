## Examples for Graph.h - 1
```C++
void testForGraph_2() {
    /* First of all, test basic functions */
    Graph graph(3); // Create a new undirected graph with 3 nodes.
    std::cout << "The graph is directed? " << graph.isDirected() << ". It should be FALSE." << std::endl; // Confirm type of graph.
    std::cout << "There are " << graph.getNodeCount() << " nodes, it should be 3." << std::endl; // Show the node count.
    std::cout << "There are " << graph.getEdgeCount() << " edges, it should be 0." << std::endl; // An empty graph, i.e. without any edges.
    std::vector<std::tuple<int, int, double>> validEdges = {{0, 1, 3.}, {1, 2, 4.}, {2, 0, 5.}};
    graph.addEdges(validEdges); // Create a right-triangle structure.
    for (int i = 0; i < graph.getNodeCount(); i++) {
        std::cout << "The " << i << "th node has " << graph.getNeighbors(i).size() << " neighbor(s): " << std::endl;
        for (std::pair<int, double> neighbor: graph.getNeighbors(i)) {
            std::cout << "The edge between " << i << " and " << neighbor.first << " is weighted by " << neighbor.second << "." << std::endl;
        }
        std::cout << "." << std::endl;
    } // Show all neighbors' information of everyone.
    graph.addNode(); // Add one node.
    graph.addNodes(2); // Add extra two nodes.
    graph.addEdges({{3, 4, 5.}, {4, 5, 3.}, {3, 5, 4.}}); // And another right-triangle structure.
    std::cout << "Now, another right-triangle created." << std::endl;
    for (int i = 3; i < graph.getNodeCount(); i++) {
        std::cout << "The " << i << "th node has " << graph.getNeighbors(i).size() << " neighbor(s): " << std::endl;
        for (std::pair<int, double> neighbor: graph.getNeighbors(i)) {
            std::cout << "The edge between " << i << " and " << neighbor.first << " is weighted by " << neighbor.second << "." << std::endl;
        }
        std::cout << "." << std::endl;
    } // Show the new sub-graph.
    graph.updateEdgeWeights({{4, 5, 12.}, {3, 5, 13.}});
    std::cout << "Now, the second right-triangle has been updated." << std::endl;
    for (int i = 0; i < graph.getNodeCount(); i++) {
        std::cout << "The " << i << "th node has " << graph.getNeighbors(i).size() << " neighbor(s): " << std::endl;
        for (std::pair<int, double> neighbor: graph.getNeighbors(i)) {
            std::cout << "The edge between " << i << " and " << neighbor.first << " is weighted by " << neighbor.second << "." << std::endl;
        }
        std::cout << "." << std::endl;
    } // Show all neighbors' information of everyone.
    graph.removeNodes({4, 5}); // Remove two nodes with there all related edges.
    graph.removeEdges({{0, 1}, {2, 1}}); // Remove two existed edges.
    std::cout << "Finally, the graph remain " << graph.getNodeCount() << " nodes(it must be 4) and " << graph.getEdgeCount() << " edge(it must be 1)." << std::endl;
    /********************************************************************************************************************************/
    std::cout << "Then you will see some errors that for incorrect calls." << std::endl;
    Sleep(2000);
    /* Then, attempt to test for incorrect calls */
    graph.getNeighbors(1000);
    graph.addEdges({{0, 100, -1.}, {100, 0, -1.}});
    graph.addEdge({0, 100, -1.});
    graph.addEdge({100, 0, -1.});
    graph.updateEdgeWeights({{100, 200, -2.}, {0, 3, -10.}});
    graph.removeNode(1000);
    graph.removeEdges({{100, 200}, {0, 3}});
}
```
