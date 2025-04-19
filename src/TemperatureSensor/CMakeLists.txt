add_library(temperature-sensor-lib TemperatureDataStore.cpp TemperatureSensor.cpp)
target_link_libraries(temperature-sensor-lib PUBLIC nlohmann_json Boost::asio)

add_library(application application.cpp)
target_link_libraries(application PRIVATE temperature-sensor-lib PUBLIC Boost::asio Boost::beast nlohmann_json)

add_library(http-client HttpClientImpl.cpp)
target_link_libraries(http-client PUBLIC Boost::asio Boost::beast)

list(APPEND TEST_LIBRARIES gtest gtest_main)

add_executable(conversion-test conversion.test.cpp)
target_link_libraries(conversion-test PRIVATE temperature-sensor-lib nlohmann_json ${TEST_LIBRARIES})
gtest_discover_tests(conversion-test)

add_executable(temperature-sensor-test TemperatureSensor.test.cpp)
target_link_libraries(temperature-sensor-test PRIVATE temperature-sensor-lib nlohmann_json ${TEST_LIBRARIES} Boost::asio)
gtest_discover_tests(temperature-sensor-test)