#ifndef FD_COST_TIMER_HPP
#define FD_COST_TIMER_HPP

#include <chrono>
#include "foundation/global.hpp"

namespace fd
{
    class CostTimer
    {
    public:
        using ClockType = std::chrono::steady_clock;
        using TimePointType = typename std::chrono::time_point<ClockType>;
        TimePointType startTime;
		TimePointType endTime;
        float currTimer;
        float costTimer;
        CostTimer();
        void begin();
        void end(Bool32 once = FD_FALSE);

        Bool32 getIsDoing() const;
    private:
        Bool32 m_isDoing;
        float m_addUpTimer;
        uint32_t m_addUpCount;

    };
} //fd

#endif //FD_COST_TIMER_HPP