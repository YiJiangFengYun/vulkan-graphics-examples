#include "graphics/texture/attachment.hpp"

namespace vg
{
	BaseColorAttachment::BaseColorAttachment(Bool32 isInputUsage)
	    : m_isInputUsage(isInputUsage)
	{

	}

    BaseColorAttachment::~BaseColorAttachment()
	{

	}

	BaseDepthStencilAttachment::BaseDepthStencilAttachment(Bool32 isInputUsage)
	    : m_isInputUsage(isInputUsage)
	{

	}

	BaseDepthStencilAttachment::~BaseDepthStencilAttachment()
	{
		
	}
} //vg