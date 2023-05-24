#include "net.h"

#include "constants_app.h"
#include "logger.h"

void ReportError(sys::error_code ec, std::string_view where) {
    BOOST_LOG_TRIVIAL(error) << "where: " << where << " message: " << ec.message();
}

tcp::endpoint SessionBase::GetEndpoint() const {
    return socket_.remote_endpoint();
}

void SessionBase::Run() {
    net::dispatch(socket_.get_executor(), std::bind(&SessionBase::Read, GetSharedThis()));
}

void SessionBase::Read() {
    using namespace std::literals;
    request_ = {};
    net::async_read(socket_, net::dynamic_buffer(request_), net::transfer_at_least(1),
                    (std::bind(&SessionBase::OnRead, GetSharedThis(), std::placeholders::_1, std::placeholders::_2)));
}

void SessionBase::OnRead(sys::error_code ec, [[maybe_unused]] std::size_t bytes_read) {
    if (ec) {
        ReportError(ec, "OnRead(...)"sv);
        throw ec;
    }
    HandleRequest(std::move(request_));
    Read();
}

void SessionBase::Close() {
    sys::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_both, ec);
    socket_.close();
}

void SessionBase::Write(std::string&& response) {
    auto safe_response = std::make_shared<std::string>(std::move(response));
    auto self = GetSharedThis();
    net::async_write(socket_, net::buffer(*safe_response),
                     [safe_response, self](sys::error_code ec, std::size_t bytes_written) {
                         self->OnWrite(ec, bytes_written);
                     });
}

void SessionBase::OnWrite(sys::error_code ec, [[maybe_unused]] std::size_t bytes_written) {
    if (ec) {
        ReportError(ec, "OnWrite(...)"sv);
        return Close();
    }
}

void ClientBase::Run() {
    DoConnect();
}

void ClientBase::DoConnect() {
    reconnect_timer.expires_from_now(boost::posix_time::millisec(BPR_NAMI::Constants::GetInstance().RECONNECT_TIME()));
    reconnect_timer.async_wait(std::bind(&ClientBase::Reconnect, GetSharedThis()));
    socket.async_connect(endpoint_, std::bind(&ClientBase::OnConnect, GetSharedThis(), std::placeholders::_1, endpoint_));
}

void ClientBase::OnConnect(const sys::error_code& ec, const tcp::endpoint& endpoint) {
    if (ec) {
        return ReportError(ec, "OnConnect(...)"sv);
    }
    BOOST_LOG_TRIVIAL(info) << "Connected to " << endpoint;
    BOOST_LOG_TRIVIAL(info) << "Try to confirm connection";
    ConfirmConnect();

    if (isConnected) {
        BOOST_LOG_TRIVIAL(info) << "Connection confirmed ";
        AsyncRunSession(std::move(socket));
    } else
        BOOST_LOG_TRIVIAL(info) << "Invalid header";
}

void ClientBase::ConfirmConnect() {
    std::stringstream ss;
    ss << (unsigned char)0x79 << (unsigned char)0x92;
    net::write(socket, net::buffer(ss.str()));
    std::string response;
    net::read(socket, net::dynamic_buffer(response), net::transfer_at_least(1));
    if ((unsigned char)response[0] == 0x79 && (unsigned char)response[1] == 0x93) {
        std::stringstream ss1;
        ss1 << (unsigned char)0x79 << (unsigned char)0x94;
        net::write(socket, net::buffer(ss1.str()));
        isConnected = true;
    }
}

void ClientBase::Reconnect() {
    if (!isConnected) {
        socket.close();
        BOOST_LOG_TRIVIAL(info) << "Try to reconnect ...";
        DoConnect();
    }
}