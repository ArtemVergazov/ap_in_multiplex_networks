#pragma once
#include <vector>
#include <ctime> // time, NULL

inline double calculateKMin(double kbar, double gamma) {
    return kbar * (gamma-2) / (gamma-1);
}

double calculateC(double kbar, double T, double gamma);
double calculateR(int N, double C);

std::vector<double> sampleKappa(int N, double kmin, double gamma, class MTRand *randNumb);
std::vector<double> sampleTheta(int N, class MTRand *randNumb);

std::vector<double> sampleConditionalKappa(
    int N, double nu, const std::vector<double> &kappa1,
    double kmin1, double gamma1,
    double kmin2, double gamma2,
    class MTRand *randNumb
);

std::vector<double> sampleConditionalTheta(
    int N, double g,
    const std::vector<double> &theta1,
    class MTRand *randNumb
);

std::vector<double> changeVariablesFromS1ToH2(
    int N,
    const std::vector<double> &kappa,
    double R,
    double kmin
);
