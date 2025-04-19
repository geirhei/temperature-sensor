#pragma once

#include <stdexcept>
#include <format>

namespace TemperatureSensor::conversion {
static constexpr double minAdcValue { 0 };
static constexpr double maxAdcValue { 4095 };

static double adcValueToTemperatureDegC(const double value)
{
    if (value < minAdcValue || value > maxAdcValue)
        throw std::invalid_argument { std::format("Value must be in range [{}, {}]", minAdcValue, maxAdcValue) };
    return value / maxAdcValue * 100.0 - 50.0;
}
}
