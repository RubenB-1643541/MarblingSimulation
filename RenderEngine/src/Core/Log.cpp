#include "pch.h"
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace RenderEngine {

	std::shared_ptr<spdlog::logger> Log::_coreLogger;
	std::shared_ptr<spdlog::logger> Log::_clientLogger;

	void Log::Init()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("FluidSim.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		_coreLogger = std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
		spdlog::register_logger(_coreLogger);
		_coreLogger->set_level(spdlog::level::trace);
		_coreLogger->flush_on(spdlog::level::trace);

		_clientLogger = std::make_shared<spdlog::logger>("SIM", begin(logSinks), end(logSinks));
		spdlog::register_logger(_clientLogger);
		_clientLogger->set_level(spdlog::level::trace);
		_clientLogger->flush_on(spdlog::level::trace);
	}

}
