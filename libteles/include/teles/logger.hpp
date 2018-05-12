#ifndef TELES_LOGGER_HPP
#define TELES_LOGGER_HPP

#include <spdlog/spdlog.h>
#include <memory>
#include <teles/daemon.hpp>
namespace teles {

class Daemon;

class Logger
{
public:
    Logger(std::weak_ptr<Daemon> daemon);

private:
    std::shared_ptr<spdlog::logger> logger;
    std::weak_ptr<Daemon> daemon;
    std::string name;
    bool has_stdout = true;
};

}


#endif // TELES_LOGGER_HPP
