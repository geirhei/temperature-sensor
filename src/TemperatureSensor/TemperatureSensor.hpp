#pragma once

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace TemperatureSensor {

class TemperatureSensorError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class TemperatureSensor {
public:
    virtual ~TemperatureSensor() = default;
    virtual double read() = 0;
};

class TemperatureDataFile : public TemperatureSensor {
public:
    explicit TemperatureDataFile(std::filesystem::path);
    double read() override;

private:
    std::filesystem::path m_path;
    std::ifstream m_file;
    std::ifstream::pos_type m_last_file_pos;
};

}
