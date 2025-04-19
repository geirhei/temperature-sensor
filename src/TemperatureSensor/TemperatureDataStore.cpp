#include "TemperatureDataStore.hpp"

#include "TemperatureSensor/TemperatureMeasurement.hpp"

#include <algorithm>
#include <chrono>
#include <limits>

using namespace TemperatureSensor;

void TemperatureDataStore::newSample(const TemperatureCelsius sample)
{
    const auto now = std::chrono::system_clock::now();
    if (m_numberOfSamples++ == 0)
        m_timeOfFirstSample = now;
    m_timeOfLastSample = now;
    m_min = std::min(m_min, sample);
    m_max = std::max(m_max, sample);
    m_sumOfSamples += sample;
}

TemperatureMeasurement TemperatureDataStore::getMeasurementAndReset()
{
    const TemperatureMeasurement measurementToReturn {
        .time = {
            .start = m_timeOfFirstSample,
            .end = m_timeOfLastSample,
        },
        .min = m_min,
        .max = m_max,
        .average = m_sumOfSamples / static_cast<double>(m_numberOfSamples),
    };
    resetDataStore();
    return measurementToReturn;
}

void TemperatureDataStore::resetDataStore()
{
    m_timeOfFirstSample = std::chrono::system_clock::time_point {};
    m_timeOfLastSample = std::chrono::system_clock::time_point {};
    m_min = std::numeric_limits<TemperatureCelsius>::max();
    m_max = std::numeric_limits<TemperatureCelsius>::min();
    m_numberOfSamples = 0;
    m_sumOfSamples = 0;
}
