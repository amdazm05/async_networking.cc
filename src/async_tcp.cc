#include <async_tcp.hpp>

// ********  TCP Connections ***********
TCP_connection::TCP_connection(boost::asio::io_context& io_context )
: _socket(io_context)
{

}

boost::shared_ptr<TCP_connection>
    TCP_connection::create(boost::asio::io_context& io_context)
{
    return boost::shared_ptr<TCP_connection>(new TCP_connection(io_context));
}

boost::asio::ip::tcp::socket& TCP_connection::socket()
{
    return _socket;
}

// Please refer here for clarity
// https://stackoverflow.com/questions/5352757/boost-asio-socket-how-to-get-ip-port-address-of-connection
void TCP_connection::start_listening()
{
    std::cout<<"Listening to "<<_socket.remote_endpoint().address().to_string()<<std::endl;
    
    boost::asio::async_read(_socket, boost::asio::buffer(_message),
    boost::bind(&TCP_connection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void TCP_connection::handle_read(const boost::system::error_code error,size_t bytesrecieved )
{
    std::cout<<"Recieved this message :"<< _message.data()<<std::endl;
}

// ********  TCP Server     ***********

Async_TCP_server::Async_TCP_server(int PortNum)
: _acceptor(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PortNum))
{
    start_accept();
}

void Async_TCP_server::start_accept()
{
    boost::shared_ptr<TCP_connection> connection_request =
        TCP_connection::create(io_context_);
    _acceptor.async_accept(
        connection_request->socket(),
        boost::bind(&Async_TCP_server::handle_accept,this,connection_request,
            boost::asio::placeholders::error
        )
    );
}

void Async_TCP_server::handle_accept(boost::shared_ptr<TCP_connection> connection_request,
            const boost::system::error_code& error)
{
    if(!error)
    {
        connection_request->start_listening();
    }
    
    start_accept();
}
