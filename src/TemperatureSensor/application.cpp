#include "Application.hpp"

#include "TemperatureSensor/HttpClientImpl.hpp"
#include "TemperatureSensor/TemperatureDataStore.hpp"
#include "TemperatureSensor/TemperatureSensor.hpp"
#include "TemperatureSensor/common.hpp"
#include "TemperatureSensor/conversion.hpp"

#include <nlohmann/json.hpp>

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <chrono>
#include <deque>
#include <format>
#include <string>

using namespace TemperatureSensor::common;
using namespace std::chrono_literals;
using namespace boost;

namespace TemperatureSensor::application {

asio::awaitable<void> runApplication(
    const ApplicationSettings& applicationSettings,
    TemperatureSensor& temperatureSensor, TemperatureDataStore& temperatureDataStore)
{
    auto executor = co_await asio::this_coro::executor;

    co_spawn(executor,
        runPeriodically(
            applicationSettings.sensorSamplePeriod, readTemperatureSensor, temperatureDataStore, temperatureSensor),
        asio::detached);
    co_spawn(executor,
        reportTemperatureMeasurements(temperatureDataStore, applicationSettings.host, applicationSettings.service),
        asio::detached);
}

void readTemperatureSensor(
    TemperatureDataStore& temperatureDataStore, TemperatureSensor& temperatureSensor)
{
    try {
        const auto sensorSample = temperatureSensor.read();
        const auto temperatureDegC = conversion::adcValueToTemperatureDegC(sensorSample);
        log(std::format("Sensor value [deg C]: {}", temperatureDegC));
        temperatureDataStore.newSample(temperatureDegC);
    } catch (const TemperatureSensorError& e) {
        log(std::format("Error reading temperature sensor: {}", e.what()));
    }
}

asio::awaitable<void> sendToBackend(TemperatureDataStore& temperatureDataStore,
    const std::string& host, std::deque<TemperatureMeasurement>& measurementsBacklog, HttpClient& httpClient)
{
    constexpr std::size_t maxBacklogSize {10};

    auto newMeasurement = temperatureDataStore.getMeasurementAndReset();

    if (measurementsBacklog.size() >= maxBacklogSize)
        measurementsBacklog.pop_front();
    measurementsBacklog.emplace_back(newMeasurement);

    std::string target {};
    nlohmann::json bodyJson {};
    if (measurementsBacklog.size() > 1) {
        target = endpoint::temperatureMissing;
        bodyJson = nlohmann::json(measurementsBacklog);
    } else {
        target = endpoint::temperature;
        bodyJson = measurementsBacklog.front();
    }

    log(std::format("Measurement to send: {}", bodyJson.dump()));

    const auto [result, body] = co_await httpClient.sendRequest(http_verb::post, host, target, bodyJson.dump());
    log(std::format("Result: {}, body: {}", static_cast<int>(result), body));

    if (result == HttpStatus::Ok) {
        log("OK");
        measurementsBacklog.clear();

    } else if (result == HttpStatus::InternalServerError) {
        log("Internal server error, this should be handled");
    }
}

asio::awaitable<void> reportTemperatureMeasurements(
    TemperatureDataStore& temperatureDataStore, const std::string& host, const std::string& service)
{
    auto executor = co_await asio::this_coro::executor;
    std::deque<TemperatureMeasurement> measurementsBacklog {};

    for (;;) {
        try {
            HttpClientImpl httpClient { executor };
            co_await httpClient.connect(host, service);
            co_await runPeriodically(2min, sendToBackend, temperatureDataStore, host, measurementsBacklog, httpClient);
        } catch (const system::system_error& e) {
            if (e.code() == asio::error::operation_aborted)
                co_return;
            log(std::format("Error in backend communication: {}", e.what()));
        }

        co_await asio::steady_timer { executor, 1s }.async_wait(asio::use_awaitable);
        log("Re-initializing HTTP connection");
    }
}
}