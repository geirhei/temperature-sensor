#pragma once

#include "TemperatureSensor/TemperatureMeasurement.hpp"

#include <chrono>
#include <limits>

namespace TemperatureSensor {
using TemperatureCelsius = double;

class TemperatureDataStore {
public:
    void newSample(TemperatureCelsius sample);
    TemperatureMeasurement getMeasurementAndReset();

private:
    void resetDataStore();

    std::chrono::system_clock::time_point m_timeOfFirstSample {};
    std::chrono::system_clock::time_point m_timeOfLastSample {};
    TemperatureCelsius m_min { std::numeric_limits<TemperatureCelsius>::max() };
    TemperatureCelsius m_max { std::numeric_limits<TemperatureCelsius>::min() };
    std::size_t m_numberOfSamples {};
    TemperatureCelsius m_sumOfSamples {};
};
}
