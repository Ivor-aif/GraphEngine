//
// Created by Ivor on 2025/12/20.
//

#include "../headers/Graph.h"

Graph::Graph(bool directed, int nodeCount) : directed(directed), nodeCount(nodeCount) {
    this->edgeCount = 0;
    this->adjacencyList = std::vector<std::vector<int>>(0, std::vector<int>(0, 0));
}

Graph::~Graph() = default;

bool Graph::isDirected() const {
    return this->directed;
}

int Graph::getNodeCount() const {
    return this->nodeCount;
}

int Graph::getEdgeCount() const {
    return this->edgeCount;
}

std::vector<int> Graph::getNeighbors(int nodeIndex) {
    if (nodeIndex >= this->getNodeCount()) {
        std::cerr << "Can not get " << nodeIndex << "th node in " << this->getNodeCount() << "node(s) graph." << std::endl;
        return std::vector<int>(0);
    }
    return this->adjacencyList[nodeIndex];
}

void Graph::addNode() {
    this->addNodes(1);
}

void Graph::addNodes(int count) {
    this->nodeCount += count;
    this->adjacencyList.emplace_back(count, 0);
}

void Graph::addEdge(std::pair<int, int> edge) {
    if (edge.first >= this->getNodeCount()) {
        std::cerr << "The first node index(" << edge.first << ") of the new edge out of this graph with " << this->getNodeCount() << " node(s)." << std::endl;
        return;
    }
    if (edge.second >= this->getNodeCount()) {
        std::cerr << "The second node index(" << edge.second << ") of the new edge out of this graph with " << this->getNodeCount() << " node(s)." << std::endl;
        return;
    }
    for (int i = 0; i <= this->adjacencyList[edge.first].size(); ++i) {
        if (i == this->adjacencyList[edge.first].size() || edge.second < this->adjacencyList[edge.first][i]) {
            this->adjacencyList[edge.first].insert(this->adjacencyList[edge.first].begin() + i, edge.second);
        }
    }
    if (!this->isDirected()) {
        for (int i = 0; i <= this->adjacencyList[edge.second].size(); ++i) {
            if (i == this->adjacencyList[edge.second].size() || edge.first < this->adjacencyList[edge.second][i]) {
                this->adjacencyList[edge.second].insert(this->adjacencyList[edge.second].begin() + i, edge.first);
            }
        }
    }
}

void Graph::addEdges(const std::vector<std::pair<int, int>>& edges) {
    for (std::pair<int, int> edge: edges) {
        if (edge.first >= this->getNodeCount()) {
            std::cerr << "The first node index(" << edge.first << ") of the new edge [" << edge.first << ", " << edge.second << "] out of this graph with " << this->getNodeCount() << " node(s)." << std::endl;
            return;
        }
        if (edge.second >= this->getNodeCount()) {
            std::cerr << "The second node index(" << edge.second << ") of the new edge [" << edge.first << ", " << edge.second << "] out of this graph with " << this->getNodeCount() << " node(s)." << std::endl;
            return;
        }
        this->addEdge(edge);
    }
}
