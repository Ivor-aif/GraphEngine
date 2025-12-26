//
// Created by Ivor on 2025/12/25.
//

#include "../headers/stats.h"
#include "../headers/algorithm.h"

std::pair<std::pair<double, double>, std::vector<double>> degreeDistribution(Graph graph) {
    if (graph.isDirected()) {
        std::cout << "For unidirected network, stat out degree only." << std::endl;
    }
    int nodeCount = graph.getNodeCount();
    std::vector<double> degreeStats(nodeCount);
    double mean = 0., variance = 0.;
    for (int i = 0; i < nodeCount; ++i) {
        int degree = (int)graph.getNeighbors(i).size();
        degreeStats[degree] += 1./nodeCount;
        mean += 1.*degree/nodeCount;
        variance += 1.*degree*degree/nodeCount;
    }
    variance -= mean*mean;
    return {{mean, variance}, degreeStats};
}

double clusterCoefficient(Graph graph) {
    if (graph.isDirected()) {
        std::cerr << "Undefined cluster coefficient for unidirected network." << std::endl;
        return 0.;
    }
    int nodeCount = graph.getNodeCount();
    double ret = 0.;
    for (int i = 0; i < nodeCount; ++i) {
        std::vector<std::pair<int, double>> nli = graph.getNeighbors(i);
        int lct = 0, nei = (int)nli.size();
        for (int j = 0; j < nei; ++j) {
            std::vector<std::pair<int, double>> nlj = graph.getNeighbors(nli[j].first);
            for (int k = 0; k < j; ++k) {
                auto it = std::find_if(nlj.begin(), nlj.end(), [&](const std::pair<int, double>& obj) {
                    return obj.first == nli[k].first;
                });
                lct += (it != nlj.end());
            }
        }
        if (lct) {
            ret += 1. * (lct << 1) / nei / (nei-1);
        }
    }
    ret /= nodeCount;
    return ret;
}

std::tuple<double, double, double> distance(const Graph& graph) {
    std::vector<std::vector<double>> distances = floyd(graph);
    int count = 0, nodeCount = graph.getNodeCount();
    double min = 1.e+150, max = -1.e+150, mean = 0.;
    for (int i = 0; i < nodeCount; ++i) {
        for (int j = 0; j < nodeCount; ++j) {
            if (i == j || distances[i][j] > 1.e+100) {
                continue;
            }
            min = (min < distances[i][j] ? min : distances[i][j]);
            max = (max > distances[i][j] ? max : distances[i][j]);
            mean += distances[i][j];
            count++;
        }
    }
    mean /= count;
    return {min, mean, max};
}
