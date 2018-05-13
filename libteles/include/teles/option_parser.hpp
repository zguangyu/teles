#ifndef TELES_OPTION_PARSER_HPP
#define TELES_OPTION_PARSER_HPP

#include <cstdlib>
#include <iostream>
#include <string>
#include <type_traits>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace teles {

/**
 * \brief OptionParser is a wrapper for boost::program_options
 *
 * This class will handle help option for you.
 *
 * TODO: implement positional argument
 */
class OptionParser
{
public:
    OptionParser() : OptionParser("") {}
    /**
     * \brief constructor of OptionParser
     *
     * \param[in] caption  the header of the help string
     */
    OptionParser(const std::string &caption) : caption(caption) {
        desc.add_options()("help", "print this help");
    }

    /**
     * \brief Add an option choice without default value.
     *
     * \sa addOption(const std::string&, const char, const std::string&, const T&, T&)
     *
     */
    template <typename T>
    void addOption(const std::string &name, char short_name,
            const std::string &description, T &storage)
    {
        std::ostringstream os;
        os << name;
        if (short_name) {
            os << ',' << short_name;
        }

        po::typed_value<T> *semantic;
        semantic = po::value<T>(&storage);

        auto option = boost::shared_ptr<po::option_description>(
            new po::option_description(os.str().c_str(), semantic,
                description.c_str()));

        desc.add(option);
    }

    template <typename T>
    void addOption(const std::string &name, char short_name,
            const std::string &description)
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

    /**
     * \brief Add an option choice with default value.
     *
     * The template type is the type of the argument, bool for no argument,
     * vector for multiple arguments.
     *
     * \param[in]  name          long option name
     * \param[in]  short_name    short option name, '\0' to omit
     * \param[in]  description   help string
     * \param[in]  default_value default value of the option, can be ignored
     * \param[out] storage       variable reference to store value
     */
    template <typename T>
    void addOption(const std::string &name, char short_name,
            const std::string &description, const T &default_value, T &storage)
    {
        std::ostringstream os;
        os << name;
        if (short_name) {
            os << ',' << short_name;
        }

        po::typed_value<T> *semantic;
        semantic = po::value<T>(&storage)->default_value(default_value);

        auto option = boost::shared_ptr<po::option_description>(
            new po::option_description(os.str().c_str(), semantic,
                description.c_str()));

        desc.add(option);
    }


    template <typename T>
    void addOption(const std::string &name, char short_name,
            const std::string &description, const T &default_value)
    {
        std::ostringstream os;
        os << name;
        if (short_name) {
            os << ',' << short_name;
        }

        po::typed_value<T> *semantic;
        semantic = po::value<T>()->default_value(default_value);

        auto option = boost::shared_ptr<po::option_description>(
            new po::option_description(os.str().c_str(), semantic,
                description.c_str()));

        desc.add(option);
    }


    void addSwitch(const std::string &name, char short_name,
            const std::string &description)
    {
        std::ostringstream os;
        os << name;
        if (short_name) {
            os << ',' << short_name;
        }

        po::typed_value<bool> *semantic;
        semantic = po::bool_switch();

        auto option = boost::shared_ptr<po::option_description>(
            new po::option_description(os.str().c_str(), semantic,
                description.c_str()));

        desc.add(option);
    }

    /**
     * \brief add a flag switch without argument
     */
    void addSwitch(const std::string &name, char short_name,
            const std::string &description, bool &storage)
    {
        std::ostringstream os;
        os << name;
        if (short_name) {
            os << ',' << short_name;
        }

        po::typed_value<bool> *semantic;
        semantic = po::bool_switch(&storage);

        auto option = boost::shared_ptr<po::option_description>(
            new po::option_description(os.str().c_str(), semantic,
                description.c_str()));

        desc.add(option);
    }


    /**
     * \brief Parse argument
     *
     * \param[in] ac argc
     * \param[in] av argv
     */
    void parseArgs(int ac, char *av[]);

    /**
     * \brief get the option argument
     *
     * It's your responsibility to insure the type is correct.
     *
     */
    template <typename T>
    const T& get(const std::string &name) const {
        return vm[name].as<T>();
    }

    /**
     * \brief check if it has option `name`
     */
    bool has(const std::string &name) const {
        return !!vm.count(name);
    }

    /**
     * \brief print the help message to stderr and quit.
     */
    void printHelp() const {
        std::cerr << caption << std::endl << desc << std::endl;
    }

private:
    po::options_description desc;
    po::variables_map vm;
    std::string caption;
};

}


#endif // TELES_OPTION_PARSER_HPP
