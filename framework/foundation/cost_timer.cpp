#include "foundation/cost_timer.hpp"

namespace fd
{
    CostTimer::CostTimer()
        : startTime()
        , endTime()
        , currTimer(0u)
        , costTimer(0u)
        , m_isDoing(FD_FALSE)
        , m_addUpTimer(0u)
        , m_addUpCount(0u)
    {

    }

    void CostTimer::begin()
    {
        startTime = ClockType::now();
        m_isDoing = FD_TRUE;
    }

    void CostTimer::end()
    {
        auto now = ClockType::now();
		endTime = now;
		auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - startTime).count();
		currTimer = diff;
		m_addUpTimer += static_cast<uint32_t>(diff);
		++m_addUpCount;
		if (m_addUpTimer > 1000u)
		{
            costTimer = m_addUpTimer / m_addUpCount;
			m_addUpTimer = 0u;
			m_addUpCount = 0u;
		}

        m_isDoing = FD_FALSE;
    }

    Bool32 CostTimer::getIsDoing() const
    {
        return m_isDoing;
    }
} //fd