#include "mock_waitable_timer.hpp"

#include <boost/asio.hpp>
#include <gtest/gtest.h>

#include <chrono>

using namespace std::chrono_literals;
using namespace boost;
using namespace test;

/**
 * This test demonstrates how we can control the expiry time of an asio timer. We verify that the
 * timer does not expire until we have explicitly jumped forward in time.
 */
TEST(MockWaitableTimerTest, testBasicTimeControl)
{
    constexpr auto timerDuration = 1s;

    asio::io_context io_context {};

    bool isDone {};

    mock_waitable_timer mock_timer { io_context };
    mock_timer.expires_after(timerDuration);
    mock_timer.async_wait([&isDone](const auto&) { isDone = true; });

    io_context.poll_one();
    ASSERT_FALSE(isDone) << "Timer should not have expired yet";

    my_clock::set_now(my_clock::now() + timerDuration);
    io_context.run_one();
    EXPECT_TRUE(isDone);
}