#include <async_tcp.hpp>

std::array<char,1<<15> TCP_connection::_message = {};
std::size_t TCP_connection::bytesRecieved = {};
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

inline const std::array<char,1<<15> * TCP_connection::get_buffer()
{
    return &(_message);
}

inline const std::size_t TCP_connection::get_bytes_recieved()
{
    return bytesRecieved;
}

// Please refer here for clarity
// https://stackoverflow.com/questions/5352757/boost-asio-socket-how-to-get-ip-port-address-of-connection
void TCP_connection::start_listening()
{
    std::cout<<"Listening to "<<_socket.remote_endpoint().address().to_string()<<std::endl;
    // clear previous messages
    bytesRecieved ={};
    _message = {};
    boost::asio::async_read(_socket, boost::asio::buffer(_message),
    boost::bind(&TCP_connection::handle_read, shared_from_this(),
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred));
}

void TCP_connection::handle_read(const boost::system::error_code error,size_t bytesrecieved )
{
    bytesRecieved = bytesrecieved;
    // std::cout<<"Recieved this message :"<< _message.data()<<std::endl;
}

// ********  TCP Server     ***********

Async_TCP_server::Async_TCP_server(int PortNum)
: _acceptor(io_context_, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PortNum))
{
    buffer = {};
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
    
    // std::cout<<latched_connection->socket().remote_endpoint().address().to_string()<<std::endl;
}

void Async_TCP_server::handle_accept(boost::shared_ptr<TCP_connection> connection_request,
            const boost::system::error_code& error)
{
    if(!error)
    {
        connection_request->start_listening();
        latched_connection = connection_request;
        latched_to_a_client = true;
        std::cout<<latched_connection->socket().remote_endpoint().address().to_string()<<std::endl;

        boost::asio::async_write(
            latched_connection->socket(),
            boost::asio::buffer(buffer,buffersizetosend),
            boost::bind(&Async_TCP_server::handlewrite, this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred)
        );
    }
    else
    {
        latched_to_a_client = false;
    }
    start_accept();
}

void Async_TCP_server::setPolling(bool flag)
{
    pollingFlag = flag;
}

void Async_TCP_server::run()
{
    // option to poll the user run_once and run 
    // read these https://stackoverflow.com/questions/291871/how-to-set-a-timeout-on-blocking-sockets-in-boost-asio
    // https://stackoverflow.com/questions/4705411/boostasio-io-service-run-vs-poll-or-how-do-i-integrate-boostasio-in-ma
    if(!static_cast<bool>(pollingFlag))
    {
        io_context_.run();
    }

    else
    {
        // Read this
        // https://stackoverflow.com/questions/51878733/boostasioio-contextrun-one-for-fails-to-send-large-buffer
        io_context_.run_one_for(std::chrono::milliseconds(30));
    }
}

const std::array<char,1<<15> * Async_TCP_server::getData()
{
    return TCP_connection::get_buffer();
}

const std::size_t Async_TCP_server::getSizeofData()
{
    return TCP_connection::get_bytes_recieved();
}

bool Async_TCP_server::send_bytes_to_client(std::shared_ptr<char> buff , std::size_t sizeofBuffer)
{
    bool status = true;
    sendBuffer = buff;

    std::cout<<"Sending to client "<<latched_connection->socket().remote_endpoint().address().to_string()<<std::endl;
    std::cout<<"Sizeof buffer "<<sizeofBuffer<<std::endl;
    // Make sure the buffer is not NULL
    if(!static_cast<bool>(sendBuffer.expired()))
    {
        buffersizetosend = sizeofBuffer;
        std::shared_ptr<char> bufferToSend = sendBuffer.lock();
        std::copy(bufferToSend.get(),bufferToSend.get()+sizeofBuffer,buffer.begin());
        std::cout<<"Buffer sent :: "<<buffer.data()<<std::endl;
    }
    else
    {
        buffer ={};
    }

    return status;
}

void Async_TCP_server::handlewrite(const boost::system::error_code error,size_t bytestransfered )
{
    if(!error)
    std::cout<<"handling a write :" <<std::endl;

    std::cout<<error<<std::endl;
}

bool Async_TCP_server::isServerLatchedtoAClient()
{
    return latched_to_a_client;
}