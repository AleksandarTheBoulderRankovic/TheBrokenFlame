#include "slpch.h"
#include "GraphicsContext.h"

#include "Stormlight/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Stormlight {

	Scope<GraphicsContext> Stormlight::GraphicsContext::Create(void* window)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:    SL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
		case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
		}

		SL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
