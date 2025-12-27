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

## Examples for algorithm.h - 1
```C++
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
```

## Examples for generator.h and stats.h - 1
```C++
void testForComplex_1() {
    gen.seed(1024); // Make it repeatable.
    Graph fc = fullConnect(20); // Full connected network.
    std::pair<std::pair<double, double>, std::vector<double>> fcd = degreeDistribution(fc);
    double fcc = clusterCoefficient(fc);
    std::tuple<double, double, double> fcl = distance(fc);
    std::cout << "The full connected network with 20 nodes will have equality degrees (19 for every node), maximum cluster coefficient (1) and same distances for each pair of nodes (1)." << std::endl;
    std::cout << "Average degree: " << fcd.first.first << ", variance of degree: " << fcd.first.second << "." << std::endl;
    std::cout << "Cluster coefficient: " << fcc << "." << std::endl;
    std::cout << "Average distance: " << std::get<1>(fcl) << ", distance distribution area: [" << std::get<0>(fcl) << ", " << std::get<2>(fcl) << "]." << std::endl;
    Graph lt = lattice(40); // 40*40 with period boundary condition 2D-lattice network.
    std::pair<std::pair<double, double>, std::vector<double>> ltd = degreeDistribution(lt);
    double ltc = clusterCoefficient(lt);
    std::cout << "The 2D-lattice grid with 40*40 nodes will have equality degrees (2*2=4 for every node), no cluster coefficient." << std::endl;
    std::cout << "Average degree: " << ltd.first.first << ", variance of degree: " << ltd.first.second << "." << std::endl;
    std::cout << "Cluster coefficient: " << ltc << "." << std::endl;
    std::pair<std::pair<double, double>, std::vector<double>> fdr = degreeDistribution(ring(99, 4));
    std::pair<std::pair<double, double>, std::vector<double>> ndr = degreeDistribution(oddRegular(100, 9));
    std::pair<std::pair<double, double>, std::vector<double>> err = degreeDistribution(erRandom(100, 9, .3, true)); // Regular graphs.
    std::cout << "Regular graph (especially ring structure here) will have equality degrees." << std::endl;
    std::cout << "Four-degree ring's average degree: " << fdr.first.first << ", variance of degree: " << fdr.first.second << "." << std::endl;
    std::cout << "Nine-degree ring's average degree: " << ndr.first.first << ", variance of degree: " << ndr.first.second << "." << std::endl;
    std::cout << "Erdös-Rényi regular random network's average degree: " << err.first.first << ", variance of degree: " << err.first.second << "." << std::endl;
    Graph erl = erRandom(200, -1, .04);
    Graph erh = erRandom(200, -1, .40); // Erdös-Rényi graph with different connect rate.
    std::cout << "Erdös-Rényi with lower connect rate will have larger average distance (" << std::get<1>(distance(erl)) << ") than the higher one (" << std::get<1>(distance(erh)) << ")." << std::endl;
    wsSmallWorld(lt, .5); // Make the lattice be small world.
    double swd = std::get<2>(distance(lt));
    std::cout << "Small world network satisfied six-degree of separation theorem, so the longest distance is " << swd << " for 1600 nodes with " << ltd.first.first << " average degree." << std::endl;
    Graph sf = baScaleFree(1000, 5);
    std::pair<std::pair<double, double>, std::vector<double>> sfd = degreeDistribution(sf);
    std::ofstream outfile;
    outfile.open("..\\tests\\out.csv", std::ios::out);
    for (int i = 0; i < sfd.second.size(); ++i) {
        outfile << i << "," << sfd.second[i] << std::endl;
    }
    outfile.close();
    std::cout << "Barabási-Albert scale-free network has power-law degree distribution, see \"out.csv\"." << std::endl;
    /********************************************************************************************************************************/
    std::cout << "Then you will see some errors that for incorrect calls." << std::endl;
    Sleep(2000);
    ring(-1), ring(4, 4), ring(10, 3), ring(10, 0);
    oddRegular(10, 4), oddRegular(10, 1), oddRegular(10, 11), oddRegular(9, 5);
    baScaleFree(10, 12), baScaleFree(11, 7);
    clusterCoefficient(Graph(5, true));
}
```
### Appendix: Barabási-Albert scale-free network's degree distribution
![out.bmp](tests/out.bmp)
[out.csv](tests/out.csv)
