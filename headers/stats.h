//
// Created by Ivor on 2025/12/25.
//

#ifndef GRAPHENGINE_STATS_H
#define GRAPHENGINE_STATS_H

#include "Graph.h"

std::pair<std::pair<double, double>, std::vector<double>> degreeDistribution(Graph graph);
double clusterCoefficient(Graph graph);
std::tuple<double, double, double> distance(const Graph& graph);

#endif // GRAPHENGINE_STATS_H
