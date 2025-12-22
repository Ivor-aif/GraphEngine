//
// Created by Ivor on 2025/12/22.
//

#include "../headers/algorithm.h"

bool dfs(Graph graph) {
    if (!graph.getNodeCount()) {
        std::cerr << "Cannot search for an empty graph." << std::endl;
        return false;
    }
    std::vector<bool> visit(graph.getNodeCount(), false);
    std::function<void(int)> dfsLambda = [&](int index) {
        visit[index] = true;
        for (std::pair<int, double> neighbor: graph.getNeighbors(index)) {
            if (!visit[neighbor.first]) {
                dfsLambda(neighbor.first);
            }
        }
    };
    dfsLambda(0);
    return std::all_of(visit.begin(), visit.end(), [](bool isTrue) {return isTrue;});
}

bool bfs(Graph graph, std::pair<int, int> objPair) {
    if (!graph.getNodeCount()) {
        std::cerr << "Cannot search for an empty graph." << std::endl;
        return false;
    }
    if (objPair.first >= graph.getNodeCount()) {
        std::cerr << "The beginning point " << objPair.first << " is not in this graph with " << graph.getNodeCount() << " node(s)." << std::endl;
        return false;
    }
    if (objPair.second >= graph.getNodeCount()) {
        std::cerr << "The ending point " << objPair.second << " is not in this graph with " << graph.getNodeCount() << " node(s)." << std::endl;
        return false;
    }
    std::vector<bool> visit(graph.getNodeCount(), false);
    visit[objPair.first] = true;
    std::queue<int> remain;
    remain.push(objPair.first);
    while (!remain.empty()) {
        for (std::pair<int, double> neighbor: graph.getNeighbors(remain.front())) {
            if (visit[neighbor.first]) {
                continue;
            }
            if (neighbor.first == objPair.second) {
                return true;
            }
            visit[neighbor.first] = true;
            remain.push(neighbor.first);
        }
        remain.pop();
    }
    return false;
}

std::vector<std::vector<double>> floyd(Graph graph) {
    if (!graph.getNodeCount()) {
        std::cerr << "Cannot apply on an empty graph." << std::endl;
        return std::vector<std::vector<double>>(0);
    }
    int nc = graph.getNodeCount();
    double inf = 1.e150; // DOUBLE_MAX ~ pow(2, 1024) ~ 1.e308, here `inf` * 2. will not exceed it.
    std::vector<std::vector<double>> distMat(nc, std::vector<double>(nc, inf));
    for (int i = 0; i < nc; ++i) {
        for (std::pair<int, double> neighbor: graph.getNeighbors(i)) {
            distMat[i][neighbor.first] = neighbor.second;
        }
        distMat[i][i] = 0.;
    }
    for (int k = 0; k < nc; ++k) {
        for (int i = 0; i < nc; ++i) {
            for (int j = 0; j < nc; ++j) {
                if (distMat[i][k] + distMat[k][j] < distMat[i][j]) {
                    distMat[i][j] = distMat[i][k] + distMat[k][j];
                }
            }
        }
    }
    return distMat;
}
