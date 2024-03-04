#pragma once
#include "graph.h"
#include <ctime> // time, NULL
#include <utility> // std::pair
#include <vector>
//#include <vector>

//void CreatePoissonNetwork(NETWORK &network, const lint N, const double meanDegree, const ulint seed);

//void CreateConfigurationModelPoissonNetwork(NETWORK &network, const lint N, const double meanDegree, const ulint seed);

//void CreateStaticNetwork(NETWORK &network, const lint N, const double gamma, const double c, const ulint seed);

void createScaleFreeNetwork(
    Graph &network,
    const int N, const double gamma, const double c,
    class MTRand *randNumb
);

void createErdosRenyiNetwork(
    Graph &network, const int N, const double c,
    class MTRand *randNumb
);

// Condition on previously created kappa and theta
void createHyperbolicNetwork(
    Graph &network, const int N, const double c,
    const double T, const double gamma,
    const std::vector<double> &kappa1,
    const std::vector<double> &theta1,
    class MTRand *randNumb
);

// Create a network from scratch and return kappa and theta created
std::pair<std::vector<double>, std::vector<double>> createHyperbolicNetwork(
    Graph &network, const int N, const double c,
    const double T, const double gamma,
    class MTRand *randNumb
);
