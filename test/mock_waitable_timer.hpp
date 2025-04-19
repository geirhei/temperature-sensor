#pragma once

#include <boost/asio.hpp>

#include <chrono>

namespace test {
using namespace boost;

/**
 * By creating our own specialization of asio's waitable timer template, we can gain full control over what time it observes.
 * We can freeze and jump forward in time to trigger time expiry when we want, which makes it easier to unit test code.
 *
 * The custom clock type allows us to set the current time point returned
 * The custom wait traits make sure that the async timer operation is woken up as soon as our clock is changed.
 */

class my_clock {
public:
    using duration = std::chrono::nanoseconds;
    using time_point = std::chrono::time_point<my_clock>;
    using rep = duration::rep;
    using period = duration::period;
    static constexpr bool is_steady = false;

    static time_point now() noexcept { return s_now; }
    static void set_now(time_point now) { s_now = now; }

private:
    static inline time_point s_now {};
};

struct mock_wait_traits {
    static my_clock::duration to_wait_duration(const my_clock::duration&) { return std::chrono::nanoseconds { 0 }; }
};

typedef asio::basic_waitable_timer<my_clock, mock_wait_traits> mock_waitable_timer;
}
