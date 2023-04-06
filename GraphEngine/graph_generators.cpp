#include "graph_generators.h"
#include <vector>
#include <cmath> // std::abs, std::pow, std::sqrt
#include <algorithm> // std::lower_bound
#include "RNG_MT.h"

void createScaleFreeNetwork(Graph &network, int N, double gamma, double c, unsigned long seed) {

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
        sum += std::pow(i + 1, -alpha);
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
                if (network[source].has(target)) {
                    break;
                }
            }
        }
    }

    //network.buildDegreeVector();
    delete randNumb;
}
