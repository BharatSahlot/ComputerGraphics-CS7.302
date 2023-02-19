#include "Random.hpp"

#include <algorithm>
#include <chrono>
#include <random>

namespace Random {

    std::mt19937 gen;

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
