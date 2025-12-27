//
// Created by Ivor on 2025/12/20.
//

#include "../headers/Graph.h"

Graph::Graph(const int nodeCount, const bool directed) : directed(directed), nodeCount(nodeCount) {
    this->edgeCount = 0;
    this->adjacencyList = std::vector<std::vector<std::pair<int, double>>>(nodeCount, std::vector<std::pair<int, double>>(0, std::pair<int, double>(0, 0.)));
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

const std::vector<std::pair<int, double>>& Graph::getNeighbors(const int nodeIndex) const {
    static constexpr std::vector<std::pair<int, double>> empty;
    if (nodeIndex < 0 || nodeIndex >= this->getNodeCount()) {
        std::cerr << "Can not get " << nodeIndex << "th node in " << this->getNodeCount() << "node(s) graph." << std::endl;
        return empty;
    }
    return adjacencyList[nodeIndex];
}

void Graph::addNode() {
    this->addNodes(1);
}

void Graph::addNodes(const int count) {
    this->nodeCount += count;
    for (int i = 0; i < count; i++) {
        this->adjacencyList.emplace_back(0);
    }
}

void Graph::addEdge(const std::tuple<int, int, double>& edge) {
    if (std::get<0>(edge) >= this->getNodeCount()) {
        std::cerr << "The first node index(" << std::get<0>(edge) << ") of the new edge out of this graph with " << this->getNodeCount() << " node(s)." << std::endl;
        return;
    }
    if (std::get<1>(edge) >= this->getNodeCount()) {
        std::cerr << "The second node index(" << std::get<1>(edge) << ") of the new edge out of this graph with " << this->getNodeCount() << " node(s)." << std::endl;
        return;
    }
    this->edgeCount ++;
    int left = std::get<0>(edge), right = std::get<1>(edge);
    double weight = std::get<2>(edge);
    for (int i = 0; i <= this->getNeighbors(left).size(); ++i) {
        if (i != this->getNeighbors(left).size() && right == this->adjacencyList[left][i].first) {
            this->adjacencyList[left][i].second += weight;
            this->edgeCount --;
            break;
        }
        if (i == this->getNeighbors(left).size() || right < this->adjacencyList[left][i].first) {
            this->adjacencyList[left].insert(this->adjacencyList[left].begin() + i, std::pair<int, double>(right, weight));
            break;
        }
    }
    if (!this->isDirected()) {
        for (int i = 0; i <= this->getNeighbors(right).size(); ++i) {
            if (i != this->getNeighbors(right).size() && left == this->adjacencyList[right][i].first) {
                this->adjacencyList[right][i].second += weight;
                break;
            }
            if (i == this->getNeighbors(right).size() || left < this->adjacencyList[right][i].first) {
                this->adjacencyList[right].insert(this->adjacencyList[right].begin() + i, std::pair<int, double>(left, weight));
                break;
            }
        }
    }
}

void Graph::addEdges(const std::vector<std::tuple<int, int, double>>& edges) {
    for (std::tuple<int, int, double> edge: edges) {
        if (std::get<0>(edge) >= this->getNodeCount()) {
            std::cerr << "The first node index(" << std::get<0>(edge) << ") of the new edge [" << std::get<0>(edge) << ", " << std::get<1>(edge) << "] out of this graph with " << this->getNodeCount() << " node(s)." << std::endl;
            continue;
        }
        if (std::get<1>(edge) >= this->getNodeCount()) {
            std::cerr << "The second node index(" << std::get<1>(edge) << ") of the new edge [" << std::get<0>(edge) << ", " << std::get<1>(edge) << "] out of this graph with " << this->getNodeCount() << " node(s)." << std::endl;
            continue;
        }
        this->addEdge(edge);
    }
}

void Graph::updateEdgeWeight(const std::tuple<int, int, double>& newEdge) {
    if (std::get<0>(newEdge) >= this->getNodeCount() || std::get<1>(newEdge) >= this->getNodeCount()) {
        std::cerr << "No such edge [" << std::get<0>(newEdge) << ", " << std::get<1>(newEdge) << "] in graph with " << this->getNodeCount() << " node(s)." << std::endl;
        return;
    }
    const int left = std::get<0>(newEdge), right = std::get<1>(newEdge);
    const double weight = std::get<2>(newEdge);
    for (int i = 0; i <= this->getNeighbors(left).size(); ++i) {
        if (i == this->getNeighbors(right).size()) {
            std::cerr << "Cannot find such edge [" << left << ", " << right << "] in graph." << std::endl;
            return;
        }
        if (right == this->adjacencyList[left][i].first) {
            this->adjacencyList[left][i].second = weight;
            break;
        }
    }
    if (!this->isDirected()) {
        for (int i = 0; i < this->getNeighbors(right).size(); ++i) {
            if (left == this->adjacencyList[right][i].first) {
                this->adjacencyList[right][i].second = weight;
                break;
            }
        }
    }
}

void Graph::updateEdgeWeights(const std::vector<std::tuple<int, int, double>>& newEdges) {
    for (const std::tuple<int, int, double> newEdge: newEdges) {
        this->updateEdgeWeight(newEdge);
    }
}

void Graph::removeNode(int index) {
    if (index >= this->getNodeCount()) {
        std::cerr << "Remove fail! Index " << index << " out of " << this->getNodeCount() << " in this graph." << std::endl;
        return;
    }
    for (int i = 0; i < this->getNodeCount(); ++i) {
        if (i == index) {
            continue;
        }
        std::vector<std::pair<int, double>>& neighbors = this->adjacencyList[i];
        const int size = static_cast<int>(neighbors.size());
        std::erase_if(neighbors, [index](const std::pair<int, double>& obj){
            return obj.first == index;
        });
        if (this->isDirected()) {
            this->edgeCount -= (size != static_cast<int>(neighbors.size()));
        }
    }
    this->edgeCount -= static_cast<int>(this->adjacencyList[index].size());
    this->adjacencyList.erase(this->adjacencyList.begin() + index);
    this->nodeCount --;
}

void Graph::removeNodes(std::vector<int> index) {
    std::ranges::sort(index, [](const int left, const int right) {
        return left > right;
    });
    for (const int i: index) {
        this->removeNode(i);
    }
}

void Graph::removeEdge(std::pair<int, int> neighborPair) {
    if (neighborPair.first >= this->getNodeCount() || neighborPair.second >= this->getNodeCount()) {
        std::cerr << "No such edge [" << neighborPair.first << ", " << neighborPair.second << "] in graph with " << this->getNodeCount() << " node(s)." << std::endl;
        return;
    }
    const int size = static_cast<int>(this->getNeighbors(neighborPair.first).size());
    std::vector<std::pair<int, double>>& neighbors = this->adjacencyList[neighborPair.first];
    std::erase_if(neighbors, [neighborPair](const std::pair<int, double>& neighbor) {
        return neighborPair.second == neighbor.first;
    });
    if (size == static_cast<int>(this->getNeighbors(neighborPair.first).size())) {
        std::cerr << "Cannot find such edge [" << neighborPair.first << ", " << neighborPair.second << "] in this graph." << std::endl;
        return;
    }
    this->edgeCount --;
    if (!this->isDirected()) {
        std::vector<std::pair<int, double>>& opNeighbors = this->adjacencyList[neighborPair.second];
        std::erase_if(opNeighbors, [neighborPair](const std::pair<int, double>& neighbor) {
            return neighborPair.first == neighbor.first;
        });
    }
}

void Graph::removeEdges(const std::vector<std::pair<int, int>>& neighborPairs) {
    for (const std::pair<int, int> neighborPair: neighborPairs) {
        this->removeEdge(neighborPair);
    }
}
