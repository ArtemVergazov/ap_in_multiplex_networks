#include <fstream>
#include <string>
#include <chrono>
#include <iostream>

#include "json.hpp"
#include "run.h"

using json = nlohmann::json;

void test();

int main(int argc, char **argv) {

    auto begin = std::chrono::high_resolution_clock::now();

    constexpr int N = 1000000;
    constexpr double cMin = 0;
    constexpr double cMax = 12;
    constexpr int cNum = 100;
    constexpr double gamma = 4;
    constexpr int nRuns = 50;

    //runSF(N, cMin, cMax, cNum, gamma, nRuns);
    runER(N, cMin, cMax, cNum, nRuns);
    //test();

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

    std::cout << "Elapsed time: " << elapsed.count() * 1e-9 << " seconds\n";

}
