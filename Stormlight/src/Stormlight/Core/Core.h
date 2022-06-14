#pragma once

#include <memory>
#include "Stormlight/Core/PlatformDetection.h"

#ifdef SL_DEBUG
#if defined(SL_PLATFORM_WINDOWS)
#define SL_DEBUGBREAK() __debugbreak()
#elif defined(SL_PLATFORM_LINUX)
#include <signal.h>
#define SL_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define SL_ENABLE_ASSERTS
#else
#define SL_DEBUGBREAK()
#endif

#ifdef SL_DEBUG
	#define SL_ENABLE_ASSERTS
#endif

#ifdef SL_ENABLE_ASSERTS
	#define SL_ASSERT(x, ...) { if(!(x)) {SL_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define SL_CORE_ASSERT(x, ...) { if(!(x)) {SL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define SL_ASSERT(x, ...)
	#define SL_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)

#define SL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Stormlight {


	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}
