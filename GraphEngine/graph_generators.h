#pragma once
#include <ctime> // time, NULL
#include "graph.h"

//void CreatePoissonNetwork(NETWORK &network, const lint N, const double meanDegree, const ulint seed);

//void CreateConfigurationModelPoissonNetwork(NETWORK &network, const lint N, const double meanDegree, const ulint seed);

//void CreateStaticNetwork(NETWORK &network, const lint N, const double gamma, const double c, const ulint seed);

void createScaleFreeNetwork(
    Graph &network,
    const int N, const double gamma, const double c,
    const unsigned long seed = static_cast<unsigned long>(time(NULL))
);

void createErdosRenyiNetwork(
    Graph &network, const int N, const double c,
    const unsigned long seed = static_cast<unsigned long>(time(NULL))
);

void createHyperbolicNetwork(
    Graph &network, const int N, const double c,
    const unsigned long seed = static_cast<unsigned long>(time(NULL))
);
