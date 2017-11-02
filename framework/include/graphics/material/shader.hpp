#ifndef KGS_SHADER_H
#define KGS_SHADER_H

#include <cstdint>
#include <vector>
#include <fstream>
#include <vulkan/vulkan.hpp>
#include <foundation/wrapper.hpp>
#include "graphics/global.hpp"
#include "graphics/module.hpp"

namespace kgs
{
	class Shader
	{
	public:
		Shader();
		//Shader(std::string vertShaderPath);
		//Shader(std::string fragShaderPath);
		Shader(std::string vertShaderPath, std::string fragShaderPath);
		~Shader();

		//void load(std::string vertShaderPath);
		//void load(std::string fragShaderPath);
		void load(std::string vertShaderPath, std::string fragShaderPath);

		std::shared_ptr<vk::ShaderModule> getVertShaderModule();
		std::shared_ptr<vk::ShaderModule> getFragShaderModule();

	private:
		//compositions
		uint32_t m_renderQueueId;
		std::shared_ptr<vk::ShaderModule> m_pVertShaderModule;
		std::shared_ptr<vk::ShaderModule> m_pFragShaderModule;

		//aggregations.

		//tool methods
		std::shared_ptr<vk::ShaderModule> _createShaderModule(const std::vector<char>& code);
		std::vector<char> _readFile(const std::string& filePath);
	};
}

#endif // !KGS_SHADER_H
