#ifndef TELES_DAEMON_HPP
#define TELES_DAEMON_HPP

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
    void doDaemon();
    bool is_daemon = false;
};

}

#endif // TELES_DAEMON_HPP
