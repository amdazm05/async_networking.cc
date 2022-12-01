#include <async_udp.hpp>

// ******** Server ********
Async_UDP_server::Async_UDP_server(int PortNum)
    : 
    // Bind socket to the portnumber passed
    _socket(  io_service, 
              boost::asio::ip::udp::endpoint(
                boost::asio::ip::udp::v4(),
                PortNum
                )
           )
{
    recieve_buffer = {};
    std::cout<<"******** --- UDP SERVER INITIALISED --- ********"<<std::endl;
    start_receiver();
}

/// @brief start recieving bytes from any client and call the token below
/// @param none
void Async_UDP_server::start_receiver()
{
    _socket.async_receive_from(
    boost::asio::buffer(recieve_buffer),_endpoint,
    boost::bind
        (
            &Async_UDP_server::handle_receieve,
            this,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

/// @brief kind of like a call back
/// @param none
void Async_UDP_server::handle_receieve(
        const boost::system::error_code & error_caught,
        size_t recievedByteCount
    )
{
    // replymessage = boost::make_shared<std::string> (new std::string("Reply"));
    std::cout<<"Handling recieved bytes count: "<<recievedByteCount<<std::endl;
    std::cout<<recieve_buffer.data()<<std::endl;
    replymessage  = boost::make_shared<std::string>("I sent you something :)");
    _socket.async_send_to
    (
        boost::asio::buffer(*replymessage),
        _endpoint,
        boost::bind(&Async_UDP_server::handle_send, this, replymessage,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred)
    );
    start_receiver();
}


void Async_UDP_server::handle_send
    (
            boost::shared_ptr<std::string>,
            const boost::system::error_code&,
            std::size_t 
    )
{
    std::cout<<"Message Sent "<<std::endl;   
}

void Async_UDP_server::run()
{
    io_service.run();
}
// ******** Client side ********
Async_UDP_client::Async_UDP_client (std::string serverIP, int PortNum) :
_socket(io_service)
{
    // Open the socket
    std::cout<<"******** --- UDP CLIENT INITIALISED --- ********"<<std::endl;
    _socket.open(boost::asio::ip::udp::v4());
    
    _endpoint =  
        boost::asio::ip::udp::endpoint
        (
            boost::asio::ip::address::from_string(serverIP),
            PortNum
        );

}

void Async_UDP_client::sendPacket(std::shared_ptr<char *> buffer, std::size_t sizeofBuffer)
{
    bufferPtr = buffer;
    // Simply place the buffer in a safer pointer 
    std::shared_ptr<char *> packetBuffer = bufferPtr.lock();
    // Check if packetBuffer is not NULL for some odd reasons
    if(static_cast<bool>(packetBuffer))
        _socket.send_to(boost::asio::buffer(*packetBuffer.get(),sizeofBuffer), _endpoint);
    
    bufferPtr.reset();
    return;
}

void  Async_UDP_client::recievePacket()
{
    size_t len = _socket.receive_from(boost::asio::buffer(recieve_buffer), _endpoint);
    std::cout.write(recieve_buffer.data(),len);
}