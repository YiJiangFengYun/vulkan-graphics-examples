#ifndef VG_MATERIAL_H
#define VG_MATERIAL_H

#include <memory>
#include "graphics/global.hpp"
#include "graphics/util/util.hpp"
#include "graphics/pass/pass.hpp"
#include "graphics/texture/texture.hpp"
#include "graphics/material/material_data.hpp"
#include "graphics/material/visualizer.hpp"

namespace vg
{
	class Material : public Base
	{
	public:
		Material();
		~Material();

		uint32_t getPassCount() const;
		Pass *getPassWithIndex(uint32_t index) const;
		Pass * const *getPasses() const;
		Bool32 isHas(const Pass *pPass) const;
		void addPass(Pass *pPass);
		void removePass(Pass *pPass);
		MaterialShowType getShowType();
		void setRenderQueueType(MaterialShowType type);
		uint32_t getRenderPriority();
		void setRenderPriority(uint32_t priority);
		/*Call the apply methods of all passes in the material.*/
		void apply();

		/**This method is called by visual object to create its visualizer.**/
		Visualizer *allocateVisualizer(Visualizer **ppVisualizer = nullptr);
		void deallocateVisualizer(Visualizer *pVisualizer);
		void clearVisualizers();
	private:
		//--compositions
		MaterialShowType m_renderQueueType;
		uint32_t m_renderPriority;
		std::vector<std::shared_ptr<Visualizer>> m_pVisualizers;
		uint32_t m_unusedVisualizerCount;
		std::vector<Visualizer *> m_pUnusedVisualizes;
		//--compositions

		//--aggregations
		std::vector<Pass *> m_arrPasses;
		std::unordered_map<InstanceID, Pass *> m_mapPasses;
		//--aggregations

		virtual Visualizer *_createVisualizer();
		void _updateVisualizer();
	};
}

#endif // !VG_MATERIAL_H
