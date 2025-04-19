#include "TemperatureSensor.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <memory>

using namespace std::chrono_literals;

struct TemperatureSensorReaderTest : public ::testing::Test {
    std::filesystem::path temperature_file_path { "../../../data/temperature.txt" };
    std::unique_ptr<TemperatureSensor::TemperatureSensor> temperature_sensor
        = std::make_unique<TemperatureSensor::TemperatureDataFile>(temperature_file_path.string());
};

TEST_F(TemperatureSensorReaderTest, read_temperature_repeatedly_ok)
{
    EXPECT_DOUBLE_EQ(3900, temperature_sensor->read());
    EXPECT_DOUBLE_EQ(1204, temperature_sensor->read());
}
