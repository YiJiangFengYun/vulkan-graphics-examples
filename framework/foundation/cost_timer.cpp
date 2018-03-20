#include "foundation/cost_timer.hpp"

namespace fd
{
    CostTimer::CostTimer()
        : m_timerType(TimerType::ONCE)
        , startTime()
        , endTime()
        , currTimer(0.0f)
        , costTimer(0.0f)
        , m_isDoing(FD_FALSE)
        , m_addUpTimer(0.0f)
        , m_addUpCount(0u)
    {

    }

     CostTimer::CostTimer(TimerType timerType)
        : m_timerType(timerType)
        , startTime()
        , endTime()
        , currTimer(0.0f)
        , costTimer(0.0f)
        , m_isDoing(FD_FALSE)
        , m_addUpTimer(0.0f)
        , m_addUpCount(0u)
    {
    }

    void CostTimer::reset()
    {
        currTimer = 0.0f;
        costTimer = 0.0f;
        m_addUpTimer = 0.0f;
		m_addUpCount = 0u;
        m_isDoing = FD_FALSE;
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
		auto diff = std::chrono::duration_cast<std::chrono::microseconds>(now - startTime).count();
		currTimer = static_cast<float>(diff / 1000.0f);
		m_addUpTimer += static_cast<float>(diff / 1000.0f);
        if (m_timerType == TimerType::AVERAGE) {
		    ++m_addUpCount;
            if ( m_addUpTimer > 1000.0f)
		    {
                costTimer = m_addUpTimer / static_cast<float>(m_addUpCount);
		    	m_addUpTimer = 0.0f;
		    	m_addUpCount = 0u;
		    }
        } else if (m_timerType == TimerType::ONCE) {
            costTimer = m_addUpTimer;
            m_addUpTimer = 0.0f;
        } else {
            costTimer = m_addUpTimer;
        }
		
        m_isDoing = FD_FALSE;
    }

    Bool32 CostTimer::getIsDoing() const
    {
        return m_isDoing;
    }
} //fd