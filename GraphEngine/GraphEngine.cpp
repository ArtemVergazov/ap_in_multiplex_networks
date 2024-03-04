#include <fstream>
#include <chrono>
#include <iostream>

#include "json.hpp"
#include "run.h"

using json = nlohmann::json;

int main(int argc, char **argv) {

    auto begin = std::chrono::high_resolution_clock::now();

    constexpr int N = 50000;
    constexpr double cMin = 0;
    constexpr double cMax = 12;
    constexpr int cNum = 200;
    constexpr int nRuns = 10;
    constexpr double gamma1 = 3.;
    constexpr double gamma2 = 3.;
    constexpr double T1 = .4;
    constexpr double T2 = .4;

    std::string outputFileName = "C:\\Users\\kuzne\\Documents\\ACTIVE_PROJECTS\\research\\experiments\\results.json";

    MonoplexPercolationRunBase *monoplexRun = new MonoplexPercolationRunHyperbolic(
        N, cMin, cMax, cNum,
        nRuns, outputFileName,
        gamma1, T1
    );
    monoplexRun->run();
    delete monoplexRun;

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "Elapsed time: " << elapsed.count() * 1e-9 << " seconds\n";

    getchar();

}
