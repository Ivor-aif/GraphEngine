//
// Created by Ivor on 2025/12/22.
//

#ifndef GRAPHENGINE_ALGORITHM_H
#define GRAPHENGINE_ALGORITHM_H

#include <functional>
#include <queue>

#include "Graph.h"

bool dfs(const Graph& graph);
bool bfs(const Graph& graph, const std::pair<int, int>& objPair);
std::vector<std::vector<double>> floyd(const Graph& graph);

#endif // GRAPHENGINE_ALGORITHM_H
