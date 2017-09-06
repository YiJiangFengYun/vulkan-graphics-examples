#ifndef KGS_PASS_H
#define KGS_PASS_H

#include <unordered_map>
#include "graphics/material/shader.hpp"
#include "graphics/material/pass_option.hpp"
#include "graphics/texture/texture.hpp"

namespace kgs
{

	class Pass
	{
	public:
		struct LayoutBindingInfo
		{
			std::string name;
			std::uint32_t binding;
			DescriptorType descriptorType;
			ShaderStageFlags stageFlags;
		};

		Pass();
		~Pass();

	private:
		//compositons
		std::vector<LayoutBindingInfo> m_binds;

		//aggregations
		std::shared_ptr<Shader> m_shader;

		std::shared_ptr<Texture> m_mainTexture;
		std::unordered_map<std::string, std::shared_ptr<Texture>> m_mapTextures;
	};
}

#endif // !KGS_PASS_H
