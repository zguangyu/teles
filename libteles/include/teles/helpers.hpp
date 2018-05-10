#ifndef TELES_HELPERS_HPP
#define TELES_HELPERS_HPP

#include <memory>
#include <boost/smart_ptr.hpp>

namespace teles {

template<typename T>
boost::shared_ptr<T> make_shared_ptr(std::shared_ptr<T>& ptr)
{
    return boost::shared_ptr<T>(ptr.get(), [ptr](T*) mutable {ptr.reset();});
}

template<typename T>
std::shared_ptr<T> make_shared_ptr(boost::shared_ptr<T>& ptr)
{
    return std::shared_ptr<T>(ptr.get(), [ptr](T*) mutable {ptr.reset();});
}

}

#endif // TELES_HELPERS_HPP
