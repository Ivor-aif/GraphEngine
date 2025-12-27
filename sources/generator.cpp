//
// Created by Ivor on 2025/12/24.
//

#include "../headers/generator.h"

Graph fullConnect(const int nodeCount) {
    auto ret = Graph(nodeCount);
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < i; ++j) {
            ret.addEdge({i, j, 1.});
        }
    }
    return ret;
}

Graph lattice(const int size, const int dim) {
    int nodeCount = 1;
    for (int i = 0; i < dim; ++i) {
        nodeCount *= size;
    }
    auto ret = Graph(nodeCount, false);
    for (int i = 0; i < nodeCount; ++i) {
        int tier = 1;
        for (int j = 0; j < dim; ++j) {
            int neighborIndex = i + tier;
            if (((i / tier) % size) == size - 1) {
                neighborIndex = i - (size-1) * tier;
            }
            ret.addEdge({i, neighborIndex, 1.});
            tier *= size;
        }
    }
    return ret;
}

Graph ring(const int nodeCount, const int degree) {
    if (degree < 0) {
        std::cerr << "Nodes cannot have NEGATIVE neighbors: " << degree << "." << std::endl;
        return Graph();
    }
    if (degree & 1) {
        std::cerr << "Nodes on ring structure MUST have even neighbor(s), " << degree << " is invalid." << std::endl;
        return Graph();
    }
    if (degree >= nodeCount) {
        std::cerr << "Cannot generate a graph with " << nodeCount << " node(s), and each node have " << degree << " neighbor(s)." << std::endl;
        return Graph();
    }
    if (!degree) {
        std::cout << "Warning! Nodes will have no neighbor." << std::endl;
    }
    auto ret = Graph(nodeCount, false);
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < (degree >> 1); ++j) {
            ret.addEdge({i, (i+1+j) % nodeCount, 1.});
        }
    }
    return ret;
}

