
#include <boost/asio/ip/tcp.hpp>

#include "logger.h"

using tcp = boost::asio::ip::tcp;

template <class SomeRequestHandler>
class LoggingRequestHandler {
   public:
    LoggingRequestHandler(SomeRequestHandler handler)
        : decorated_(std::move(handler)) {
    }

    template <typename Send>
    void operator()(tcp::endpoint&& endpoint, std::string&& req, Send&& send) {
        auto ip = endpoint.address().to_string();
        auto start_ts = std::chrono::system_clock::now();
        LogRequest(ip, req);

        auto logging_send = [send = std::forward<Send>(send), ip, start_ts](auto&& response) {
            auto end_ts = std::chrono::system_clock::now();
            LogResponse(ip, response, end_ts - start_ts);
            send(std::forward<decltype(response)>(response));
        };

        decorated_(std::forward<decltype(endpoint)>(endpoint), std::forward<decltype(req)>(req), logging_send);
    }

   private:
    static void LogRequest(const std::string& ip, const std::string& req) {
        BOOST_LOG_TRIVIAL(info) << "request from " << ip;
    }

    static void LogResponse(const std::string& ip, const std::string& req, std::chrono::system_clock::duration duration) {
        BOOST_LOG_TRIVIAL(info) << "response to " << ip << " response time [ms]:" << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << std::endl;
    }

    SomeRequestHandler decorated_;
};