#ifndef RANDOM_H
#define RANDOM_H

#include <algorithm>
#include <random>

namespace Random {
    extern std::mt19937 gen;

    void Init();
    int GetInt();
    float GetFloat();

    template<typename It>
    void Shuffle(It begin, It end)
    {
        std::shuffle(begin, end, gen);
    }
}

#endif
