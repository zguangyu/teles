#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
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
    options.addSwitch("debug", '\0', "debug mode (verbose zyre)");
    options.addSwitch("version", 'v', "print version");
    self = this;
}

void Daemon::run(int argc, char *argv[])
{
    options.parseArgs(argc, argv);
    processOptions();

    if (is_daemon)
        doDaemon();

    // [IMPORTANT] create loop after fork, or it will crash on mac.
    loop = uv_default_loop();
    initSocket();
    startLoop();
}

const std::string Daemon::version() const
{
    return "teles v" TELES_VERSION;
}

void Daemon::processOptions()
{
    if (options.get<bool>("version")) {
        std::cout << version() << std::endl;
        exit(EXIT_SUCCESS);
    }
    if (options.has("daemon"))
        is_daemon = options.get<bool>("daemon");
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

    std::cout << "Fork to background" << std::endl;
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
    // fallback to use idle handler to poll zyre
    uv_zyre_handler = std::make_shared<uv_idle_t>();
    uv_idle_init(loop, uv_zyre_handler.get());
    uv_idle_start(uv_zyre_handler.get(), Daemon::zyreProcess);

    uv_sigint_handler = std::make_shared<uv_signal_t>();
    uv_signal_init(loop, uv_sigint_handler.get());
    uv_signal_start(uv_sigint_handler.get(), Daemon::cleanupAndExit, SIGINT);

    std::cout << "Start looping..." << std::endl;
    uv_run(loop, UV_RUN_DEFAULT);

    uv_loop_close(loop);
}

void Daemon::initSocket()
{
    zyre_node = zyre_new(component_name.c_str());

    if (options.get<bool>("debug"))
        zyre_set_verbose(zyre_node);

    // set udp port
    if (options.has("port"))
        zyre_set_port(zyre_node, options.get<int>("port"));
    else
        zyre_set_port(zyre_node, TELES_UDP_PORT);

    zyre_start(zyre_node);

    if (options.has("site"))
        zyre_join(zyre_node, options.get<std::string>("site").c_str());
    else
        zyre_join(zyre_node, "teles");

    zclock_sleep(250); // wait zyre for initialization

    zsock_t *sock = zyre_socket(zyre_node);
    // The fd is a socket on mac but it is a pipe on linux
    int zyre_fd = zsock_fd(sock);

    // TODO: can't poll the fd from libuv.
}

void Daemon::zyreProcess(uv_idle_t *handle)
{
    zmq_pollitem_t items[1];
    zsock_t *sock = zyre_socket(self->zyre_node);
    items[0].socket = zsock_resolve(sock);
    items[0].events = ZMQ_POLLIN;
    int rc = zmq_poll(items, 1, 0);
    assert (rc >= 0);
    if (!items[0].revents)
        return;

    auto zyre_event = zyre_event_new(self->zyre_node);
    auto event_type = zyre_event_type(zyre_event);
    std::cout << event_type << std::endl;

    zyre_event_destroy(&zyre_event);
}

Daemon::~Daemon()
{
    std::cout << "Clean up..." << std::endl;
    zyre_destroy(&zyre_node);
}

void Daemon::cleanupAndExit(uv_signal_t *handler, int signum)
{
    std::cout << "Catch signal " << signum << std::endl;
    uv_stop(self->loop);
}
