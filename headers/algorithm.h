//
// Created by Ivor on 2025/12/22.
//

#ifndef GRAPHENGINE_ALGORITHM_H
#define GRAPHENGINE_ALGORITHM_H

#include <functional>
#include <queue>

#include "Graph.h"

bool dfs(Graph graph);
bool bfs(Graph graph, std::pair<int, int> objPair);
std::vector<std::vector<double>> floyd(Graph graph);

#endif // GRAPHENGINE_ALGORITHM_H
