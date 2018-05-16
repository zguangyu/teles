#ifndef TELES_HELPERS_HPP
#define TELES_HELPERS_HPP

#include <memory>
#include <boost/smart_ptr.hpp>
#include <uv.h>

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

/**
 * \brief add fd to libuv.
 *
 * TODO: error check needed.
 */
std::shared_ptr<uv_poll_t> uv_add_fd(uv_loop_t *loop, int fd, uv_poll_cb cb, int events, void *user_data = nullptr)
{
    auto uv_poll_ptr = std::make_shared<uv_poll_t>();
    auto raw_ptr = uv_poll_ptr.get();
    uv_poll_init(loop, raw_ptr, fd);
    if (user_data)
        uv_handle_set_data((uv_handle_t*)raw_ptr, user_data);
    uv_poll_start(raw_ptr, events, cb);

    return uv_poll_ptr;
}

}

#endif // TELES_HELPERS_HPP
