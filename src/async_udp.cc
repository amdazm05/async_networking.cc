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
    
    for(int i=0 ;i<recievedByteCount;i++)
        printf("Recieved character : %i %c" ,i, recieve_buffer.data()+i);
        std::cout<<std::endl;

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