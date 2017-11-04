#ifndef KGS_SCENE_2_H
#define KGS_SCENE_2_H

#include "graphics/scene/space_info.hpp"
#include "graphics/scene/scene.hpp"

namespace kgs
{
	class Scene2 : public Scene<SpaceType::SPACE_2>
	{
	public:
		Scene2();

	private:
		std::shared_ptr<VisualObjectType> m_pRoot;

		void _addVisualObject(const std::shared_ptr<VisualObjectType> pTarget
			, const std::shared_ptr<VisualObjectType> pParent) override;
		void _removeVisualObject(const std::shared_ptr<VisualObjectType> pTarget) override;
	};

} //namespace kgs

#endif // !KGS_SCENE_2_H
