#include <windows.h>

#include "headers/Graph.h"
#include "headers/algorithm.h"

// void testForGraph_1(); // Something has been updated that the old usages may not available.
void testForGraph_2();
void testForAlgorithm_1();

int main() {
    // testForGraph_1(); Something has been updated that the old usages may not available.
    // testForGraph_2();
    testForAlgorithm_1();
    return 0;
}

void testForAlgorithm_1() {
    Graph graph(5);
    graph.addEdges({{1, 2, 2.}, {2, 3, 2.}, {3, 4, 2.}, {4, 1, 2.}});
    graph.addEdges({{0, 1, 1.414}, {0, 2, 1.414}, {0, 3, 1.414}, {0, 4, 1.414}}); // Create a square with 'x'.
    std::cout << "The graph is well-connected, right?" << (dfs(graph) ? " Yes." : " No!") << std::endl; // DFS is for confirm whether the whole graph is well-connected or not.
    graph.addNodes(3);
    graph.addEdges({{5, 6, 1.}, {6, 7, 1.}, {7, 5, 1.}}); // Add a subgraph.
    std::cout << "Now, it is not well-connected, You will see FALSE. " << dfs(graph) << std::endl;
    std::cout << "Nodes between 1 and 3 are connected: " << bfs(graph, {1, 3}) << "." << std::endl; // BFS is for confirm whether the first node can reach the second one or not.
    std::cout << "Nodes between 1 and 7 are not connected: " << bfs(graph, {1, 7}) << "." << std::endl;
    auto dist = floyd(graph); // Floyd is Floyd-Warshall algorithm, to calculate the minimal distance of every pair nodes.
    std::cout << "The minimal distance between 1 and 3 is " << dist[1][3] << "." << std::endl;
    /********************************************************************************************************************************/
    std::cout << "Then you will see some errors that for incorrect calls." << std::endl;
    Sleep(2000);
    /* Then, attempt to test for incorrect calls */
    bfs(graph, {100, 0});
    bfs(graph, {0, 100}); // Overflow errors.
    Graph emptyGraph;
    dfs(emptyGraph);
    bfs(emptyGraph, {0, 0});
    floyd(emptyGraph); // Empty errors.
}

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

/*
void testForGraph_1() {
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
}
*/ // Something has been updated that the old usages may not available.
