#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <iostream>
#include <thread>
#include "net.h"
#include "request_handler.h"

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

    // 4. Создаём обработчик запросов
    RequestHandler handler;

    const auto address = net::ip::make_address("0.0.0.0");
    constexpr unsigned short port = 15556;

    ClientBPR(ioc, {address, port}, [&handler](auto&& req, auto&& send) {
        handler(std::forward<decltype(req)>(req), std::forward<decltype(send)>(send));
    });

    // Эта надпись сообщает тестам о том, что сервер запущен и готов обрабатывать запросы
    std::cout << "Server has started..."sv << std::endl;

    RunWorkers(num_threads, [&ioc] {
        ioc.run();
    });

  return 0;
}
