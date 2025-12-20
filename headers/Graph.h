//
// Created by Ivor on 2025/12/20.
//

#ifndef GRAPHENGINE_GRAPH_H
#define GRAPHENGINE_GRAPH_H

#include <vector>
#include <iostream>

class Graph {
private:
    const bool directed;
    int nodeCount, edgeCount;
    std::vector<std::vector<int>> adjacencyList;

public:
    explicit Graph(bool directed = false, int nodeCount = 0);
    ~Graph();
    [[nodiscard]] bool isDirected() const;
    [[nodiscard]] int getNodeCount() const;
    [[nodiscard]] int getEdgeCount() const;
    std::vector<int> getNeighbors(int nodeIndex);
    void addNode();
    void addNodes(int count);
    void addEdge(std::pair<int, int> edge);
    void addEdges(const std::vector<std::pair<int, int>>& edges);
};

#endif //GRAPHENGINE_GRAPH_H
