add_executable(mock-waitable-timer-test mock_waitable_timer.test.cpp)
target_link_libraries(mock-waitable-timer-test PRIVATE gtest gtest_main Boost::asio)
gtest_discover_tests(mock-waitable-timer-test)