#pragma once
#include "RNG_MT.h"
#include "graph.h"
#include <string>

class MonoplexPercolationRunBase {
public:
    MonoplexPercolationRunBase(
        int N,
        double cMin,
        double cMax,
        int cNum,
        int nRuns,
        std::string outputFileName
    ) : N_(N), cMin_(cMin), cMax_(cMax), cNum_(cNum), nRuns_(nRuns), outputFileName_(outputFileName) {}

    void run() const;
    virtual ~MonoplexPercolationRunBase() {}

protected:
    const int N_;
    const double cMin_;
    const double cMax_;
    const int cNum_;
    const int nRuns_;
    const std::string outputFileName_;

private:
    virtual void createNetwork_(
        Graph &G,
        double c,
        class MTRand *randNumb
    ) const = 0;
};

class MonoplexPercolationRunER : public MonoplexPercolationRunBase {
private:
    void createNetwork_(
        Graph &G,
        double c,
        class MTRand *randNumb
    ) const override;
};

class MonoplexPercolationRunHyperbolic : public MonoplexPercolationRunBase {
public:
    MonoplexPercolationRunHyperbolic(
        int N,
        double cMin,
        double cMax,
        int cNum,
        int nRuns,
        std::string outputFileName,
        double gamma,
        double T
    ) :
        MonoplexPercolationRunBase(N, cMin, cMax, cNum, nRuns, outputFileName),
        gamma_(gamma),
        T_(T) {}

private:
    void createNetwork_(
        Graph &G,
        double c,
        class MTRand *randNumb
    ) const override;

    const double gamma_;
    const double T_;
};

class MultiplexPercolationRunBase {
public:
    MultiplexPercolationRunBase(
        int N,
        double cMin,
        double cMax,
        int cNum,
        int nRuns,
        std::string outputFileName
    ) : N_(N), cMin_(cMin), cMax_(cMax), cNum_(cNum), nRuns_(nRuns), outputFileName_(outputFileName) {}

    void run() const;
    virtual ~MultiplexPercolationRunBase() {}

protected:
    const int N_;
    const double cMin_;
    const double cMax_;
    const int cNum_;
    const int nRuns_;
    const std::string outputFileName_;

private:
    virtual void createNetworks_(
        Graph &G1,
        Graph &G2,
        double c,
        class MTRand *randNumb1,
        class MTRand *randNumb2
    ) const = 0;
};

class MultiplexPercolationRunER : public MultiplexPercolationRunBase {
private:
    void createNetworks_(
        Graph &G1,
        Graph &G2,
        double c,
        class MTRand *randNumb1,
        class MTRand *randNumb2
    ) const override;
};

class MultiplexPercolationRunSF : public MultiplexPercolationRunBase {
public:
    MultiplexPercolationRunSF(
        int N,
        double cMin,
        double cMax,
        int cNum,
        int nRuns,
        std::string outputFileName,
        double gamma1,
        double gamma2
    ) :
        MultiplexPercolationRunBase(N, cMin, cMax, cNum, nRuns, outputFileName),
        gamma1_(gamma1),
        gamma2_(gamma2) {}
private:
    void createNetworks_(
        Graph &G1,
        Graph &G2,
        double c,
        class MTRand *randNumb1,
        class MTRand *randNumb2
    ) const override;

    const double gamma1_;
    const double gamma2_;
};

class MultiplexPercolationRunHyperbolic : public MultiplexPercolationRunBase {
public:
    MultiplexPercolationRunHyperbolic(
        int N,
        double cMin, double cMax, int cNum,
        int nRuns,
        std::string outputFileName,
        double gamma1, double gamma2,
        double T1, double T2,
        double nu, double angularCorrelation
    ) :
        MultiplexPercolationRunBase(N, cMin, cMax, cNum, nRuns, outputFileName),
        gamma1_(gamma1),
        gamma2_(gamma2),
        T1_(T1),
        T2_(T2),
        nu_(nu),
        angularCorrelation_(angularCorrelation) {}
private:
    void createNetworks_(
        Graph &G1,
        Graph &G2,
        double c,
        class MTRand *randNumb1,
        class MTRand *randNumb2
    ) const override;

    const double gamma1_;
    const double gamma2_;
    const double T1_;
    const double T2_;
    const double nu_;
    const double angularCorrelation_;
};

void runMultiplexAngularCorrelationPercolationHyperbolic(
    int N, double c, double nu,
    const std::vector<double> &angularCorrelationVec,
    double gamma1, double gamma2,
    double T1, double T2,
    int nRuns,
    std::string outputFileName
);

void runMultiplexAngularCorrelationPercolationHyperbolic(
    int N, double c, double nu,
    double angularCorrelationMin, double angularCorrelationMax, int angularCorrelationNum,
    double gamma1, double gamma2,
    double T1, double T2,
    int nRuns,
    std::string outputFileName
);

void seriesOfMultiplexHyperbolicRuns(
    int N, double nu,
    double cMin, double cMax, int cNum,
    double angularCorrelationMin, double angularCorrelationMax, int angularCorrelationNum,
    double gamma1, double gamma2,
    double T1, double T2,
    int nRuns,
    std::string outputFileNameTemplate,
    const std::vector<std::string> &angularCorrelationsLabels
);

void runSFNoCascade(
    int N,
    double cMin,
    double cMax,
    int cNum,
    double gamma,
    int nRuns
);

void runFromFile(int nLayers, int nNodes);

void runJsonTest();

void runThreeLayered(
    int N,
    double cMin,
    double cMax,
    int cNum,
    int nRuns
);
