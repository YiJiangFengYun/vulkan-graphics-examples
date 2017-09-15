#ifndef KGS_MESH_H
#define KGS_MESH_H

#include <glm/glm.hpp>
#include "graphics/global.hpp"

namespace kgs
{
	template <typename VecType>
	class Mesh
	{
	public:
		typedef VecType type_value;
		Mesh();
		~Mesh();

	private:
		fd::Bounds<type_value> m_bounds;
	};
}

#endif // !KGS_MESH_H
