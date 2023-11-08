#pragma once
#include "graph.h"
#include <vector>
#include <set>
#include <utility> // std::pair

std::vector<std::set<int>> connectedComponents(const Graph &G);

std::vector<int> articulationPoints(const Graph &G);

std::pair<std::set<int>, int> removeArticulationPoints(Graph &G1, Graph &G2);

std::pair<std::set<int>, int> removeArticulationPoints(std::vector<Graph> &G);

std::pair<std::set<int>, int> removeArticulationPointsMonoplex(Graph &G);
