#pragma once

#include <boost/asio.hpp>

#include <string>
#include <utility>

namespace TemperatureSensor {

namespace http_verb {
  static constexpr auto& post {"POST"};
}

enum class HttpStatus {
  Ok = 200,
  InternalServerError = 500,
};

class HttpClient {
public:
    virtual ~HttpClient() = default;
    virtual boost::asio::awaitable<void> connect(const std::string& host, const std::string& service) = 0;
    virtual boost::asio::awaitable<std::pair<HttpStatus, std::string>> sendRequest(
        const std::string& verb, const std::string& host, const std::string& target, const std::string& body)
        = 0;
};
}