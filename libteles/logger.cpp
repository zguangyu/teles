#include <teles/logger.hpp>

using namespace teles;

Logger::Logger(std::weak_ptr<Daemon> daemon) : daemon(daemon)
{
    auto daemon_ptr = daemon.lock();
    name = daemon_ptr->getName();
    if (daemon_ptr->isDaemon())
        has_stdout = false;

    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::ansicolor_stdout_sink_mt>());
    logger = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
}
