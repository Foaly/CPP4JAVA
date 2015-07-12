#include "clock.hpp"

Clock::Clock() :
    m_startTime(std::chrono::high_resolution_clock::now())
{

}


std::chrono::microseconds Clock::getElapsedTime() const
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - m_startTime);
}


std::chrono::microseconds Clock::restart()
{
    auto now = std::chrono::high_resolution_clock::now();
    auto elapsed = now - m_startTime;
    m_startTime = now;

    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed);
}

