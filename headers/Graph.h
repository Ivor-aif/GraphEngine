//
// Created by Ivor on 2025/12/20.
//

#ifndef GRAPHENGINE_GRAPH_H
#define GRAPHENGINE_GRAPH_H

#include <vector>
#include <tuple>
#include <iostream>
#include <algorithm>

class Graph {
private:
    bool directed;
    int nodeCount, edgeCount;
    std::vector<std::vector<std::pair<int, double>>> adjacencyList;

public:
    explicit Graph(int nodeCount = 0, bool directed = false);
    ~Graph();
    [[nodiscard]] bool isDirected() const;
    [[nodiscard]] int getNodeCount() const;
    [[nodiscard]] int getEdgeCount() const;
    [[nodiscard]] const std::vector<std::pair<int, double>>& getNeighbors(int nodeIndex) const;
    void addNode();
    void addNodes(int count);
    void addEdge(const std::tuple<int, int, double>& edge);
    void addEdges(const std::vector<std::tuple<int, int, double>>& edges);
    void updateEdgeWeight(const std::tuple<int, int, double>& newEdge);
    void updateEdgeWeights(const std::vector<std::tuple<int, int, double>>& newEdges);
    void removeNode(int index);
    void removeNodes(std::vector<int> index);
    void removeEdge(std::pair<int, int> neighborPair);
    void removeEdges(const std::vector<std::pair<int, int>>& neighborPairs);
};

#endif // GRAPHENGINE_GRAPH_H
