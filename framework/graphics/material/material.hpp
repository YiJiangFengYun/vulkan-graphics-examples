#ifndef VG_MATERIAL_H
#define VG_MATERIAL_H

#include <memory>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/material/pass.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"

namespace vg
{
	class Material : public Base
	{
	public:
		Material();
		~Material();

		uint32_t getPassCount();
		Pass *getPassWithIndex(uint32_t index) const;
		Bool32 isHas(const Pass *pPass) const;
		void addPass(Pass *pPass);
		void removePass(Pass *pPass);
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
		std::vector<Pass *> m_arrPasses;
		std::unordered_map<InstanceID, Pass *> m_mapPasses;
		//--aggregations

		//tool methods
		
	};
}

#endif // !VG_MATERIAL_H
