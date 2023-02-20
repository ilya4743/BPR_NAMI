#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <thread>
#include "net.h"
#include "request_handler.h"
#include "const.h"


namespace sys = boost::system;
namespace net = boost::asio;

// Запускает функцию fn на n потоках, включая текущий
template <typename Fn>
void RunWorkers(unsigned n, const Fn& fn) {
    n = std::max(1u, n);
    std::vector<std::jthread> workers;
    workers.reserve(n - 1);
    // Запускаем n-1 рабочих потоков, выполняющих функцию fn
    while (--n) {
        workers.emplace_back(fn);
    }
    fn();
}


int main(int argc, char **argv)
{
    try
    {
        BPR_NAMI::Constants::SetConstatsFromFile("config.json");

        unsigned num_threads = std::thread::hardware_concurrency();        
        //num_threads=0;

        net::io_context ioc(num_threads);
        // Подписываемся на сигналы и при их получении завершаем работу
        net::signal_set signals(ioc, SIGINT, SIGTERM);
        signals.async_wait([&ioc](const sys::error_code& ec, [[maybe_unused]] int signal_number) {
            if (!ec) {
                ioc.stop();
            }
        });

        // Создаём обработчик запросов
        RequestHandler handler;

        const auto address = net::ip::make_address(BPR_NAMI::Constants::GetInstance().IP());
        const unsigned short port = BPR_NAMI::Constants::GetInstance().PORT();

        ClientBPR(ioc, {address, port}, [&handler](auto&& req, auto&& send) {
            handler(std::forward<decltype(req)>(req), std::forward<decltype(send)>(send));
        });

        RunWorkers(num_threads, [&ioc] {
            ioc.run();
        });
    }
    catch(...)
    {
        std::cout<<"error";
    }

  return 0;
}
