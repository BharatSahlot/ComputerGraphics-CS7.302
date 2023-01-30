#include "Random.hpp"

#include <chrono>
#include <random>

static std::mt19937 gen;

namespace Random {
    void Init()
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        gen = std::mt19937(seed);
    }

    int GetInt()
    {
        return gen();
    }

    // random float 0..1
    float GetFloat()
    {
        return (float)gen() / (float)gen.max();
    }
}
