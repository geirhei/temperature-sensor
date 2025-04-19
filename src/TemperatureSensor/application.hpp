#pragma once

#include <boost/asio.hpp>

#include <chrono>
#include <deque>
#include <string>

namespace TemperatureSensor {

namespace endpoint {
    static constexpr auto& temperature { "/api/temperature" };
    static constexpr auto& temperatureMissing { "/api/temperature/missing" };
}

class TemperatureDataStore;
class TemperatureSensor;
class TemperatureMeasurement;
class HttpClient;

namespace application {
    struct ApplicationSettings {
        std::string host;
        std::string service;
        std::chrono::milliseconds sensorSamplePeriod;
        std::chrono::milliseconds measurementsReportPeriod;
    };

    boost::asio::awaitable<void> runApplication(const ApplicationSettings&, TemperatureSensor&, TemperatureDataStore&);

    void readTemperatureSensor(TemperatureDataStore&, TemperatureSensor&);

    boost::asio::awaitable<void> sendToBackend(TemperatureDataStore&, const std::string& host,
        std::deque<TemperatureMeasurement>& measurementsBacklog, HttpClient&);

    boost::asio::awaitable<void> reportTemperatureMeasurements(TemperatureDataStore&, const std::string& host, const std::string& service);
}
}