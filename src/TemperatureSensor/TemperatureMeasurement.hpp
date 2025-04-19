#pragma once

#include <nlohmann/json.hpp>

#include <chrono>
#include <ostream>
#include <string>

#include <cmath>

namespace TemperatureSensor {
struct TemperatureMeasurement {
    struct Time {
        std::chrono::system_clock::time_point start {};
        std::chrono::system_clock::time_point end {};
    } time {};

    double min {};
    double max {};
    double average {};
};
}

namespace nlohmann {
template <> struct adl_serializer<TemperatureSensor::TemperatureMeasurement::Time> {

    static void to_json(json& j, const TemperatureSensor::TemperatureMeasurement::Time& value)
    {
        j["Start"] = time_point_to_iso8601(value.start);
        j["End"] = time_point_to_iso8601(value.end);
    }

private:
    static json::string_t time_point_to_iso8601(const std::chrono::system_clock::time_point& time_point)
    {
        const auto milliseconds
            = std::chrono::duration_cast<std::chrono::milliseconds>(time_point.time_since_epoch()) % 1000;
        const std::time_t time = std::chrono::system_clock::to_time_t(time_point);

        std::tm tm {};
        gmtime_s(&tm, &time);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%S");
        oss << '.' << std::setw(3) << std::setfill('0') << milliseconds.count() << 'Z';

        return json::string_t(oss.str());
    }
};

template <> struct adl_serializer<TemperatureSensor::TemperatureMeasurement> {
    static void to_json(json& j, const TemperatureSensor::TemperatureMeasurement& value)
    {
        j["Time"] = value.time;
        j["Min"] = round_to_2_decimals(value.min);
        j["Max"] = round_to_2_decimals(value.max);
        j["Avg"] = round_to_2_decimals(value.average);
    }

private:
    static json::number_float_t round_to_2_decimals(const double value) { return std::round(value * 100) / 100.0; }
};
}
