#include "graphics/scene/scene.hpp"

namespace vg
{
	BaseScene::BaseScene()
		: Base(BaseType::SCENE)
	{

	}

    SpaceType BaseScene::getSpaceType() const
	{
		return m_spaceType;
	}

} //namespace kgs