Graph oddRegular(const int nodeCount, const int degree) {
    if (degree < 3) {
        std::cerr << "Too few neighbor(s): " << degree << "." << std::endl;
        return Graph();
    }
    if (!(degree & 1)) {
        std::cerr << "Here we need a odd number, " << degree << " is invalid. You may try `ring` method to generate." << std::endl;
        return Graph();
    }
    if (degree >= nodeCount) {
        std::cerr << "Cannot generate a graph with " << nodeCount << " node(s), and each node have " << degree << " neighbor(s)." << std::endl;
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

void reconnect(Graph& graph, const double rate, const int mode) {
    const int nodeCount = graph.getNodeCount();
    std::uniform_real_distribution<double> dist(0., 1.);
    std::uniform_int_distribution<int> range(0, nodeCount - 1);
    for (int i = 0; i < nodeCount; ++i) {
        if (!mode) {
            for (int j = 0; j < i; ++j) {
                if (dist(gen) < rate) {
                    graph.addEdge({i, j, 1.});
                }
            }
        } else if (!~mode) {
            for (std::vector<std::pair<int, double>> neighbors = graph.getNeighbors(i); std::pair<int, double> neighbor: neighbors) {
                if (i < neighbor.first) {
                    break;
                }
                if (dist(gen) < rate) {
                    if (!std::ranges::binary_search(graph.getNeighbors(i), std::make_pair(neighbor.first, 1.),
                                                    [](const std::pair<int, double>& left, const std::pair<int, double>& right) {
                                                        return left.first < right.first;
                                                    })) {
                        continue;
                    }
                    graph.removeEdge({i, neighbor.first});
                    int newNeighbor = range(gen);
                    while (newNeighbor == i || std::ranges::binary_search(graph.getNeighbors(i), std::make_pair(newNeighbor, 1.),
                                                                          [](const std::pair<int, double>& left, const std::pair<int, double>& right) {
                                                                              return left.first < right.first;
                                                                          })) {
                        newNeighbor = range(gen);
                    }
                    graph.addEdge({i, newNeighbor, 1.});
                    std::pair<int, int> nextEdge(newNeighbor, -1);
                    while (true) {
                        std::uniform_int_distribution<int> local(0, static_cast<int>(graph.getNeighbors(nextEdge.first).size()) - 1);
                        int iter = graph.getNeighbors(nextEdge.first)[local(gen)].first;
                        graph.removeEdge({nextEdge.first, iter});
                        nextEdge.first = iter;
                        while (nextEdge.first == nextEdge.second || !~nextEdge.second ||
                               std::ranges::binary_search(graph.getNeighbors(nextEdge.first), std::make_pair(nextEdge.second, 1.),
                                                          [](const std::pair<int, double>& left, const std::pair<int, double>& right) {
                                                              return left.first < right.first;
                                                          })) {
                            nextEdge.second = range(gen);
                        }
                        graph.addEdge({nextEdge.first, nextEdge.second, 1.});
                        if (nextEdge.second == neighbor.first) {
                            break;
                        }
                        nextEdge.first = nextEdge.second;
                        nextEdge.second = -1;
                    }
                }
            }
        } else {
            for (std::vector<std::pair<int, double>> neighbors = graph.getNeighbors(i); std::pair<int, double> neighbor: neighbors) {
                if (i < neighbor.first) {
                    break;
                }
                if (dist(gen) < rate) {
                    graph.removeEdge({i, neighbor.first});
                    int newNeighbor = range(gen);
                    while (newNeighbor == i || std::ranges::binary_search(graph.getNeighbors(i), std::make_pair(newNeighbor, 1.),
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

Graph erRandom(const int nodeCount, const int averageDegree, const double connectRate, const bool isRegular) {
    if (isRegular) {
        if (averageDegree & 1) {
            Graph ret = oddRegular(nodeCount, averageDegree);
            reconnect(ret, connectRate, -1);
            return ret;
        }
        Graph ret = ring(nodeCount, averageDegree);
        reconnect(ret, connectRate, -1);
        return ret;
    }
    std::cout << "Arg `averageDegree` is not effective for irregular Erdös-Rényi random network." << std::endl;
    auto ret = Graph(nodeCount);
    reconnect(ret, connectRate, 0);
    return ret;
}

void wsSmallWorld(Graph& graph, const double reconnectRate) {
    reconnect(graph, reconnectRate, 1);
}

Graph baScaleFree(const int nodeCount, const int averageDegree) {
    if (nodeCount + 1 < averageDegree) {
        std::cerr << "Too few node(s) " << nodeCount << " for " << averageDegree << " average degree." << std::endl;
        return Graph();
    }
    if ((averageDegree & 1) && (nodeCount & 1)) {
        std::cerr << "Neither average degree " << averageDegree << " nor node count " << nodeCount <<" can be odd." << std::endl;
        return Graph();
    }
    Graph ret = fullConnect(averageDegree + 1);
    std::vector<int> degreeList(averageDegree + 1, averageDegree);
    for (int i = averageDegree + 1; i < nodeCount; ++i) {
        ret.addNode();
        const int selectedCount = (averageDegree >> 1) + ((averageDegree & 1) ? (i & 1) : 0);
        std::vector<int> selected(0);
        std::vector<int> weights(degreeList.size());
        std::ranges::copy(degreeList, weights.begin());
        int totalWeight = std::accumulate(weights.begin(), weights.end(), 0);
        for (int j = 0; j < selectedCount; ++j) {
            std::uniform_int_distribution<int> sample(0, totalWeight - 1);
            int locate = sample(gen);
            for (int k = 0; k < i; ++k) {
                if (!weights[k]) {
                    continue;
                }
                locate -= weights[k];
                if (locate <= 0) {
                    selected.push_back(k);
                    totalWeight -= weights[k];
                    weights[k] = 0;
                    break;
                }
            }
        }
        for (int obj: selected) {
            ret.addEdge({i, obj, 1.});
            degreeList[obj] ++;
        }
        degreeList.push_back(averageDegree);
    }
    return ret;
}
