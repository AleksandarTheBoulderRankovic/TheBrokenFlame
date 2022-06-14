#include "slpch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Stormlight {
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}