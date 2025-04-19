add_subdirectory(TemperatureSensor)

add_executable(temperature-sensor main.cpp)
target_link_libraries(temperature-sensor PRIVATE application temperature-sensor-lib http-client Boost::asio Boost::beast)