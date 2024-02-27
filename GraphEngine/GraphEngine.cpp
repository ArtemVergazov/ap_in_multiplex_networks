#include <fstream>
#include <chrono>
#include <iostream>

#include "json.hpp"
#include "run.h"

using json = nlohmann::json;

void test();

int main(int argc, char **argv) {

    auto begin = std::chrono::high_resolution_clock::now();

    constexpr int N = 100000;
    constexpr double cMin = 0;
    constexpr double cMax = 20;
    constexpr int cNum = 200;
    constexpr int nRuns = 10;
    constexpr double gamma1 = 2.5;
    constexpr double gamma2 = 2.5;

    runSF(N, cMin, cMax, cNum, gamma1, gamma2, nRuns);
    //runER(N, cMin, cMax, cNum, nRuns);
    //runSFNoCascade(N, cMin, cMax, cNum, gamma, nRuns);
    //runERMonoplex(N, cMin, cMax, cNum, nRuns);
    //test();
    //runJsonTest();
    //runFromFile(2, 4458);
    //runThreeLayered(N, cMin, cMax, cNum, nRuns);

    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
    std::cout << "Elapsed time: " << elapsed.count() * 1e-9 << " seconds\n";

    getchar();

}
