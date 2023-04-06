#include <fstream>
#include <string>

#include "json.hpp"
#include "run.h"

using json = nlohmann::json;

void test();

int main(int argc, char **argv) {

    runSF(1000000, 0, 12, 100, 2.5, 100);
    //test();

}
