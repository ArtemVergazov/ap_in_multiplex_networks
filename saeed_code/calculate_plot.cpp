#include "json.hpp"
#include "NetworkModels.h"
#include "AP.h"

using json = nlohmann::json;
/*
void runSF(lint N, double cMin, double cMax, int cNum, double gamma, int nRuns) {

    json results;
    double dc = (cMax - cMin) / (cNum - 1);
    ulint seed = ONE;

    for (int i = 0; i <= cNum; ++i) {

        double c = cMin + i * dc;
        results[i]["c"] = c;
        results[i]["runs"] = {};
        
        for (int runID = 0; runID < nRuns; ++runID) {

            results[i]["runs"].push_back({});
            //current_run = results[-1]['runs'][-1]
            results[i]["runs"][runID]["run_id"] = runID;
            //current_run['cascade'] = []
            results[i]["runs"][runID]["cascade"] = {};

            NETWORK G1, G2;
            CreateStaticNetwork(G1, N, gamma, c, seed + seed - ONE);
            CreateStaticNetwork(G2, N, gamma, c, seed + seed);
            ++seed;

            while (true) {

                int cascadeSize = results[i]["runs"][runID]["cascade"].size();
                int t = cascadeSize ?
                        static_cast<int>(results[i]["runs"][runID]["cascade"][cascadeSize - 1]["t"]) :
                        0;
                results[i]["runs"][runID]["cascade"].push_back({ {"t", t} });

                G1, G2, art_points, gcc_size = remove_ap(G1, G2);
                current_run['cascade'][-1]['n_ap'] = len(art_points);
                current_run['cascade'][-1]['gcc_size'] = gcc_size;

                if (not art_points)
                    break;
            }
        }
    }
        if save_results :
            with open(f'results_{dt()}.json', 'w') as json_file :
    json.dump({
        'N': N,
        'nruns' : nruns,
        'crange' : [crange[0], crange[-1]] ,
        'results' : results,
        }, json_file, indent = 4)

}
*/