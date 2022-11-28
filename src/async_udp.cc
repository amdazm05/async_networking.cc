#include <async_udp.hpp>

Async_UDP_server::Async_UDP_server(boost::asio::io_service &ioservice, int PortNum)
    : 
    // Bind socket to the portnumber passed
    _socket(  ioservice, 
              boost::asio::ip::udp::endpoint(
                boost::asio::ip::udp::v4(),
                PortNum
                )
           ),
    // Make sure recieve buffer is empty on initialisation
    recieve_buffer({})
{
    std::cout<<"******** --- UDP SERVER INITIALISED --- ********"<<std::endl;
}

/// @brief start recieving bytes from any client and call the token below
/// @param none
void Async_UDP_server::start_receiver()
{
    _socket.async_receive_from(
        recieve_buffer,_endpoint,
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
    std::cout<<"Handling recieved bytes"<<std::endl;
    std::cout<<"Recieved character :" << recieve_buffer.at(1)<<std::endl;
}
