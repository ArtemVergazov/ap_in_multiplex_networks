#include "json.hpp"
#include "run.h"

#include <fstream>
#include <chrono>
#include <iostream>
#include <vector>

using json = nlohmann::json;

int main(int argc, char **argv) {

    auto begin = std::chrono::high_resolution_clock::now();

    constexpr int N = 50000;

    constexpr double cMin = .01;
    constexpr double cMax = 12;
    constexpr int cNum = 200;

    constexpr int nRuns = 10;

    constexpr double gamma1 = 3;
    constexpr double gamma2 = 3;

    constexpr double T1 = .4;
    constexpr double T2 = .4;

    //constexpr double c = 8;

    constexpr double nu = 1;
    constexpr double g = 1;

    //constexpr double angularCorrelationMin = .2;
    //constexpr double angularCorrelationMax = 1;
    //constexpr int angularCorrelationNum = 5;

    //constexpr double nuMin = 0;
    //constexpr double nuMax = 1;
    //constexpr int nuNum = 11;

    //const std::vector<double> angularCorrelationVec{ 0, .1, .2, .3, .4, .5, .6, .7, .8, .9, .925, .95, .975, 1 };

    //const std::string project_dir = "C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\";
    const std::string project_dir = "C:\\Users\\kuzne\\OneDrive\\Desktop\\projects\\research";
    std::string outputFileName = project_dir +
        //"\\experiments\\hyperbolic\\correlation_percolation\\results_nu_1.json";
        //"\\experiments\\articulation_points_multiplex\\results\\different_correlations\\results_nu_0_g_1.json";
        "\\experiments\\articulation_points_multiplex\\results\\hyperbolic_50000\\results_hyperbolic_50000.json";

    //const std::string outputFileNameTemplate = project_dir +
        //"\\experiments\\articulation_points_multiplex\\results\\different_correlations\\results_nu_0.8_g_";

    //const std::vector<std::string> angularCorrelationLabels{ "0", "0.2", "0.4", "0.6", "0.8", "1" };

    //MonoplexPercolationRunBase *run = new MonoplexPercolationRunHyperbolic(
    //    N, cMin, cMax, cNum,
    //    nRuns, outputFileName,
    //    gamma1, T1
    //);

    MultiplexPercolationRunBase *run = new MultiplexPercolationRunHyperbolic(
        N, cMin, cMax, cNum,
        nRuns, outputFileName,
        gamma1, gamma2,
        T1, T2,
        nu, g
    );

    run->run();
    delete run;

    //runMultiplexAngularCorrelationPercolationHyperbolic(
    //    N, c, nu,
    //    angularCorrelationVec,
    //    gamma1, gamma2,
    //    T1, T2,
    //    nRuns,
    //    outputFileName
    //);

    //runMultiplexRadialCorrelationPercolationHyperbolic(
    //    N, c, g,
    //    nuMin, nuMax, nuNum,
    //    gamma1, gamma2,
    //    T1, T2,
    //    nRuns,
    //    outputFileName
    //);

    //seriesOfMultiplexHyperbolicRuns(
    //    N, nu,
    //    cMin, cMax, cNum,
    //    angularCorrelationMin, angularCorrelationMax, angularCorrelationNum,
    //    gamma1, gamma2,
    //    T1, T2,
    //    nRuns,
    //    outputFileNameTemplate,
    //    angularCorrelationLabels
    //);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "Elapsed time: " << elapsed.count() * 1e-9 << " seconds\n";

    getchar();
}
