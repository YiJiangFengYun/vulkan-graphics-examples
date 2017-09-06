#ifndef KGS_SHADER_H
#define KGS_SHADER_H

#include <cstdint>

namespace kgs
{
	class Shader
	{
	public:
		Shader();
		~Shader();

	private:
		uint32_t m_renderQueueId;
	};
}

#endif // !KGS_SHADER_H
