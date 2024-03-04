#define _USE_MATH_DEFINES

#include "RNG_MT.h"
#include "gsl/gsl_sf_lambert.h"
#include "boost/math/special_functions/erf.hpp" // boost::math::erf_inv

#include "hyperbolic.h"

#include <cmath> // sin, pow, log, exp, std::erf, std::fmod
#include <algorithm> // std::min

using boost::math::erf_inv;

double calculateC(double kbar, double T, double gamma) {
    return kbar * sin(M_PI * T) / (2. * T) * pow(((gamma - 2.) / (gamma - 1.)), 2.);
}

double calculateR(int N, double C) {
    return 2. * log(N / C);
}

std::vector<double> sampleKappa(int N, double kmin, double gamma, MTRand *randNumb) {

    std::vector<double> kappa(N);
    for (auto &v : kappa) {
        v = kmin * pow(1 - randNumb->randExc(), 1. / (1.-gamma));
    }

    return kappa;
}

std::vector<double> sampleTheta(int N, MTRand *randNumb) {

    std::vector<double> theta(N);
    for (auto &v : theta) {
        v = 2. * M_PI * randNumb->randExc();
    }

    return theta;
}

std::vector<double> sampleConditionalKappa(
    int N, double nu, const std::vector<double> &kappa1,
    double kmin1, double gamma1,
    double kmin2, double gamma2,
    MTRand *randNumb
) {
    std::vector<double> kappa2(N);

    if (nu == 1) {
        for (int i = 0; i < N; ++i) {
            kappa2[i] = kmin2 * pow(kappa1[i] / kmin1, (1.-gamma1) / (1.-gamma2));
        }
    } else if (nu == 0) {
        kappa2 = sampleKappa(N, kmin2, gamma2, randNumb);
    } else {
        for (int i = 0; i < N; ++i) {
            double phi = -log(1. - pow(kmin1 / kappa1[i], gamma1 - 1.));
            double z = 1. / kmin1 * pow(phi, nu / (nu-1.)) * pow(kappa1[i], -gamma1);
            z *= kmin1*pow(kappa1[i], gamma1) - pow(kmin1, gamma1)*kappa1[i];
            double zr = z * randNumb->randExc();
            zr = nu / (1.-nu) * gsl_sf_lambert_W0(pow(zr, (nu-1.) / nu) / (nu/(1.-nu)));
            zr = pow(zr, 1. / (1.-nu)) - pow(phi, 1. / (1.-nu));
            zr = exp(-pow(zr, 1. - nu));
            zr = kmin2 * pow(1. - zr, 1. / (1.-gamma2));
            kappa2[i] = zr;
        }
    }

    return kappa2;
}

std::vector<double> sampleConditionalTheta(
    int N, double g,
    const std::vector<double> &theta1,
    MTRand *randNumb
) {
    std::vector<double> theta2(N);

    if (g == 1) {
        theta2 = theta1;
    } else if (g == 0) {
        theta2 = sampleTheta(N, randNumb);
    } else {
        constexpr double twoPi = 2 * M_PI;
        double sigma0 = std::min(N / (4.*M_PI), 100.);
        double sigma = sigma0 * (1./g-1.);
        for (int i = 0; i < N; ++i) {
            double l = sqrt(2.) * sigma * erf_inv((-1.+2.*randNumb->randExc()) * std::erf(N / (2*sqrt(2)*sigma)));
            theta2[i] = std::fmod(theta1[i] + twoPi*l/N, twoPi);
        }
    }

    return theta2;
}

std::vector<double> changeVariablesFromS1ToH2(int N, const std::vector<double> &kappa, double R, double kmin) {
    std::vector<double> r(N);
    for (int i = 0; i < N; ++i) {
        r[i] = std::max(R - 2*log(kappa[i] / kmin), 0.);
    }
    return r;
}
