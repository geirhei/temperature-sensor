#include "TemperatureSensor/conversion.hpp"

#include "TemperatureSensor/TemperatureMeasurement.hpp"

#include <nlohmann/json.hpp>

#include <gtest/gtest.h>

#include <chrono>
#include <stdexcept>

using namespace TemperatureSensor;

TEST(ConversionTest, adcValueToTemperatureDegCInRange)
{
    EXPECT_NEAR(conversion::adcValueToTemperatureDegC(2048), 0, 0.5);
    EXPECT_NEAR(conversion::adcValueToTemperatureDegC(3000), 23, 0.5);
}

TEST(ConversionTest, adcValueToTemperatureDegCOutsideAllowedRange)
{
    EXPECT_THROW(conversion::adcValueToTemperatureDegC(-1), std::invalid_argument);
    EXPECT_THROW(conversion::adcValueToTemperatureDegC(4096), std::invalid_argument);
}

TEST(ConversionTest, verifyCorrectJsonSerialization)
{
    const TemperatureMeasurement measurement {
        .time = {
            .start = std::chrono::system_clock::time_point { std::chrono::milliseconds { 1738586085123 } },
            .end = std::chrono::system_clock::time_point { std::chrono::milliseconds { 1738586086789 } }
        },
        .min = { -123.456 },
        .max = {678.901},
        .average = {234.567},
    };

    const nlohmann::json expected = {
        { "Time", {
                { "Start", "2025-02-03T12:34:45.123Z" },
                { "End", "2025-02-03T12:34:46.789Z" }
        }},
        { "Min", -123.46 },
        { "Max", 678.90 },
        { "Avg", 234.57 }
    };

    EXPECT_EQ(nlohmann::json(measurement), expected);
}
