#include "graph_generators.h"
#include <vector>
#include <cmath> // std::abs, std::pow, std::sqrt, std::log
#include <algorithm> // std::lower_bound
#include "RNG_MT.h"

void createScaleFreeNetwork(
    Graph &network, const int N, const double gamma, 
    const double c, const unsigned long seed) {

    class MTRand *randNumb = new MTRand((unsigned long)seed);
    network = Graph(N);
    std::vector<double> cdf;

    double alpha = 1. / (gamma - 1.);

    //double i0 = std::abs(alpha - .5) <= 1e-6 || alpha < .5
    //            ? 1.
    //            : std::pow((10. * std::sqrt(2.) * (1. - alpha)),
    //                       (1. / alpha)) *
    //              std::pow(N, 1. - .5 / alpha);

    double sum = 0.0;
    int numEdges = static_cast<int>(c * N / 2);
    for (int i = 0; i < N; ++i) {
        //sum += std::pow(i + i0, -alpha);
        sum += std::pow(i + 1., -alpha);
        cdf.push_back(sum);
    }

    for (int i = 0; i < N; ++i)
        cdf[i] /= sum;

    // Create links
    for (int i = 0; i < numEdges; ++i) {

        bool condition = true;

        while (condition) {
            int source = std::lower_bound(
                cdf.begin(), cdf.end(),
                randNumb->randExc()
            ) - cdf.begin();
            int target = std::lower_bound(
                cdf.begin(), cdf.end(),
                randNumb->randExc()
            ) - cdf.begin();
            
            condition = !network.addEdge(source, target);

            if (condition) {
                continue;
                //if (network[source].has(target)) {
                //    break;
                //}
            }
        }
    }

    //network.buildDegreeVector();
    delete randNumb;
}

//void createErdosRenyiNetwork(
//    Graph &network, const int N, const double c,
//    const unsigned long seed
//) {
//
//    class MTRand *randNumb = new MTRand(seed);
//    network = Graph(N);
//
//    double p = c / (N-1.);
//
//    for (int source = 0; source < N - 1; ++source) {
//        for (int target = source + 1; target < N; ++target) {
//
//            if (randNumb->randExc() < p) {
//                network.addEdge(source, target);
//            }
//        }
//    }
//
//    delete randNumb;
//}

void createErdosRenyiNetwork(
    Graph &network, const int N, const double c,
    const unsigned long seed
) {

    network = Graph(N);
    double p = c / (N - 1.);

    if (p <= 0) return;
    if (p >= 1) {
        for (int i = 0; i < network.size() - 1; ++i) {
            for (int j = i + 1; j < network.size(); ++j) {
                network.addEdge(i, j);
            }
        }
        return;
    }

    class MTRand *randNumb = new MTRand((unsigned long)seed);
    double lp = std::log(1. - p);

    // Nodes in graph are from 0, n - 1 (start with v as the second node index)
    int v = 1;
    int w = -1;
    while (v < N) {

        double lr = std::log(1. - randNumb->randExc());
        w = w + 1 + int(lr/lp);
        while (w >= v && v < N) {
            w = w - v;
            v = v + 1;
        }
        if (v < N) {
            network.addEdge(v, w);
        }
    }

    delete randNumb;
}

void createHyperbolicNetwork(
    Graph &network, const int N,
    const double c1, const double c2,
    const double nu, const double g,
    const 
    const unsigned long seed
) {

}
