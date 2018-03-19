#include "foundation/cost_timer.hpp"

namespace fd
{
    CostTimer::CostTimer()
        : startTime()
        , endTime()
        , currTimer(0.0f)
        , costTimer(0.0f)
        , m_isDoing(FD_FALSE)
        , m_addUpTimer(0.0f)
        , m_addUpCount(0u)
    {

    }

    void CostTimer::begin()
    {
        startTime = ClockType::now();
        m_isDoing = FD_TRUE;
    }

    void CostTimer::end(Bool32 once)
    {
        auto now = ClockType::now();
		endTime = now;
		auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();
		currTimer = static_cast<float>(diff / 1000.0f);
		m_addUpTimer += static_cast<float>(diff / 1000.0f);
		++m_addUpCount;
		if (once || m_addUpTimer > 1000.0f)
		{
            costTimer = m_addUpTimer / static_cast<float>(m_addUpCount);
			m_addUpTimer = 0.0f;
			m_addUpCount = 0u;
		}

        m_isDoing = FD_FALSE;
    }

    Bool32 CostTimer::getIsDoing() const
    {
        return m_isDoing;
    }
} //fd