#include <iostream>
#include "graph_generators.h"
#include "algorithms.h"

void cascadeRemoveLinks(Graph &Ga, Graph &Gb);
std::set<int> articulationPointsMultiplex(const Graph &G1, const Graph &G2);

void test() {

    constexpr int N = 100000;
    constexpr double c = 2.78;
    constexpr double gamma = 3;
    constexpr int nRuns = 50;

    Graph G1, G2;

    createScaleFreeNetwork(G1, N, gamma, c);
    createScaleFreeNetwork(G2, N, gamma, c, 3452452);

    //auto artPointsGccSize = removeArticulationPoints(G1, G2);
    cascadeRemoveLinks(G1, G2);
    //auto artPoints = articulationPointsMultiplex(G1, G2);
    //auto ap1 = articulationPoints(G1);
    /*auto ap2 = articulationPoints(G2);*/

    std::vector<int> ap(G1.size());
    std::vector<int> low(G1.size());
    std::vector<int> disc(G1.size());

}
