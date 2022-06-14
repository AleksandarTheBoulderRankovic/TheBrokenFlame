#include "slpch.h"
#include "Buffer.h"

#include "Stormlight/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Stormlight {
	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: SL_CORE_ASSERT(false, "RendererAPI: None is currently supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(size);
		}

		SL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size) {
		
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: SL_CORE_ASSERT(false, "RendererAPI: None is currently supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLVertexBuffer>(vertices, size);
		}

		SL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count) {
		
		switch (Renderer::GetAPI()) {
			case RendererAPI::API::None: SL_CORE_ASSERT(false, "RendererAPI: None is currently supported!"); return nullptr;
			case RendererAPI::API::OpenGL: return CreateRef<OpenGLIndexBuffer>(indices, count);
		}

		SL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}