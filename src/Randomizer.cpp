#include <Randomizer.h>

Randomizer::Randomizer()
    : rng(std::random_device()()) {
}

double Randomizer::randomUnitDouble() {
    return std::generate_canonical<double, 16>(rng);
}

Randomizer::~Randomizer() {
}