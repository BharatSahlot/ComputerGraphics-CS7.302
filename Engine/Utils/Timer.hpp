#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer
{
    public:
        void Start();
        float Tick();
        float TimeSinceStart() const;

    private:
        std::chrono::high_resolution_clock::time_point start;
        std::chrono::high_resolution_clock::time_point last;
};

#endif
