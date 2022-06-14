#include "slpch.h"
#include "Texture.h"

#include "Stormlight/Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace Stormlight {
	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: SL_CORE_ASSERT(false, "RendererAPI: None is currently supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(width, height);
		}

		SL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
	Ref<Texture2D> Stormlight::Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI()) {
		case RendererAPI::API::None: SL_CORE_ASSERT(false, "RendererAPI: None is currently supported!"); return nullptr;
		case RendererAPI::API::OpenGL: return CreateRef<OpenGLTexture2D>(path);
		}

		SL_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}
