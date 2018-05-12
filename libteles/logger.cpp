#include <teles/logger.hpp>

using namespace teles;

Logger::Logger(std::weak_ptr<Daemon> daemon) : daemon(daemon)
{

}
