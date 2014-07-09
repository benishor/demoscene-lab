#pragma once

#include <random>

class Randomizer {
public:

    Randomizer();
    virtual ~Randomizer();

    double randomUnitDouble();

private:
    std::mt19937 rng;
};
