#pragma once

#include "HttpClient.hpp"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <string>
#include <utility>

namespace TemperatureSensor {
class HttpClientImpl : public HttpClient {
public:
    explicit HttpClientImpl(boost::asio::any_io_executor&);
    boost::asio::awaitable<void> connect(const std::string& host, const std::string& service) override;
    boost::asio::awaitable<std::pair<HttpStatus, std::string>> sendRequest(
        const std::string& verb, const std::string& host, const std::string& target, const std::string& body) override;

private:
    boost::beast::tcp_stream m_stream;
    std::string m_host;
    std::string m_service;
};
}