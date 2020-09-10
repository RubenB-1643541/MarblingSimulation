#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace RenderEngine {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return _coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> _coreLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
	};

}

// Engine Log Macros
#define CORE_TRACE(...)    ::RenderEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::RenderEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)     ::RenderEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...)    ::RenderEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::RenderEngine::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Simulation Log Macros
#define TRACE(...)         ::RenderEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define INFO(...)          ::RenderEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define WARN(...)          ::RenderEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ERROR(...)         ::RenderEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define CRITICAL(...)      ::RenderEngine::Log::GetClientLogger()->critical(__VA_ARGS__)