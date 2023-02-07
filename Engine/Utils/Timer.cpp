#include "Timer.hpp"
#include <chrono>

using namespace std::chrono;

void Timer::Start()
{
    start = high_resolution_clock::now();
    last = start;
}

float Timer::Tick()
{
    auto time = high_resolution_clock::now();
    float delta = duration_cast<duration<float>>(time - last).count();
    last = time;
    return delta;
}

float Timer::TimeSinceStart() const
{
    auto time = high_resolution_clock::now();
    float delta = duration_cast<duration<float>>(time - start).count();
    return delta;
}
