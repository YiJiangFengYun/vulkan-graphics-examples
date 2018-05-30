#ifndef FD_COST_TIMER_HPP
#define FD_COST_TIMER_HPP

#include <chrono>
#include "foundation/global.hpp"

namespace fd
{
    class CostTimer
    {
    public:
        enum class TimerType {
            ONCE,
            AVERAGE,
            ACCUMULATION
        };
        using ClockType = std::chrono::steady_clock;
        using TimePointType = typename std::chrono::time_point<ClockType>;

        TimePointType startTime;
        TimePointType endTime;
        float currTimer;
        float costTimer;
        CostTimer();
        CostTimer(TimerType timerType);
        void reset();
        void begin();
        void end();

        Bool32 getIsDoing() const;
    private:
        TimerType m_timerType;
        Bool32 m_isDoing;
        float m_addUpTimer;
        uint32_t m_addUpCount;

    };
} //fd

#endif //FD_COST_TIMER_HPP