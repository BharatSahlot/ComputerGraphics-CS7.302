#include "Timer.hpp"
#include <chrono>

using namespace std::chrono;

void Timer::Start()
{
    this->start = high_resolution_clock::now();
    this->last = this->start;
}

float Timer::Tick()
{
    auto time = high_resolution_clock::now();
    float delta = duration_cast<duration<float>>(time - this->last).count();
    this->last = time;
    return delta;
}

float Timer::TimeSinceStart() const
{
    auto time = high_resolution_clock::now();
    float delta = duration_cast<duration<float>>(time - this->start).count();
    return delta;
}
