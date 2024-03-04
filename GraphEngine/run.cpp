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

void MonoplexPercolationRunER::createNetwork_(
    Graph &G,
    const double c,
    class MTRand *randNumb
) const {
    createErdosRenyiNetwork(G, N_, c, randNumb);
}

void MonoplexPercolationRunHyperbolic::createNetwork_(
    Graph &G,
    const double c,
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
    const double c,
    class MTRand *randNumb1,
    class MTRand *randNumb2
) const {
    createErdosRenyiNetwork(G1, N_, c, randNumb1);
    createErdosRenyiNetwork(G2, N_, c, randNumb2);
}

void MultiplexPercolationRunSF::createNetworks_(
    Graph &G1,
    Graph &G2,
    const double c,
    class MTRand *randNumb1,
    class MTRand *randNumb2
) const {
    createScaleFreeNetwork(G1, N_, gamma1_, c, randNumb1);
    createScaleFreeNetwork(G2, N_, gamma2_, c, randNumb2);
}

void MultiplexPercolationRunHyperbolic::createNetworks_(
    Graph &G1,
    Graph &G2,
    const double c,
    class MTRand *randNumb1,
    class MTRand *randNumb2
) const {
    auto kappaTheta = createHyperbolicNetwork(G1, N_, c, T1_, gamma1_, randNumb1);
    createHyperbolicNetwork(G2, N_, c, T2_, gamma2_, kappaTheta.first, kappaTheta.second, randNumb2);
}

//void runSFNoCascade(
//    const int N,
//    const double cMin,
//    const double cMax,
//    const int cNum,
//    const double gamma,
//    const int nRuns
//) {
//
//    json results;
//    double dc = (cMax - cMin) / (cNum - 1);
//    //unsigned long seed = static_cast<unsigned long>(time(NULL));
//    unsigned long seed = 1;
//
//    for (int i = 0; i < cNum; ++i) {
//
//        double c = cMin + i * dc;
//        results[i]["c"] = c;
//        results[i]["runs"] = {};
//
//#ifdef OUTPUTS
//        std::cout << "\n\nc = " << c << std::endl;
//#endif // OUTPUTS
//
//        for (int runID = 0; runID < nRuns; ++runID) {
//
//            results[i]["runs"].push_back({});
//            //current_run = results[-1]['runs'][-1]
//            results[i]["runs"][runID]["run_id"] = runID;
//            //current_run['cascade'] = []
//            //results[i]["runs"][runID]["cascade"] = {};
//
//            Graph G1, G2;
//            createScaleFreeNetwork(G1, N, gamma, c, seed + seed - 1);
//            createScaleFreeNetwork(G2, N, gamma, c, seed + seed);
//            ++seed;
//
//            //std::cout << "\tNetwork created\n";
//
//            //int cascadeSize = results[i]["runs"][runID]["cascade"].size();
//            //int t = cascadeSize ?
//            //        1 + static_cast<int>(results[i]["runs"][runID]["cascade"][cascadeSize - 1]["t"]) :
//            //        0;
//            //    results[i]["runs"][runID]["cascade"].push_back({ {"t", t} });
//            //    ++cascadeSize;
//
//                //std::cout << "\t\tCascade t = " << t << std::endl;
//
//            auto artPointsGccSize = removeArticulationPoints(G1, G2);
//            const auto &artPoints = artPointsGccSize.first;
//            const auto &gccSize = artPointsGccSize.second;
//
//            results[i]["runs"][runID]["n_ap"] = artPoints.size();
//            results[i]["runs"][runID]["gcc_size"] = gccSize;
//
//            //std::cout << "\t\tNumber of articulation points: " << artPoints.size() << std::endl;
//            //std::cout << "\t\tGCC size: " << gccSize << std::endl;
//        }
//    }
//    //    if save_results :
//    //        with open(f'results_{dt()}.json', 'w') as json_file :
//    //json.dump({
//    //    'N': N,
//    //    'nruns' : nruns,
//    //    'crange' : [crange[0], crange[-1]] ,
//    //    'results' : results,
//    //    }, json_file, indent = 4)
//
//    json output;
//    output["N"] = N;
//    output["nruns"] = nRuns;
//    output["crange"] = { cMin, cMax, cNum };
//    output["results"] = results;
//
//    // Write prettified JSON to another file
//    std::ofstream o("C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\articulation_points_multiplex\\results_lambda_4_no_cascade.json");
//    o << std::setw(4) << output << std::endl;
//
//}

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

