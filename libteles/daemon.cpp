#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <teles/daemon.hpp>
#include <teles/helpers.hpp>
#include "config.hpp"

using namespace teles;

Daemon *Daemon::self;

Daemon::Daemon(const std::string &name) : options(name), component_name(name)
{
    options.addSwitch("daemon", 'd', "run as daemon");
    options.addOption<int>("port", 'p', "udp port for discovery");
    options.addOption<std::string>("interface", 'i', "interface name for connection");
    options.addOption<std::string>("site", 'i', "site name");

    loop = uv_default_loop();

    self = this;
}

void Daemon::run(int argc, char *argv[])
{
    options.parseArgs(argc, argv);
    processOptions();

    if (is_daemon)
        doDaemon();

    initSocket();

    startLoop();
}

void Daemon::processOptions()
{
    if (options.has("daemon"))
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

    // start new session
    if (setsid()==-1) {
        std::cerr << "failed to become a session leader while"
            "daemonising(errno=" << errno << ")" << std::endl;
    }

    // fork again
    signal(SIGHUP,SIG_IGN); // TODO: install a signal processor here
    pid=fork();
    if (pid == -1) {
        std::cerr << "Fail to fork" << std::endl;
        exit(EXIT_FAILURE);
    } else if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    // redirect stdin, stdout, stderr to /dev/null
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, 0);
    dup2(fd, 1);
    dup2(fd, 2);
    if (fd > 2)
        close(fd);

    chdir("/");
}

void Daemon::startLoop()
{
    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
}

void Daemon::initSocket()
{
    zyre_node = zyre_new(component_name.c_str());

    // set udp port
    if (options.has("port"))
        zyre_set_port(zyre_node, options.get<int>("port"));
    else
        zyre_set_port(zyre_node, TELES_UDP_PORT);

    if (options.has("site"))
        zyre_join(zyre_node, options.get<std::string>("site").c_str());
    else
        zyre_join(zyre_node, "teles");

    void *sock = zsock_resolve(zyre_socket(zyre_node));
    int zyre_fd;
    size_t option_len;
    zmq_getsockopt(sock, ZMQ_FD, &zyre_fd, &option_len);

    uv_main_poll = uv_add_fd(loop, zyre_fd, Daemon::zyreProcess, UV_READABLE);
}

void Daemon::zyreProcess(uv_poll_t *handle, int status, int events)
{

}
