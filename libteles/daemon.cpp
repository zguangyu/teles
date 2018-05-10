#include <teles/daemon.hpp>

using namespace teles;

Daemon::Daemon(std::string name)
{
    options.addOption<bool>(std::string("daemon"), 'd', std::string("run as daemon"));
    if (name != "nameserver")
        options.addOption<std::string>("nameserver", 's', "address (ip:port) of nameserver");
}

void Daemon::run(int argc, char *argv[])
{
    options.parse(argc, argv);
    processOptions();

    if (is_daemon)
        doDaemon();
}

void Daemon::processOptions()
{
    if (options.hasOption("daemon"))
        is_daemon = true;
}

void Daemon::doDaemon()
{

}
