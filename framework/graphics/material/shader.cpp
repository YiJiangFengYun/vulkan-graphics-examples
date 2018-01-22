#include "graphics/material/shader.hpp"

namespace vg
{

	Shader::Shader() 
		: Base(BaseType::SHADER)
		, m_pVertShaderModule()
		, m_pFragShaderModule()
	{

	}

	Shader::Shader(std::string vertShaderPath, std::string fragShaderPath)
		: Base(BaseType::SHADER)
	{
		load(vertShaderPath, fragShaderPath);
	}

	Shader::~Shader()
	{
	}

	void Shader::load(std::string vertShaderPath, std::string fragShaderPath)
	{
		auto vertShaderCode = _readFile(vertShaderPath);
		auto fragShaderCode = _readFile(fragShaderPath);
		m_pVertShaderModule = _createShaderModule(vertShaderCode);
		m_pFragShaderModule = _createShaderModule(fragShaderCode);
	}

	std::shared_ptr<vk::ShaderModule> Shader::getVertShaderModule()
	{
		return m_pVertShaderModule;
	}

	std::shared_ptr<vk::ShaderModule> Shader::getFragShaderModule()
	{
		return m_pFragShaderModule;
	}

	std::shared_ptr<vk::ShaderModule> Shader::_createShaderModule(const std::vector<char>& code)
	{
		auto device = pApp->getDevice();

		/*std::vector<uint32_t> codeAligned(code.size() / sizeof(uint32_t) + 1);
		memcpy(codeAligned.data(), code.data(), code.size());*/

		vk::ShaderModuleCreateInfo createInfo = {
			vk::ShaderModuleCreateFlags(),
			code.size(),
			/*codeAligned.data()*/reinterpret_cast<const uint32_t*>(code.data())
		};

		return fd::createShaderModule(device, createInfo);
	}

	std::vector<char> Shader::_readFile(const std::string& filePath)
	{
		std::ifstream file(filePath, std::ios::ate | std::ios::binary);

		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file: " + filePath);
		}

		size_t fileSize = (size_t)file.tellg();
		std::vector<char> buffer(fileSize);

		file.seekg(0);
		file.read(buffer.data(), fileSize);

		file.close();

		return buffer;
	}
}