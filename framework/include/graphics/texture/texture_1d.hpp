#ifndef KGS_TEXTURE_1D_H
#define KGS_TEXTURE_1D_H

#include "graphics/texture/texture.hpp"

#include <glm/glm.hpp>
namespace kgs
{
	class Texture1D : public Texture
	{
	public:
		Texture1D(Device device);
		~Texture1D();

	private:
		Texture1D();
	};
}

#endif // !KGS_TEXTURE_1D_H
