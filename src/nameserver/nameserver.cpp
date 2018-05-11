#include <teles/daemon.hpp>

namespace teles {

class NameServer : public Daemon
{
public:
    NameServer() : Daemon::Daemon("nameserver") {}
};

}


int main(int argc, char *argv[])
{
    auto nameserver = teles::NameServer();
    nameserver.run(argc, argv);
    return 0;
}
