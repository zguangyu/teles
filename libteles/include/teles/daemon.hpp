#ifndef TELES_DAEMON_HPP
#define TELES_DAEMON_HPP

#include <uv.h>
#include <teles/option_parser.hpp>

namespace teles {

class Daemon
{
public:
    Daemon(std::string name);
    void run(int argc, char *argv[]);

protected:
    virtual void processOptions();
    OptionParser options;

private:
    /**
     * \brief fork the process to the background.
     *
     * It will also redirect stdio to /dev/null. This is not a cross platform
     * function.
     */
    void doDaemon();

    /**
     * \brief start libuv loop
     *
     * It will initialize the loop variable
     */
    void startLoop();

    bool is_daemon = false;
    uv_loop_t *loop;
    std::string component_name;
};

}

#endif // TELES_DAEMON_HPP
