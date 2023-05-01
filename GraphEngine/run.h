#pragma once

void runSF(
    const int N,
    const double cMin,
    const double cMax,
    const int cNum,
    const double gamma,
    const int nRuns
);

void runER(
    const int N,
    const double cMin,
    const double cMax,
    const int cNum,
    const int nRuns
);

void runSFNoCascade(
    const int N,
    const double cMin,
    const double cMax,
    const int cNum,
    const double gamma,
    const int nRuns
);

void runERMonoplex(
    const int N,
    const double cMin,
    const double cMax,
    const int cNum,
    const int nRuns
);
