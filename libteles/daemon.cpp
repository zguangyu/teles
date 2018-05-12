#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <teles/daemon.hpp>

using namespace teles;

Daemon::Daemon(const std::string &name) : options(name), component_name(name)
{
    options.addSwitch(std::string("daemon"), 'd', std::string("run as daemon"));
    if (name != "nameserver")
        options.addOption<std::string>("nameserver", 's', "address (ip:port) of nameserver");
}

void Daemon::run(int argc, char *argv[])
{
    options.parse(argc, argv);
    processOptions();

    if (is_daemon)
        doDaemon();

    startLoop();
}

void Daemon::processOptions()
{
    if (options.hasOption("daemon"))
        is_daemon = true;
}

void Daemon::doDaemon()
{
    pid_t pid = fork();

    // check failure
    if (pid < 0) {
        std::cerr << "Fail to fork" << std::endl;
        exit(EXIT_FAILURE);
    }

    // parent process
    if (pid > 0) {
        std::cout << "Fork to background" << std::endl;
        exit(EXIT_SUCCESS);
    }

    // redirect stdin, stdout, stderr to /dev/null
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
        close(fd);
}

void Daemon::startLoop()
{
    auto loop = uv_default_loop();

    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
}
