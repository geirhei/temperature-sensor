#include "TemperatureSensor.hpp"

#include "common.hpp"

#include <filesystem>
#include <format>
#include <iostream>
#include <utility>

using namespace TemperatureSensor;
using namespace common;

TemperatureDataFile::TemperatureDataFile(std::filesystem::path path)
    : m_path { std::move(path) }
{
}

double TemperatureDataFile::read()
{
    if (!m_file.is_open()) {
        m_file.open(m_path);
        m_file.seekg(m_last_file_pos);
    }

    if (double reading; m_file >> reading) {
        m_last_file_pos = m_file.tellg();
        return reading;
    }

    if (m_file.eof()) {
        log("eof detected");
        m_file.clear();
        // If we reach the end of the file, start again from the beginning
        m_file.seekg(0, std::ifstream::beg);
        log("seeked to beginning");
        return read();
    }

    throw TemperatureSensorError { std::format("Failed to read temperature data from file {}", m_path.string()) };
}