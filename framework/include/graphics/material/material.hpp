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
	class Material
	{
	public:
		Material();
		~Material();

		int32_t getPassIndex(std::string name);
		std::string getPassName(uint32_t index);
		const std::shared_ptr<Pass>& getPass(std::string name) const;
		void setPass(std::string name, const std::shared_ptr<Pass>& pass);
		const std::shared_ptr<Pass>& getPassWithIndex(uint32_t index) const;
		uint32_t getPassCount();
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
		std::vector<std::string> m_arrPasseNames;
		std::unordered_map<std::string, std::shared_ptr<Pass>> m_mapPasses;
		//--aggregations

		//tool methods
		
	};
}

#endif // !KGS_MATERIAL_H
