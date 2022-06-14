#pragma once

#include "Stormlight/Core/Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Stormlight {
	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}



#define SL_CORE_TRACE(...) ::Stormlight::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define SL_CORE_INFO(...) ::Stormlight::Log::GetCoreLogger()->info(__VA_ARGS__);
#define SL_CORE_WARN(...) ::Stormlight::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define SL_CORE_ERROR(...) ::Stormlight::Log::GetCoreLogger()->error(__VA_ARGS__);
#define SL_CORE_CRITICAL(...) ::Stormlight::Log::GetCoreLogger()->critical(__VA_ARGS__);

#define SL_TRACE(...) ::Stormlight::Log::GetClientLogger()->trace(__VA_ARGS__);
#define SL_INFO(...) ::Stormlight::Log::GetClientLogger()->info(__VA_ARGS__);
#define SL_WARN(...) ::Stormlight::Log::GetClientLogger()->warn(__VA_ARGS__);
#define SL_ERROR(...) ::Stormlight::Log::GetClientLogger()->error(__VA_ARGS__);
#define SL_CRITICAL(...) ::Stormlight::Log::GetClientLogger()->critical(__VA_ARGS__);

