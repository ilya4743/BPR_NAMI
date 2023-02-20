#include "net.h"

void ReportError(sys::error_code ec, std::string_view what) {
    std::cerr << what << ": "sv << ec.message() << std::endl;
}

void SessionBase::Run() {
    net::dispatch(socket_.get_executor(), net::bind_executor(strand_, std::bind(&SessionBase::Read, GetSharedThis())));
}

void SessionBase::Read() {
    using namespace std::literals;
    request_ = {};
    net::async_read(socket_, net::dynamic_buffer(request_),net::transfer_at_least(1),
                     net::bind_executor(strand_, (std::bind(&SessionBase::OnRead, GetSharedThis(), std::placeholders::_1, std::placeholders::_2))));                         
}
    
void SessionBase::OnRead(sys::error_code ec, [[maybe_unused]] std::size_t bytes_read) {
    if (ec) {
        //return close();
    }
    HandleRequest(std::move(request_));
    Read();
}

void SessionBase::Close() {
    sys::error_code ec;
    socket_.shutdown(tcp::socket::shutdown_send, ec);
}

void SessionBase::Write(std::string&& response) {
    auto safe_response = std::make_shared<std::string>(std::move(response));
    auto self = GetSharedThis();
    net::async_write(socket_, net::buffer(*safe_response),
                      net::bind_executor(strand_,[safe_response, self](sys::error_code ec, std::size_t bytes_written) {
                          self->OnWrite(ec, bytes_written);
                      }));
}

void SessionBase::OnWrite(sys::error_code ec, [[maybe_unused]] std::size_t bytes_written) {
    if (ec) {ReportError(ec, "write"sv);
return Close();    }

    //if (close) {
        //return Close();
    //}

    // Считываем следующий запрос
    //Read();
}

void ClientBase::Run()
{
    DoConnect();
}

void ClientBase::DoConnect()
{           
    reconnect_timer.async_wait(std::bind(&ClientBase::Reconnect, GetSharedThis()));
    socket.async_connect(endpoint_, net::bind_executor(strand, std::bind(&ClientBase::OnConnect, GetSharedThis(), std::placeholders::_1, endpoint_)));
}

void ClientBase::OnConnect(const sys::error_code& ec, const tcp::endpoint& endpoint)
{
    if (ec) {
        return ReportError(ec, "connect"sv);
    }
    std::cout<<"connected "<<endpoint<<std::endl;
    if(ConfirmConnect())
    {
        std::cout<<"confirm\n";
        // Асинхронно обрабатываем сессию
        AsyncRunSession(std::move(socket),std::move(strand));
    }
}

bool ClientBase::ConfirmConnect()
{
    std::stringstream ss;
    ss<<(unsigned char)0x79<<(unsigned char)0x92;
    net::write(socket, net::buffer(ss.str()));
    std::string response;
    net::read(socket, net::dynamic_buffer(response), net::transfer_at_least(1));
    if((unsigned char)response[0]==0x79 && (unsigned char)response[1]==0x93)
    {
        std::stringstream ss1;
        ss1<<(unsigned char)0x79<<(unsigned char)0x94;
        net::write(socket, net::buffer(ss1.str()));
        isConnected=true;
    }
}

void ClientBase::Reconnect()
{
    // Check whether the deadline has passed. We compare the deadline against
    // the current time since a new asynchronous operation may have moved the
    // deadline before this actor had a chance to run.
    if (reconnect_timer.expiry() <= net::steady_timer::clock_type::now())
    {
      // The deadline has passed. The socket is closed so that any outstanding
      // asynchronous operations are cancelled.
      socket.close();

      // There is no longer an active deadline. The expiry is set to the
      // maximum time point so that the actor takes no action until a new
      // deadline is set.
      reconnect_timer.expires_after(net::chrono::seconds(5));    
      if (!isConnected)
        DoConnect();
    }

}