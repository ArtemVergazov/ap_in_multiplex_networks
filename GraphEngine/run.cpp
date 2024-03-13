#include "json.hpp"
#include "RNG_MT.h"

#include "run.h"
#include "graph_generators.h"
#include "algorithms.h"

#include <iostream>
#include <fstream>
#include <ctime> // time, NULL

using json = nlohmann::json;

#define OUTPUTS

void MonoplexPercolationRunBase::run() const {
    json results;
    json output;
    output["N"] = N_;
    output["nruns"] = nRuns_;
    output["crange"] = { cMin_, cMax_, cNum_ };
    double dc = (cMax_-cMin_) / (cNum_-1);
    unsigned long seed = 1;

    for (int i = 0; i < cNum_; ++i) {

        double c = cMin_ + i * dc;
        results[i]["c"] = c;
        results[i]["runs"] = {};

#ifdef OUTPUTS
        std::cout << "\n\nc = " << c << std::endl;
#endif // OUTPUTS

        for (int runId = 0; runId < nRuns_; ++runId) {

            results[i]["runs"].push_back({});
            results[i]["runs"][runId]["run_id"] = runId;
            results[i]["runs"][runId]["cascade"] = {};

            class MTRand *randNumb = new MTRand((unsigned long int)(seed + seed - 1));
            Graph G;
            createNetwork_(G, c, randNumb);
            delete randNumb;
            ++seed;

            while (true) {

                int cascadeSize = results[i]["runs"][runId]["cascade"].size();
                int t = cascadeSize ?
                    1 + static_cast<int>(results[i]["runs"][runId]["cascade"][cascadeSize - 1]["t"]) :
                    0;
                results[i]["runs"][runId]["cascade"].push_back({ {"t", t} });
                ++cascadeSize;

                auto artPointsGccSize = removeArticulationPointsMonoplex(G);
                const auto &artPoints = artPointsGccSize.first;
                const auto &gccSize = artPointsGccSize.second;

                results[i]["runs"][runId]["cascade"][cascadeSize - 1]["n_ap"] = artPoints.size();
                results[i]["runs"][runId]["cascade"][cascadeSize - 1]["gcc_size"] = gccSize;

                if (!artPoints.size())
                    break;
            }
        }
        output["results"] = results;

        // Write prettified JSON to another file
        std::ofstream o(outputFileName_);
        o << std::setw(4) << output << std::endl;
    }
}

void MonoplexPercolationRunER::createNetwork_(
    Graph &G,
    double c,
    class MTRand *randNumb
) const {
    createErdosRenyiNetwork(G, N_, c, randNumb);
}

void MonoplexPercolationRunHyperbolic::createNetwork_(
    Graph &G,
    double c,
    class MTRand *randNumb
) const {
    createHyperbolicNetwork(G, N_, c, T_, gamma_, randNumb);
}

void MultiplexPercolationRunBase::run() const {
    json results;
    double dc = (cMax_-cMin_) / (cNum_-1);
    unsigned long seed = 1;

    for (int i = 0; i < cNum_; ++i) {

        double c = cMin_ + i * dc;
        results[i]["c"] = c;
        results[i]["runs"] = {};

#ifdef OUTPUTS
        std::cout << "\n\nc = " << c << std::endl;
#endif // OUTPUTS

        for (int runId = 0; runId < nRuns_; ++runId) {

            results[i]["runs"].push_back({});
            results[i]["runs"][runId]["run_id"] = runId;
            results[i]["runs"][runId]["cascade"] = {};

            class MTRand *randNumb1 = new MTRand((unsigned long int)(seed + seed - 1));
            class MTRand *randNumb2 = new MTRand((unsigned long int)(seed + seed));
            Graph G1, G2;
            createNetworks_(G1, G2, c, randNumb1, randNumb2);
            delete randNumb1;
            delete randNumb2;
            ++seed;

            while (true) {

                int cascadeSize = results[i]["runs"][runId]["cascade"].size();
                int t = cascadeSize ?
                    1 + static_cast<int>(results[i]["runs"][runId]["cascade"][cascadeSize - 1]["t"]) :
                    0;
                results[i]["runs"][runId]["cascade"].push_back({ {"t", t} });
                ++cascadeSize;

                auto artPointsGccSize = removeArticulationPoints(G1, G2);
                const auto &artPoints = artPointsGccSize.first;
                const auto &gccSize = artPointsGccSize.second;

                results[i]["runs"][runId]["cascade"][cascadeSize - 1]["n_ap"] = artPoints.size();
                results[i]["runs"][runId]["cascade"][cascadeSize - 1]["gcc_size"] = gccSize;

                if (!artPoints.size())
                    break;
            }
        }
    }

    json output;
    output["N"] = N_;
    output["nruns"] = nRuns_;
    output["crange"] = { cMin_, cMax_, cNum_ };
    output["results"] = results;

    // Write prettified JSON to another file
    std::ofstream o(outputFileName_);
    o << std::setw(4) << output << std::endl;
}

