#include <fstream>
#include <chrono>
#include <iostream>

#include "json.hpp"
#include "run.h"

using json = nlohmann::json;

int main(int argc, char **argv) {

    auto begin = std::chrono::high_resolution_clock::now();

    constexpr int N = 1000;
    constexpr double cMin = .01;
    constexpr double cMax = 12;
    constexpr int cNum = 200;
    constexpr int nRuns = 1000;
    constexpr double gamma1 = 3;
    constexpr double gamma2 = 3;
    constexpr double T1 = .4;
    constexpr double T2 = .4;
    constexpr double c = 8;
    constexpr double nu = 1;
    constexpr double gMin = 0;
    constexpr double gMax = 1;
    constexpr int gNum = 10;

    std::string project_dir = "C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\";
    std::string outputFileName = project_dir +
        //"\\experiments\\hyperbolic\\correlation_percolation\\results_nu_1.json";
        "\\experiments\\articulation_points_monoplex\\results\\hyperbolic\\results_hyperbolic.json";

    MonoplexPercolationRunBase *monoplexRun = new MonoplexPercolationRunHyperbolic(
        N, cMin, cMax, cNum,
        nRuns, outputFileName,
        gamma1, T1
    );
    monoplexRun->run();
    delete monoplexRun;
    //runMultiplexAngularCorrelationPercolationHyperbolic(
    //    N, c, nu, gMin, gMax, gNum, gamma1, gamma2, T1, T2, nRuns, outputFileName
    //);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "Elapsed time: " << elapsed.count() * 1e-9 << " seconds\n";

    getchar();

}
