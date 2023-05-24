#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>
#include <memory>

namespace net = boost::asio;
namespace sys = boost::system;
using net::ip::tcp;

using namespace std::literals;

class SessionBase {
   public:
    SessionBase(const SessionBase&) = delete;
    SessionBase& operator=(const SessionBase&) = delete;
    void Run();
    tcp::endpoint GetEndpoint() const;

   protected:
    explicit SessionBase(tcp::socket&& socket)
        : socket_(std::move(socket)){};
    ~SessionBase() = default;
    void Write(std::string&& response);

   private:
    tcp::socket socket_;
    std::string request_;

    virtual std::shared_ptr<SessionBase> GetSharedThis() = 0;
    virtual void HandleRequest(std::string&& request) = 0;

    void Read();
    void OnRead(sys::error_code ec, [[maybe_unused]] std::size_t bytes_read);
    void OnWrite(sys::error_code ec, [[maybe_unused]] std::size_t bytes_written);
    void Close();
};

template <typename RequestHandler>
class Session : public SessionBase, public std::enable_shared_from_this<Session<RequestHandler>> {
   public:
    template <typename Handler>
    Session(tcp::socket&& socket, Handler&& request_handler)
        : SessionBase(std::move(socket)), request_handler_(std::forward<Handler>(request_handler)) {
    }

   private:
    RequestHandler request_handler_;
    std::shared_ptr<SessionBase> GetSharedThis() override {
        return this->shared_from_this();
    }

    void HandleRequest(std::string&& request) override {
        // Захватываем умный указатель на текущий объект Session в лямбде,
        // чтобы продлить время жизни сессии до вызова лямбды.
        // Используется generic-лямбда функция, способная принять response произвольного типа
        request_handler_(GetEndpoint(), std::move(request), [self = this->shared_from_this()](auto&& response) {
            self->Write(std::move(response));
        });
    }
};

class ClientBase {
   public:
    void Run();

   protected:
    ClientBase(net::io_context& ioc, const tcp::endpoint& endpoint)
        : ioc_(ioc), endpoint_(endpoint), socket(ioc), reconnect_timer(ioc), isConnected(false) {
        socket.open(endpoint.protocol());
        socket.set_option(net::socket_base::reuse_address(true));
        socket.set_option(net::socket_base::enable_connection_aborted(true));
    }
    ~ClientBase() = default;

    bool isConnected;

   private:
    net::io_context& ioc_;
    tcp::endpoint endpoint_;
    tcp::socket socket;
    net::deadline_timer reconnect_timer;

    virtual std::shared_ptr<ClientBase> GetSharedThis() = 0;
    virtual void AsyncRunSession(tcp::socket&& socket) = 0;

    void DoConnect();
    void OnConnect(const sys::error_code& ec, const tcp::endpoint& endpoint);
    void ConfirmConnect();
    void Reconnect();
};

template <typename RequestHandler>
class Client : public ClientBase, public std::enable_shared_from_this<Client<RequestHandler>> {
   public:
    template <typename Handler>
    Client(net::io_context& ioc, const tcp::endpoint& endpoint, Handler&& request_handler)
        : ClientBase(ioc, endpoint), request_handler_(std::forward<Handler>(request_handler)) {}

   private:
    RequestHandler request_handler_;

    std::shared_ptr<ClientBase> GetSharedThis() override {
        return this->shared_from_this();
    }

    void AsyncRunSession(tcp::socket&& socket) override {
        std::make_shared<Session<RequestHandler>>(std::move(socket), request_handler_)->Run();
    }
};

template <typename RequestHandler>
void ClientBPR(net::io_context& ioc, const tcp::endpoint& endpoint, RequestHandler&& handler) {
    // При помощи decay_t исключим ссылки из типа RequestHandler,
    // чтобы Client хранил RequestHandler по значению
    using MyClient = Client<std::decay_t<RequestHandler>>;

    std::make_shared<MyClient>(ioc, endpoint, std::forward<RequestHandler>(handler))->Run();
}