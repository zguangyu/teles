#ifndef TELES_OPTION_PARSER_HPP
#define TELES_OPTION_PARSER_HPP

#include <cstdlib>
#include <iostream>
#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace teles {

/**
 * option_parser: a wrapper for boost::program_options
 * 
 * This class will handle help option for you.
 * 
 * TODO: implement positional argument
 */
class option_parser
{
public:
    option_parser() : option_parser("") {}
    option_parser(const std::string &caption) : caption(caption) {
        desc.add_options()("help", "print this help");
    }

    /**
     * Add an option choice without default value.
     * 
     * \sa add_option(const std::string&, const char, const std::string&, const T&)
     * 
     */
    template <typename T>
    void add_option(const std::string &name, const char short_name,
            const std::string &description);

    /**
     * Add an option choice without default value.
     * 
     * An option without default value is a required option. The template type
     * is the type of the argument, bool for no argument, vector for multiple
     * arguments.
     * 
     * \param[in]  name          long option name
     * \param[in]  short_name    short option name, '\0' to omit
     * \param[in]  description   help string
     * \param[in]  default_value default value of the option, can be ignored.
     */
    template <typename T>
    void add_option(const std::string &name, const char short_name,
            const std::string &description, const T &default_value);

    /**
     * Parse argument
     * 
     * \param[in] ac argc
     * \param[in] av argv
     */
    void parse(int ac, char *av[]);

    /**
     * get the option argument
     * 
     * It's your responsibility to insure the type is correct.
     * 
     */
    template <typename T>
    const T& get_option(const std::string &name) const {
        return vm[name].as<T>();
    }

    bool has_option(const std::string &name) const {
        return !!vm.count(name);
    }

    /**
     * print the help message to stderr and quit.
     */
    void print_help() const {
        std::cerr << caption << desc << std::endl;
        std::exit(1);
    }

private:
    po::options_description desc;
    po::variables_map vm;
    std::string caption;
};

}


#endif // TELES_OPTION_PARSER_HPP