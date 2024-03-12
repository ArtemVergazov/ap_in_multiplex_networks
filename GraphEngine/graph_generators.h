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
    int N, double gamma, double c,
    class MTRand *randNumb
);

void createErdosRenyiNetwork(
    Graph &network, int N, double c,
    class MTRand *randNumb
);

// Create a network from scratch and return kappa and theta created
std::pair<std::vector<double>, std::vector<double>> createHyperbolicNetwork(
    Graph &network, int N, double c,
    double T, double gamma,
    class MTRand *randNumb
);

// Condition on previously created kappa and theta
void createHyperbolicNetwork(
    Graph &network, int N,
    double c1, double c2,
    double T2,
    double gamma1, double gamma2,
    const std::vector<double> &kappa1,
    const std::vector<double> &theta1,
    double nu, double g,
    class MTRand *randNumb
);
