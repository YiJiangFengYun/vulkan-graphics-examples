#include "graphics/scene/scene.hpp"

namespace vg
{
	BaseScene::BaseScene()
		: Base(BaseType::SCENE)
		, m_isRightHand(VG_FALSE)
	{

	}

    SpaceType BaseScene::getSpaceType() const
	{
		return m_spaceType;
	}

	Bool32 BaseScene::getIsRightHand() const
    {
		return m_isRightHand;
	}

	Bool32 BaseScene::getIsLeftHand() const
	{
	   if (m_isRightHand)
	   {
		   return VG_FALSE;
	   }
	   else
	   {
		   return VG_TRUE;
	   }
	}

	void BaseScene::setIsRightHand(Bool32 isRightHand)
    {
		m_isRightHand = isRightHand;
	}

	void BaseScene::setIsLeftHand(Bool32 isLeftHand)
	{
		if (isLeftHand)
		{
			m_isRightHand = VG_FALSE;
		}
		else
		{
			m_isRightHand = VG_TRUE;
		}
	}

} //namespace kgs