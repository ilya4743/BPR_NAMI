#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <thread>

#include "constants_app.h"
#include "logger.h"
#include "logging_request_handler.h"
#include "net.h"
#include "request_handler.h"

namespace sys = boost::system;
namespace net = boost::asio;

void StartClient() {
    net::io_context ioc;
    const auto address = net::ip::make_address(BPR_NAMI::Constants::GetInstance().IP());
    const unsigned short port = BPR_NAMI::Constants::GetInstance().PORT();
    auto handler = std::make_shared<handler::RequestHandler>();

    while (true) {
        try {
            // Подписываемся на сигналы и при их получении завершаем работу
            net::signal_set signals(ioc, SIGINT, SIGTERM);
            signals.async_wait([&ioc](const sys::error_code& ec, [[maybe_unused]] int signal_number) {
                if (!ec) {
                    ioc.stop();
                    throw std::runtime_error("Exit from signal"s);
                }
            });

            if (BPR_NAMI::Constants::GetInstance().IS_PRINT_LOG()) {
                LoggingRequestHandler logging_request_handler{[handler](auto&& endpoint, auto&& req, auto&& send) {
                    (*handler)(std::forward<decltype(endpoint)>(endpoint), std::forward<decltype(req)>(req),
                               std::forward<decltype(send)>(send));
                }};
                ClientBPR(ioc, {address, port}, logging_request_handler);
            } else {
                ClientBPR(ioc, {address, port}, [&handler](auto&& endpoint, auto&& req, auto&& send) {
                    (*handler)(std::forward<decltype(endpoint)>(endpoint), std::forward<decltype(req)>(req),
                               std::forward<decltype(send)>(send));
                });
            }
            ioc.run();
        } catch (const std::exception& e) {
            if (e.what() == "Exit from signal"s)
                throw std::runtime_error("Exit from signal"s);
            std::cerr << e.what();
        } catch (...) {
            std::cerr << "errr";
        }
    }
}

int main(int argc, char** argv) {
    try {
        // устанавливаем файлы конфигов для синглтонов
        BPR_NAMI::Constants::SetConstatsFromFile("config.json");
        HybridAStar::Constants::SetConstatsFromFile("config_hybrid_a_star.json");

        InitLogger();
        StartClient();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        if (e.what() != "Exit from signal"s)
            // переподключаемся, если вдруг произошел разрыв соединения
            StartClient();
    }
    return 0;
}