void MultiplexPercolationRunER::createNetworks_(
    Graph &G1,
    Graph &G2,
    double c,
    class MTRand *randNumb1,
    class MTRand *randNumb2
) const {
    createErdosRenyiNetwork(G1, N_, c, randNumb1);
    createErdosRenyiNetwork(G2, N_, c, randNumb2);
}

void MultiplexPercolationRunSF::createNetworks_(
    Graph &G1,
    Graph &G2,
    double c,
    class MTRand *randNumb1,
    class MTRand *randNumb2
) const {
    createScaleFreeNetwork(G1, N_, gamma1_, c, randNumb1);
    createScaleFreeNetwork(G2, N_, gamma2_, c, randNumb2);
}

void MultiplexPercolationRunHyperbolic::createNetworks_(
    Graph &G1,
    Graph &G2,
    double c,
    class MTRand *randNumb1,
    class MTRand *randNumb2
) const {
    const auto &kappaTheta = createHyperbolicNetwork(G1, N_, c, T1_, gamma1_, randNumb1);
    createHyperbolicNetwork(G2, N_, c, c, T2_, gamma1_, gamma2_, kappaTheta.first, kappaTheta.second, nu_, g_, randNumb2);
}

void runMultiplexAngularCorrelationPercolationHyperbolic(
    int N, double c, double nu,
    double gMin, double gMax, int gNum,
    double gamma1, double gamma2,
    double T1, double T2,
    int nRuns,
    std::string outputFileName
) {
    json output;
    output["N"] = N;
    output["nruns"] = nRuns;
    output["grange"] = { gMin, gMax, gNum };
    json results;
    double dg = (gMax - gMin) / (gNum - 1);
    unsigned long seed = 1;

    for (int i = 0; i < gNum; ++i) {

        double g = gMin + i * dg;
        results[i]["g"] = g;
        results[i]["runs"] = {};

#ifdef OUTPUTS
        std::cout << "\n\ng = " << g << std::endl;
#endif // OUTPUTS

        for (int runId = 0; runId < nRuns; ++runId) {

            results[i]["runs"].push_back({});
            results[i]["runs"][runId]["run_id"] = runId;

            class MTRand *randNumb1 = new MTRand((unsigned long int)(seed + seed - 1));
            class MTRand *randNumb2 = new MTRand((unsigned long int)(seed + seed));
            Graph G1, G2;
            const auto &kappaTheta = createHyperbolicNetwork(G1, N, c, T1, gamma1, randNumb1);
            createHyperbolicNetwork(
                G2, N, c, c, T2,
                gamma1, gamma2,
                kappaTheta.first, kappaTheta.second,
                nu, g,
                randNumb2
            );
            delete randNumb1;
            delete randNumb2;
            ++seed;
            
            int gccSize = 0;
            while (true) {

                auto artPointsGccSize = removeArticulationPoints(G1, G2);
                const auto &artPoints = artPointsGccSize.first;
                gccSize = artPointsGccSize.second;
                if (!artPoints.size())
                    break;
            }
            results[i]["runs"][runId]["rgcc_size"] = gccSize;
        }
        output["results"] = results;
        std::ofstream o(outputFileName);
        o << std::setw(4) << output << std::endl;
    }
}

void runFromFile(int nLayers, int nNodes) {

    std::string pathToLayerData("C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\real_data_experiments\\cpp_data");
    std::vector<Graph> G;
    for (int i = 0; i < nLayers; ++i) {
        std::string pathToEdgeData(pathToLayerData + "\\l_" + std::to_string(i + 1) + ".txt");
        std::cout << "Reading from " + pathToEdgeData << std::endl;
        G.emplace_back(nNodes);
        G[i].fromJson(pathToEdgeData);
    }

    while (true) {

        auto artPointsGccSize = removeArticulationPoints(G);
        const auto &artPoints = artPointsGccSize.first;
        const auto &gccSize = artPointsGccSize.second;

        std::cout << "\t\tNumber of articulation points: " << artPoints.size() << std::endl;
        std::cout << "\t\tGCC size: " << gccSize << std::endl;
        std::cout << std::endl;

        if (!artPoints.size())
            break;
    }
}
