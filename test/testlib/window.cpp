#include "testlib/window.hpp"

namespace testlib
{
    template <vg::SpaceType SPACE_TYPE>
	void Window<SPACE_TYPE>::_updateCamera()
	{	
	}

    template <>
	void Window<vg::SpaceType::SPACE_3>::_updateCamera()
	{
		m_pCamera->updateProj(glm::radians(60.0f), (float)m_width / (float)m_height, 0.1f, 256.0f);
		auto transform = m_pCamera->getTransform();

		transform->setLocalPosition(glm::vec3(0.0f, 0.0f, m_zoom));
		transform->setLocalRotation(m_rotation);
		m_pCamera->apply();		
	}

    template <>
	void Window<vg::SpaceType::SPACE_2>::_updateCamera()
	{
		m_pCamera->updateProj(fd::Bounds<glm::vec2>(glm::vec2(-1.0f, -1.0f), 
		    glm::vec2(1.0f, 1.0f)));
		auto pTransform = m_pCamera->getTransform();

		pTransform->setLocalRotation(m_rotation);
		m_pCamera->apply();		
	}
}