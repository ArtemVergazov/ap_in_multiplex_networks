#define _USE_MATH_DEFINES

#include "RNG_MT.h"

#include "graph_generators.h"
#include "hyperbolic.h"

#include <vector>
#include <cmath> // std::abs, std::pow, std::sqrt, std::log
#include <algorithm> // std::lower_bound
#include <cstdlib> // abs

void createScaleFreeNetwork(
    Graph &network, const int N, const double gamma, 
    const double c, class MTRand *randNumb) {

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
            
            network.addEdge(source, target);
        }
    }
}

void createErdosRenyiNetwork(
    Graph &network, const int N, const double c,
    class MTRand *randNumb
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
}

void createHyperbolicNetwork(
    Graph &network, const int N, const double c,
    const double T, const double gamma,
    const std::vector<double> &kappa,
    const std::vector<double> &theta,
    class MTRand *randNumb
) {

    network = Graph(N);

    constexpr double twoPi = 2. * M_PI;
    double mu = sin(M_PI * T) / (twoPi * c * T);
    double invT = 1. / T;

    for (int i = 0; i < N - 1; ++i) {
        for (int j = 0; j < N; ++j) {
            double dTheta = N / twoPi * abs(M_PI - abs(M_PI - abs(theta[i] - theta[j])));
            double threshold = 1. / (1. + pow(dTheta / (mu * kappa[i] * kappa[j]), invT));
            if (randNumb->randExc() < threshold) {
                network.addEdge(i, j);
            }
        }
    }
}

std::pair<std::vector<double>, std::vector<double>> createHyperbolicNetwork(
    Graph &network, const int N, const double c,
    const double T, const double gamma,
    class MTRand *randNumb
) {

    double kmin = calculateKMin(c, gamma);
    double C = calculateC(c, T, gamma);
    double R = calculateR(N, C);

    auto kappa = sampleKappa(N, kmin, gamma, randNumb);
    auto theta = sampleTheta(N, randNumb);
    //auto r = changeVariablesFromS1ToH2(N, kappa, R, kmin);

    //for (int i = 0; i < N; ++i) {
    //    if (r[i] < 0. || theta[i] < 0. || kappa[i] < 0.) {
    //        throw std::runtime_error("Error when generating network: self-edge!\n\n");
    //    }
    //}

    createHyperbolicNetwork(network, N, c, T, gamma, kappa, theta, randNumb);
    return std::make_pair(kappa, theta);
}
