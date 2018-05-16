#ifndef TELES_DAEMON_HPP
#define TELES_DAEMON_HPP

#include <uv.h>
#include <teles/option_parser.hpp>
#include <teles/logger.hpp>
#include <zyre.h>

namespace teles {

class Daemon
{
public:
    /**
     * \brief the constructor
     *
     * \param[in] name the name of the component, should be set from subclass
     *
     */
    Daemon(const std::string &name);

    /**
     * \brief start program looping
     *
     * Always called from the main function
     */
    void run(int argc, char *argv[]);

    /**
     * \brief return name of this component
     */
    const std::string& getName() const { return component_name; }

    /**
     * \brief return if it runs as daemon or not
     *
     * This is useful for checking stdio availability.
     */
    const bool isDaemon() const { return is_daemon; }

    const std::string version() const;

protected:
    /**
     * \brief useful when you need to check something
     */
    virtual void onIdle() {}

    /**
     * \brief process options and set some flags
     *
     * This function is suitable for overriding
     *
     */
    virtual void processOptions();

    OptionParser options; //< used to add option

private:
    /**
     * \brief fork the process to the background.
     *
     * It will also redirect stdio to /dev/null. This is not a cross platform
     * function.
     */
    void doDaemon();

    /**
     * \brief start libuv main loop
     *
     * It will initialize the loop variable
     */
    void startLoop();

    /**
     * \brief initialize zyre socket
     */
    void initSocket();

    /**
     * \brief Zyre message process
     */
    static void zyreProcess(uv_poll_t *handle, int status, int events);

    bool is_daemon = false;
    const std::string component_name;

    uv_loop_t *loop;
    std::shared_ptr<uv_poll_t> uv_main_poll;
    zyre_t *zyre_node;

    /**
     * This is a trick for libuv callback to access Daemon object.
     */
    static Daemon *self;
};

}

#endif // TELES_DAEMON_HPP
