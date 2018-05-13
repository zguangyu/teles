#include <iostream>
#include <boost/smart_ptr.hpp>
#include <boost/program_options.hpp>

#include <teles/option_parser.hpp>

namespace po = boost::program_options;
using namespace teles;

void OptionParser::parseArgs(int ac, char *av[])
{
    try {
        po::store(po::parse_command_line(ac, av, desc), vm);
        po::notify(vm);
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        printHelp();
        std::exit(1);
    }

    if (vm.count("help")) {
        printHelp();
        std::exit(1);
    }
}
