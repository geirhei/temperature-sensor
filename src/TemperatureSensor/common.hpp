#pragma once

#include <boost/asio.hpp>

#include <chrono>
#include <concepts>
#include <iostream>
#include <string_view>
#include <utility>

namespace TemperatureSensor::common {

/**
 * Helper template for running a function with a fixed period. Allows customization of the TimerType to facilitate unit
 * testing.
 */
template <typename Func, typename TimerType = boost::asio::steady_timer, typename... Args>
    requires std::invocable<Func, Args...>
boost::asio::awaitable<void> runPeriodically(const std::chrono::milliseconds period, Func func, Args&&... args)
{
    TimerType timer { co_await boost::asio::this_coro::executor };
    for (;;) {
        timer.expires_after(period);
        func(std::forward<Args>(args)...);
        co_await timer.async_wait(boost::asio::use_awaitable);
    }
}

static void log(const std::string_view message)
{
    std::cout << message << "\n";
}

}