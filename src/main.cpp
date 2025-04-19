#include "TemperatureSensor/TemperatureDataStore.hpp"
#include "TemperatureSensor/TemperatureSensor.hpp"
#include "TemperatureSensor/application.hpp"
#include "TemperatureSensor/common.hpp"

#include <boost/asio.hpp>

#include <chrono>
#include <exception>
#include <format>
#include <iostream>

#include <cstdlib>

using namespace TemperatureSensor;
using namespace application;
using namespace common;
using namespace std::chrono_literals;
using namespace boost;

int main()
{
    log("Starting application");

    asio::io_context ioContext {};

    TemperatureDataStore temperatureDataStore {};
    TemperatureDataFile temperatureSensor { "../../data/temperature.txt" };

    const ApplicationSettings applicationSettings {
        .host = "127.0.0.1",
        .service = "5000",
        .sensorSamplePeriod = 100ms,
        .measurementsReportPeriod = 2min,
    };

    co_spawn(ioContext, runApplication(applicationSettings, temperatureSensor, temperatureDataStore), asio::detached);

    try {
        const auto numHandlersExecuted = ioContext.run();
        log(std::format("Application exited gracefully after executing {} handlers.", numHandlersExecuted));
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
