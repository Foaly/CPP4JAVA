#ifndef CLOCK_H
#define CLOCK_H

#include <chrono>


class Clock
{
public:
    Clock();

    std::chrono::microseconds getElapsedTime() const;

    std::chrono::microseconds restart();

private:
    std::chrono::high_resolution_clock::time_point m_startTime;
};

#endif // CLOCK_H
