//
// Created by Ivor on 2025/12/24.
//

#ifndef GRAPHENGINE_GENERATOR_H
#define GRAPHENGINE_GENERATOR_H

#include <random>

#include "Graph.h"

static std::mt19937 gen; // Seed can be set by user.

Graph fullConnect(int nodeCount);
Graph lattice(int size, int dim = 2);
Graph ring(int nodeCount, int degree = 4);
Graph oddRegular(int nodeCount, int degree = 3);
Graph erRandom(int nodeCount, int averageDegree, double connectRate, bool isRegular = false);
void wsSmallWorld(Graph& graph, double reconnectRate);
Graph baScaleFree(int nodeCount, int averageDegree);

#endif // GRAPHENGINE_GENERATOR_H
