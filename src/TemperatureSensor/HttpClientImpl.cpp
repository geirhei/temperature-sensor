#include "HttpClientImpl.hpp"

#include "common.hpp"

#include <boost/asio.hpp>
#include <boost/beast.hpp>

#include <chrono>
#include <iostream>
#include <string>
#include <utility>

using namespace TemperatureSensor;
using namespace common;

namespace asio = boost::asio;
namespace beast = boost::beast;
namespace http = beast::http;

constexpr auto DefaultCompletionToken = asio::as_tuple(asio::use_awaitable);
static constexpr std::chrono::seconds defaultStreamExpiryDuration { 30 };

HttpClientImpl::HttpClientImpl(asio::any_io_executor& executor)
    : m_stream { executor }
{
}

asio::awaitable<void> HttpClientImpl::connect(const std::string& host, const std::string& service)
{
    asio::ip::tcp::resolver resolver { m_stream.get_executor() };
    const auto [ec, results] = co_await resolver.async_resolve(host, service, DefaultCompletionToken);

    m_stream.expires_after(defaultStreamExpiryDuration);
    co_await m_stream.async_connect(results, asio::use_awaitable);
    log("Connected");
}

asio::awaitable<std::pair<HttpStatus, std::string>> HttpClientImpl::sendRequest(
    const std::string& verb, const std::string& host, const std::string& target, const std::string& body)
{
    http::request<http::string_body> request { http::string_to_verb(verb), target, 11 };
    request.set(http::field::host, host);
    request.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    request.set(http::field::content_type, "application/json");
    request.body() = body;
    request.prepare_payload();

    m_stream.expires_after(defaultStreamExpiryDuration);

    const auto [ec, numBytesTransferred] = co_await http::async_write(m_stream, request, DefaultCompletionToken);
    log(std::format("Bytes written: {}", numBytesTransferred));

    beast::flat_buffer receive_buffer;
    http::response<http::dynamic_body> response;
    const auto [_, numBytesRead]
        = co_await http::async_read(m_stream, receive_buffer, response, DefaultCompletionToken);
    log(std::format("Bytes read: {}", numBytesRead));

    log(std::format("Status: {} {}", response.result_int(), response.reason().data()));

    co_return std::make_pair(
        static_cast<HttpStatus>(response.result_int()), beast::buffers_to_string(response.body().data()));
}