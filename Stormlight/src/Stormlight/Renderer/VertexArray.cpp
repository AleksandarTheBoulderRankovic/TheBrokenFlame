#include "slpch.h"
#include "VertexArray.h"

#include "Stormlight/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Stormlight {

	Ref<VertexArray> VertexArray::Create() {

		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: SL_CORE_ASSERT(false, "RendererAPI: None is currently supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexArray>();
		}

		SL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}