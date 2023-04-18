#include "run.h"

#include <iostream>
#include <fstream>
#include <ctime> // time, NULL
#include "json.hpp"

#include "graph_generators.h"
#include "algorithms.h"

using json = nlohmann::json;

#define OUTPUTS

void runSF(
    const int N,
    const double cMin,
    const double cMax,
    const int cNum,
    const double gamma,
    const int nRuns
) {

    json results;
    double dc = (cMax - cMin) / (cNum - 1);
    //unsigned long seed = static_cast<unsigned long>(time(NULL));
    unsigned long seed = 1;

    for (int i = 0; i < cNum; ++i) {

        double c = cMin + i * dc;
        results[i]["c"] = c;
        results[i]["runs"] = {};

#ifdef OUTPUTS
        std::cout << "\n\nc = " << c << std::endl;
#endif // OUTPUTS
        
        for (int runID = 0; runID < nRuns; ++runID) {

            results[i]["runs"].push_back({});
            //current_run = results[-1]['runs'][-1]
            results[i]["runs"][runID]["run_id"] = runID;
            //current_run['cascade'] = []
            results[i]["runs"][runID]["cascade"] = {};

            Graph G1, G2;
            createScaleFreeNetwork(G1, N, gamma, c, seed + seed - 1);
            createScaleFreeNetwork(G2, N, gamma, c, seed + seed);
            ++seed;

            //std::cout << "\tNetwork created\n";

            while (true) {

                int cascadeSize = results[i]["runs"][runID]["cascade"].size();
                int t = cascadeSize ?
                        1 + static_cast<int>(results[i]["runs"][runID]["cascade"][cascadeSize - 1]["t"]) :
                        0;
                results[i]["runs"][runID]["cascade"].push_back({ {"t", t} });
                ++cascadeSize;

                //std::cout << "\t\tCascade t = " << t << std::endl;

                auto artPointsGccSize = removeArticulationPoints(G1, G2);
                const auto &artPoints = artPointsGccSize.first;
                const auto &gccSize = artPointsGccSize.second;

                results[i]["runs"][runID]["cascade"][cascadeSize - 1]["n_ap"] = artPoints.size();
                results[i]["runs"][runID]["cascade"][cascadeSize - 1]["gcc_size"] = gccSize;

                //std::cout << "\t\tNumber of articulation points: " << artPoints.size() << std::endl;
                //std::cout << "\t\tGCC size: " << gccSize << std::endl;

                if (!artPoints.size())
                    break;
            }
        }
    }
    //    if save_results :
    //        with open(f'results_{dt()}.json', 'w') as json_file :
    //json.dump({
    //    'N': N,
    //    'nruns' : nruns,
    //    'crange' : [crange[0], crange[-1]] ,
    //    'results' : results,
    //    }, json_file, indent = 4)

    json output;
    output["N"] = N;
    output["nruns"] = nRuns;
    output["crange"] = { cMin, cMax, cNum };
    output["results"] = results;

    // Write prettified JSON to another file
    std::ofstream o("C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\articulation_points_multiplex\\results.json");
    o << std::setw(4) << output << std::endl;

}

void runER(
    const int N,
    const double cMin,
    const double cMax,
    const int cNum,
    const int nRuns
) {

    json results;
    double dc = (cMax - cMin) / (cNum - 1);
    //unsigned long seed = static_cast<unsigned long>(time(NULL));
    unsigned long seed = 1;

    for (int i = 0; i < cNum; ++i) {

        double c = cMin + i * dc;
        results[i]["c"] = c;
        results[i]["runs"] = {};

#ifdef OUTPUTS
        std::cout << "\n\nc = " << c << std::endl;
#endif // OUTPUTS

        for (int runID = 0; runID < nRuns; ++runID) {

            results[i]["runs"].push_back({});
            //current_run = results[-1]['runs'][-1]
            results[i]["runs"][runID]["run_id"] = runID;
            //current_run['cascade'] = []
            results[i]["runs"][runID]["cascade"] = {};

            Graph G1, G2;
            createErdosRenyiNetwork(G1, N, c, seed + seed - 1);
            createErdosRenyiNetwork(G2, N, c, seed + seed);
            ++seed;

            //std::cout << "\tNetwork created\n";
            //if (G1 == G2) {
            //    std::cout << "G1 == G2\n";
            //}

            while (true) {

                int cascadeSize = results[i]["runs"][runID]["cascade"].size();
                int t = cascadeSize ?
                    1 + static_cast<int>(results[i]["runs"][runID]["cascade"][cascadeSize - 1]["t"]) :
                    0;
                results[i]["runs"][runID]["cascade"].push_back({ {"t", t} });
                ++cascadeSize;

                //std::cout << "\t\tCascade t = " << t << std::endl;

                auto artPointsGccSize = removeArticulationPoints(G1, G2);
                const auto &artPoints = artPointsGccSize.first;
                const auto &gccSize = artPointsGccSize.second;

                results[i]["runs"][runID]["cascade"][cascadeSize - 1]["n_ap"] = artPoints.size();
                results[i]["runs"][runID]["cascade"][cascadeSize - 1]["gcc_size"] = gccSize;

                //std::cout << "\t\tNumber of articulation points: " << artPoints.size() << std::endl;
                //std::cout << "\t\tGCC size: " << gccSize << std::endl;

                if (!artPoints.size())
                    break;
            }
        }
    }
    //    if save_results :
    //        with open(f'results_{dt()}.json', 'w') as json_file :
    //json.dump({
    //    'N': N,
    //    'nruns' : nruns,
    //    'crange' : [crange[0], crange[-1]] ,
    //    'results' : results,
    //    }, json_file, indent = 4)

    json output;
    output["N"] = N;
    output["nruns"] = nRuns;
    output["crange"] = { cMin, cMax, cNum };
    output["results"] = results;

    // Write prettified JSON to another file
    std::ofstream o("C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\articulation_points_multiplex\\results.json");
    o << std::setw(4) << output << std::endl;

}

