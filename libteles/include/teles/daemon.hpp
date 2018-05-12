#ifndef TELES_DAEMON_HPP
#define TELES_DAEMON_HPP

#include <uv.h>
#include <teles/option_parser.hpp>
#include <teles/logger.hpp>

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

    const bool isDaemon() const { return is_daemon; }

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
     * \brief start libuv loop
     *
     * It will initialize the loop variable
     */
    void startLoop();

    bool is_daemon = false;
    const std::string component_name;
};

}

#endif // TELES_DAEMON_HPP
