#include "graphics/scene/scene_2.hpp"

namespace kgs
{
	Scene2::Scene2()
		: Scene<SpaceType::SPACE_2>()
	{

	}

	//void Scene2::_addVisualObject(const std::shared_ptr<VisualObjectType> pTarget
	//	, const std::shared_ptr<VisualObjectType> pParent)
	//{
	//	Scene<SpaceType::SPACE_2>::_addVisualObject(pTarget, pParent);

	//	//root 
	//	if (pParent == nullptr)
	//	{
	//		if (m_pRoot != nullptr)
	//		{
	//			//The origin root becomes to child of new root.
	//			pTarget->getTransform()->addChild(m_pRoot->getTransform().get());
	//			m_pRoot = pTarget;
	//		}
	//		else
	//		{
	//			m_pRoot = pParent;
	//		}
	//	}
	//}

	//void Scene2::_removeVisualObject(const std::shared_ptr<VisualObjectType> pTarget)
	//{
	//	//root
	//	if (pTarget == m_pRoot && m_pRoot->getTransform()->getChildCount())
	//	{
	//		throw std::runtime_error("Can't directly remove the root when it has children.");
	//	}
	//	else
	//	{
	//		//connect between children and parent of target.
	//		auto pTranform = pTarget->getTransform();
	//		auto pParent = pTranform->getParent();
	//		auto pos = pParent->getChildPos(pTranform.get());
	//		//copy chilren refs;
	//		auto children = pTranform->getChildren();
	//		//first remove all chilren of target.
	//		pTranform->detachChildren();
	//		//insert to pos of parent before target
	//		for (const auto& child : children)
	//		{
	//			pParent->addChild(child, pos);
	//		}
	//	}
	//	Scene<SpaceType::SPACE_2>::_removeVisualObject(pTarget);

	//	//for root
	//	if (pTarget == m_pRoot)
	//	{
	//		m_pRoot = nullptr;
	//	}
	//}
} //namespace kgs