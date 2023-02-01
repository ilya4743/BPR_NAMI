// #include <QCoreApplication>
// #include <QFile>
// #include "mytcpsocket.h"
// #include "settings.h"
// #include "myexception.h"
// #include "logger.h"

// #include <QDir>
// #include <QScopedPointer>
// #include <QTextStream>
// #include <QDateTime>

// // Умный указатель на файл логирования
// QScopedPointer<QFile>   m_logFile;

// // Объявляение обработчика
// void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// // Реализация обработчика
// void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
// {
//     // Открываем поток записи в файл
//     QTextStream out(m_logFile.data());
//     // Записываем дату записи
//     out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz ");
//     // По типу определяем, к какому уровню относится сообщение
//     switch (type)
//     {
//     case QtInfoMsg:     out << "INF "; break;
//     case QtDebugMsg:    out << "DBG "; break;
//     case QtWarningMsg:  out << "WRN "; break;
//     case QtCriticalMsg: out << "CRT "; break;
//     case QtFatalMsg:    out << "FTL "; break;
//     }
//     // Записываем в вывод категорию сообщения и само сообщение
//     out << context.category << ": "
//         << msg<<'\n';
//     out.flush();    // Очищаем буферизированные данные
// }


// /**
// @mainpage Блок принятия решений Документация
// @details Прив <b> рврвар </b>
// */

// int main(int argc, char *argv[])
// {
//     try
//     {
//         QCoreApplication a(argc, argv);
//         QFile f("config.cfg");
//         if(!f.open(QIODevice::ReadOnly))
//             throw MyException("config.cfg not found", ConfigNotFound);
//         Settings::setDefaults(f.readAll());
//         // Пишем ли логи
//         if(Settings::get(Settings::PRINT_LOG).toBool())
//         {
//             m_logFile.reset(new QFile("log.txt"));
//             // Открываем файл логирования
//             m_logFile.data()->open(QFile::Append | QFile::Text);
//             // Устанавливаем обработчик
//             qInstallMessageHandler(messageHandler);
//         }
//         else
//             qInstallMessageHandler(+[](QtMsgType, const QMessageLogContext &, const QString &){});
//         int DEBUG_OUTPUT=Settings::get(Settings::DEBUG_OUTPUT).toInt();
//         f.close();
//         MyTcpSocket s;
//         s.doConnect(Settings::get(Settings::IP_SERVER).toString(),Settings::get(Settings::PORT).toInt(),
//         DEBUG_OUTPUT, Settings::get(Settings::PRINT_LOG).toBool(), Settings::get(Settings::RECONNECT_TIME).toInt());

//         return a.exec();
//     }  catch (MyException& Ex)
//     {
//         qDebug()<<Ex.what();
//         qDebug()<<"Error "<<Ex.GetErrorCode();
//     }
// }

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
  const unsigned num_threads = std::thread::hardware_concurrency();

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
