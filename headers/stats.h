//
// Created by Ivor on 2025/12/25.
//

#ifndef GRAPHENGINE_STATS_H
#define GRAPHENGINE_STATS_H

#include "Graph.h"

std::pair<std::pair<double, double>, std::vector<double>> degreeDistribution(const Graph& graph);
double clusterCoefficient(const Graph& graph);
std::tuple<double, double, double> distance(const Graph& graph);

#endif // GRAPHENGINE_STATS_H
