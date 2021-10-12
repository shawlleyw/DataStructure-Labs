#ifndef DS_UTILS_C_
#define DS_UTILS_C_

#include "utils.hh"


// ! class RandomGenerator {

Utils::RandomGenerator::RandomGenerator() {
    srand(time(NULL));
}

int Utils::RandomGenerator::RangeInt(int base, int mod) {
    return base + rand() % mod;
}

// ! }
#endif