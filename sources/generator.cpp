//
// Created by Ivor on 2025/12/24.
//

#include "../headers/generator.h"

Graph lattice(int size, int dim) {
    int nodeCount = 1;
    for (int i = 0; i < dim; ++i) {
        nodeCount *= size;
    }
    Graph ret = Graph(nodeCount, false);
    for (int i = 0; i < nodeCount; ++i) {
        int tier = 1;
        for (int j = 0; j < dim; ++j) {
            int neighborIndex = (i + tier) % (tier * size);
            ret.addEdge({i, neighborIndex, 1.});
            tier *= size;
        }
    }
    return ret;
}

Graph ring(int nodeCount, int degree) {
    if (degree < 0) {
        std::cerr << "Nodes cannot have NEGATIVE neighbors: " << degree << "." << std::endl;
        return Graph();
    }
    if (degree & 1) {
        std::cerr << "Nodes on ring structure MUST have even neighbor(s), " << degree << " is invalid." << std::endl;
        return Graph();
    }
    if (degree >= nodeCount) {
        std::cerr << "Cannot generate a graph with " << nodeCount << " node(s), and each node have" << degree << " neighbor(s)." << std::endl;
        return Graph();
    }
    if (!degree) {
        std::cout << "Warning! Nodes will have no neighbor." << std::endl;
    }
    Graph ret = Graph(nodeCount, false);
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < (degree >> 1); ++j) {
            ret.addEdge({i, (i+1+j) % nodeCount, 1.});
        }
    }
    return ret;
}

Graph oddRegular(int nodeCount, int degree) {
    if (degree < 3) {
        std::cerr << "Too few neighbor(s): " << degree << "." << std::endl;
        return Graph();
    }
    if (!(degree & 1)) {
        std::cerr << "Here we need a odd number, " << degree << " is invalid. You may try `ring` method to generate." << std::endl;
        return Graph();
    }
    if (degree >= nodeCount) {
        std::cerr << "Cannot generate a graph with " << nodeCount << " node(s), and each node have" << degree << " neighbor(s)." << std::endl;
        return Graph();
    }
    if (nodeCount & 1) {
        std::cerr << "Odd regular graph must have even nodes, " << nodeCount << " is invalid." << std::endl;
    }
    Graph ret = ring(nodeCount, degree-1);
    for (int i = 0; i < (nodeCount >> 1); ++i) {
        ret.addEdge({i, i + (nodeCount >> 1), 1.});
    }
    return ret;
}

void reconnect(Graph& graph, double rate, int mode) {
    int nodeCount = graph.getNodeCount();
    std::uniform_real_distribution<double> dist(0., 1.);
    std::uniform_int_distribution<int> range(0, nodeCount);
    for (int i = 0; i < nodeCount; ++i) {
        if (!mode) {
            for (int j = 0; j < i; ++j) {
                if (dist(gen) < rate) {
                    graph.addEdge({i, j, 1.});
                }
            }
        } else if (!~mode) {
            std::vector<std::pair<int, double>> neighbors = graph.getNeighbors(i);
            for (std::pair<int, double> neighbor: neighbors) {
                if (i < neighbor.first) {
                    break;
                }
                if (dist(gen) < rate) {
                    graph.removeEdge({i, neighbor.first});
                    int newNeighbor = range(gen);
                    while (newNeighbor == i || std::binary_search(graph.getNeighbors(i).begin(), graph.getNeighbors(i).end(), std::make_pair(newNeighbor, 1.),
                                                                  [](const std::pair<int, double>& left, const std::pair<int, double>& right) {
                                                                      return left.first < right.first;
                                                                  })) {
                        newNeighbor = range(gen);
                    }
                    graph.addEdge({i, newNeighbor, 1.});
                    std::pair<int, int> nextEdge(newNeighbor, range(gen));
                    while (true) {
                        while (nextEdge.first == nextEdge.second ||
                               std::binary_search(graph.getNeighbors(nextEdge.first).begin(), graph.getNeighbors(nextEdge.first).end(), std::make_pair(nextEdge.second, 1.),
                                                  [](const std::pair<int, double>& left, const std::pair<int, double>& right) {
                                                      return left.first < right.first;
                                                  })) {
                            nextEdge.second = range(gen);
                        }
                        std::uniform_int_distribution<int> local(0, (int)graph.getNeighbors(nextEdge.first).size());
                        graph.removeEdge({nextEdge.first, graph.getNeighbors(nextEdge.first)[local(gen)].first});
                        graph.addEdge({nextEdge.first, nextEdge.second, 1.});
                        if (nextEdge.second == i) {
                            break;
                        } else {
                            nextEdge.first = nextEdge.second;
                            nextEdge.second = range(gen);
                        }
                    }
                }
            }
        } else {
            std::vector<std::pair<int, double>> neighbors = graph.getNeighbors(i);
            for (std::pair<int, double> neighbor: neighbors) {
                if (i < neighbor.first) {
                    break;
                }
                if (dist(gen) < rate) {
                    graph.removeEdge({i, neighbor.first});
                    int newNeighbor = range(gen);
                    while (newNeighbor == i || std::binary_search(graph.getNeighbors(i).begin(), graph.getNeighbors(i).end(), std::make_pair(newNeighbor, 1.),
                                                                  [](const std::pair<int, double>& left, const std::pair<int, double>& right) {
                                                                      return left.first < right.first;
                                                                  })) {
                        newNeighbor = range(gen);
                    }
                    graph.addEdge({i, newNeighbor, 1.});
                }
            }
        }
    }
}

Graph erRandom(int nodeCount, int averageDegree, double connectRate, bool isRegular) {
    if (isRegular) {
        if (averageDegree & 1) {
            Graph ret = oddRegular(nodeCount, averageDegree);
            reconnect(ret, connectRate, -1);
            return ret;
        } else {
            Graph ret = ring(nodeCount, averageDegree);
            reconnect(ret, connectRate, -1);
            return ret;
        }
    } else {
        std::cout << "Arg `averageDegree` is not effective for irregular Erdös-Rényi random network." << std::endl;
        Graph ret = Graph(nodeCount);
        reconnect(ret, connectRate, 0);
        return ret;
    }
}

void wsSmallWorld(Graph& graph, double reconnectRate) {
    reconnect(graph, reconnectRate, 1);
}
