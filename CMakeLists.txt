cmake_minimum_required(VERSION 3.30)
project(temperature_sensor LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

include(cmake/CPM.cmake)

# Windows 11
add_definitions(-D_WIN32_WINNT=0x0A00)

CPMAddPackage(
        NAME Boost
        VERSION 1.87.0
        GITHUB_REPOSITORY "boostorg/boost"
        GIT_TAG "boost-1.87.0"
)

## https://github.com/chriskohlhoff/asio/issues/1590
if (MINGW)
    target_link_libraries(asio INTERFACE ws2_32)
endif()

CPMAddPackage("gh:nlohmann/json@3.11.3")
CPMAddPackage("gh:google/googletest@1.13.0")

include(GoogleTest)
enable_testing()

include_directories("${CMAKE_SOURCE_DIR}/src")

add_subdirectory(src)
add_subdirectory(test)