//void runJsonTest() {
//
//    Graph G1;
//    createScaleFreeNetwork(G1, 100000, 3, 5, 1);
//    Graph G2;
//    createScaleFreeNetwork(G2, 100000, 3.5, 5, 1);
//    Graph G3;
//    createScaleFreeNetwork(G3, 100000, 2.5, 5, 1);
//    G1.toJson("C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\network_models\\graph_3.json");
//    G2.toJson("C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\network_models\\graph_3.5.json");
//    G3.toJson("C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\network_models\\graph_2.5.json");
//
//}

//void runThreeLayered(
//    const int N,
//    const double cMin,
//    const double cMax,
//    const int cNum,
//    const int nRuns
//) {
//
//    json results;
//    double dc = (cMax - cMin) / (cNum - 1);
//    //unsigned long seed = static_cast<unsigned long>(time(NULL));
//    unsigned long seed = 1;
//
//    for (int i = 0; i < cNum; ++i) {
//
//        double c = cMin + i * dc;
//        results[i]["c"] = c;
//        results[i]["runs"] = {};
//
//#ifdef OUTPUTS
//        std::cout << "\n\nc = " << c << std::endl;
//#endif // OUTPUTS
//
//        for (int runID = 0; runID < nRuns; ++runID) {
//
//            results[i]["runs"].push_back({});
//            //current_run = results[-1]['runs'][-1]
//            results[i]["runs"][runID]["run_id"] = runID;
//            //current_run['cascade'] = []
//            results[i]["runs"][runID]["cascade"] = {};
//
//            std::vector<Graph> G(3);
//            createErdosRenyiNetwork(G[0], N, c, seed + seed + seed - 2);
//            createErdosRenyiNetwork(G[1], N, c, seed + seed + seed - 1);
//            createErdosRenyiNetwork(G[2], N, c, seed + seed + seed);
//            ++seed;
//
//            //std::cout << "\tNetwork created\n";
//            //if (G1 == G2) {
//            //    std::cout << "G1 == G2\n";
//            //}
//
//            while (true) {
//
//                int cascadeSize = results[i]["runs"][runID]["cascade"].size();
//                int t = cascadeSize ?
//                    1 + static_cast<int>(results[i]["runs"][runID]["cascade"][cascadeSize - 1]["t"]) :
//                    0;
//                results[i]["runs"][runID]["cascade"].push_back({ {"t", t} });
//                ++cascadeSize;
//
//                //std::cout << "\t\tCascade t = " << t << std::endl;
//
//                auto artPointsGccSize = removeArticulationPoints(G);
//                const auto &artPoints = artPointsGccSize.first;
//                const auto &gccSize = artPointsGccSize.second;
//
//                results[i]["runs"][runID]["cascade"][cascadeSize - 1]["n_ap"] = artPoints.size();
//                results[i]["runs"][runID]["cascade"][cascadeSize - 1]["gcc_size"] = gccSize;
//
//                //std::cout << "\t\tNumber of articulation points: " << artPoints.size() << std::endl;
//                //std::cout << "\t\tGCC size: " << gccSize << std::endl;
//
//                if (!artPoints.size())
//                    break;
//            }
//        }
//    }
//    //    if save_results :
//    //        with open(f'results_{dt()}.json', 'w') as json_file :
//    //json.dump({
//    //    'N': N,
//    //    'nruns' : nruns,
//    //    'crange' : [crange[0], crange[-1]] ,
//    //    'results' : results,
//    //    }, json_file, indent = 4)
//
//    json output;
//    output["N"] = N;
//    output["nruns"] = nRuns;
//    output["crange"] = { cMin, cMax, cNum };
//    output["results"] = results;
//
//    // Write prettified JSON to another file
//    std::ofstream o("C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\articulation_points_multiplex\\results_ER_three.json");
//    o << std::setw(4) << output << std::endl;
//
//}
