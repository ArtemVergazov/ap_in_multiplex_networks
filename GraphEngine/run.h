#pragma once
#include "RNG_MT.h"
#include "graph.h"
#include <string>

class MonoplexPercolationRunBase {
public:
    MonoplexPercolationRunBase(
        const int N,
        const double cMin,
        const double cMax,
        const int cNum,
        const int nRuns,
        std::string outputFileName
    ) : N_(N), cMin_(cMin), cMax_(cMax), cNum_(cNum), nRuns_(nRuns), outputFileName_(outputFileName) {}

    void run() const;
    virtual ~MonoplexPercolationRunBase() {}

protected:
    int N_;
    double cMin_;
    double cMax_;
    int cNum_;
    int nRuns_;
    std::string outputFileName_;

private:
    virtual void createNetwork_(
        Graph &G,
        const double c,
        class MTRand *randNumb
    ) const = 0;
};

class MonoplexPercolationRunER : public MonoplexPercolationRunBase {
private:
    void createNetwork_(
        Graph &G,
        const double c,
        class MTRand *randNumb
    ) const override;
};

class MonoplexPercolationRunHyperbolic : public MonoplexPercolationRunBase {
public:
    MonoplexPercolationRunHyperbolic(
        const int N,
        const double cMin,
        const double cMax,
        const int cNum,
        const int nRuns,
        std::string outputFileName,
        const double gamma,
        const double T
    ) :
        MonoplexPercolationRunBase(N, cMin, cMax, cNum, nRuns, outputFileName),
        gamma_(gamma),
        T_(T) {}

private:
    void createNetwork_(
        Graph &G,
        const double c,
        class MTRand *randNumb
    ) const override;

    double gamma_;
    double T_;
};

class MultiplexPercolationRunBase {
public:
    MultiplexPercolationRunBase(
        const int N,
        const double cMin,
        const double cMax,
        const int cNum,
        const int nRuns,
        std::string outputFileName
    ) : N_(N), cMin_(cMin), cMax_(cMax), cNum_(cNum), nRuns_(nRuns), outputFileName_(outputFileName) {}

    void run() const;
    virtual ~MultiplexPercolationRunBase() {}

protected:
    int N_;
    double cMin_;
    double cMax_;
    int cNum_;
    int nRuns_;
    std::string outputFileName_;

private:
    virtual void createNetworks_(
        Graph &G1,
        Graph &G2,
        const double c,
        class MTRand *randNumb1,
        class MTRand *randNumb2
    ) const = 0;
};

class MultiplexPercolationRunER : public MultiplexPercolationRunBase {
private:
    void createNetworks_(
        Graph &G1,
        Graph &G2,
        const double c,
        class MTRand *randNumb1,
        class MTRand *randNumb2
    ) const override;
};

class MultiplexPercolationRunSF : public MultiplexPercolationRunBase {
public:
    MultiplexPercolationRunSF(
        const int N,
        const double cMin,
        const double cMax,
        const int cNum,
        const int nRuns,
        std::string outputFileName,
        const double gamma1,
        const double gamma2
    ) :
        MultiplexPercolationRunBase(N, cMin, cMax, cNum, nRuns, outputFileName),
        gamma1_(gamma1),
        gamma2_(gamma2) {}
private:
    void createNetworks_(
        Graph &G1,
        Graph &G2,
        const double c,
        class MTRand *randNumb1,
        class MTRand *randNumb2
    ) const override;

    double gamma1_;
    double gamma2_;
};

class MultiplexPercolationRunHyperbolic : public MultiplexPercolationRunBase {
public:
    MultiplexPercolationRunHyperbolic(
        const int N,
        const double cMin,
        const double cMax,
        const int cNum,
        const int nRuns,
        std::string outputFileName,
        const double gamma1,
        const double gamma2,
        const double T1,
        const double T2
    ) :
        MultiplexPercolationRunBase(N, cMin, cMax, cNum, nRuns, outputFileName),
        gamma1_(gamma1),
        gamma2_(gamma2),
        T1_(T1),
        T2_(T2) {}
private:
    void createNetworks_(
        Graph &G1,
        Graph &G2,
        const double c,
        class MTRand *randNumb1,
        class MTRand *randNumb2
    ) const override;

    double gamma1_;
    double gamma2_;
    double T1_;
    double T2_;
};

void runSFNoCascade(
    const int N,
    const double cMin,
    const double cMax,
    const int cNum,
    const double gamma,
    const int nRuns
);

void runFromFile(int nLayers, int nNodes);

void runJsonTest();

void runThreeLayered(
    const int N,
    const double cMin,
    const double cMax,
    const int cNum,
    const int nRuns
);
