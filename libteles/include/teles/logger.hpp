#include <spdlog/spdlog.h>
#include <memory>

namespace teles {

class Daemon;

class Logger
{
public:
    Logger(std::weak_ptr<Daemon> daemon);

private:
    std::shared_ptr<spdlog::logger> logger;
    std::weak_ptr<Daemon> daemon;
};

}
