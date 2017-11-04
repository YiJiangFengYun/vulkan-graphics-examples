#ifndef KGS_MATERIAL_H
#define KGS_MATERIAL_H

#include <memory>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/material/pass.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"

namespace kgs
{
	class Material : public Base
	{
	public:
		Material();
		~Material();

		uint32_t getPassCount();
		const std::shared_ptr<Pass>& getPassWithIndex(uint32_t index) const;
		Bool32 isHas(const std::shared_ptr<Pass>& pPass);
		void addPass(const std::shared_ptr<Pass>& pPass);
		void removePass(const std::shared_ptr<Pass>& pPass);
		MaterialShowType getShowType();
		void setRenderQueueType(MaterialShowType type);
		uint32_t getRenderPriority();
		void setRenderPriority(uint32_t priority);

		/*Call the methods of all passes in the material.*/
		void apply();
	private:
		//--compositions
		MaterialShowType m_renderQueueType;
		uint32_t m_renderPriority;
		//--compositions

		//--aggregations
		std::vector<std::shared_ptr<Pass>> m_arrPasses;
		std::unordered_map<InstanceID, std::shared_ptr<Pass>> m_mapPasses;
		//--aggregations

		//tool methods
		
	};
}

#endif // !KGS_MATERIAL_H
