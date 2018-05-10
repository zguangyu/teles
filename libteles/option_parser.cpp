#include <iostream>
#include <boost/smart_ptr.hpp>
#include <boost/program_options.hpp>

#include <teles/option_parser.hpp>

namespace po = boost::program_options;
using namespace teles;

template <typename T>
void option_parser::add_option(const std::string &name, const char short_name,
    const std::string &description)
{
    std::ostringstream os;
    os << name;
    if (short_name) {
        os << ',' << short_name;
    }

    po::typed_value<T> *semantic;
    if (typeid(T) == typeid(bool)) {
        semantic = po::bool_switch();
    } else {
        semantic = po::value<T>();
    }

    auto option = boost::shared_ptr<po::option_description>(
        new po::option_description(os.str().c_str(), semantic,
            description.c_str()));

    desc.add(option);
}

template <typename T>
void option_parser::add_option(const std::string &name, const char short_name,
    const std::string &description, const T &default_value)
{
    std::ostringstream os;
    os << name;
    if (short_name) {
        os << ',' << short_name;
    }

    po::typed_value<T> *semantic;
    semantic = po::value<T>();

    auto option = boost::shared_ptr<po::option_description>(
        new po::option_description(os.str().c_str(), semantic,
            description.c_str()));
    
    desc.add(option);
}

void option_parser::parse(int ac, char *av[])
{
    po::store(po::parse_command_line(ac, av, desc), vm);
    po::notify(vm);
}