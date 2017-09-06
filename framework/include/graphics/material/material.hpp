#ifndef KGS_MATERIAL_H
#define KGS_MATERIAL_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "graphics/material/shader.hpp"
#include "graphics/texture/texture.hpp"

namespace kgs
{
	class Material
	{
	public:
		Material(Shader shader);
		~Material();

	private:
		Material() = delete;

		//compositions
		std::shared_ptr<Texture> m_mainTexture;
		std::vector<std::shared_ptr<Texture>> m_arrTextures;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_mapTextures;

		//aggregations
	};
}

#endif // !KGS_MATERIAL_H
