#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

namespace RenderEngine {

	class Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetEngineLogger() { return _engineLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return _clientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> _engineLogger;
		static std::shared_ptr<spdlog::logger> _clientLogger;
	};

}

// Engine Log Macros
#define ENGINE_TRACE(...)    ::RenderEngine::Log::GetEngineLogger()->trace(__VA_ARGS__)
#define ENGINE_INFO(...)     ::RenderEngine::Log::GetEngineLogger()->info(__VA_ARGS__)
#define ENGINE_WARN(...)     ::RenderEngine::Log::GetEngineLogger()->warn(__VA_ARGS__)
#define ENGINE_ERROR(...)    ::RenderEngine::Log::GetEngineLogger()->error(__VA_ARGS__)
#define ENGINE_CRITICAL(...) ::RenderEngine::Log::GetEngineLogger()->critical(__VA_ARGS__)

// Simulation Log Macros
#define TRACE(...)         ::RenderEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define INFO(...)          ::RenderEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define WARN(...)          ::RenderEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define ERROR(...)         ::RenderEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define CRITICAL(...)      ::RenderEngine::Log::GetClientLogger()->critical(__VA_ARGS__)