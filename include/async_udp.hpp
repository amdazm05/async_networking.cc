#ifndef  _ASYNC_UDP
#define _ASYNC_UDP

// Standard library imports
#include <string>
#include <iostream>
#include <memory>
// Boost imports
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/make_shared.hpp>

//Macros 
#define  SERVER_BUFFER_BYTES  1 << 15
class Async_UDP_server
{
    public:
        Async_UDP_server()=delete; // This constructor is not usuable
        Async_UDP_server(int PortNum);
        void run();
    private:
        //  Member functions to expose
        void start_receiver();
        void handle_receieve(
                const boost::system::error_code & error_caught,
                size_t recievedByteCount 
        );

        void handle_send(
            boost::shared_ptr<std::string>,
            const boost::system::error_code&,
            std::size_t 
        );
        boost::asio::io_service io_service;
        boost::asio::ip::udp::socket _socket;
        boost::asio::ip::udp::endpoint _endpoint;
        // 65535 bytes in place
        std::array<char,SERVER_BUFFER_BYTES> recieve_buffer;
        boost::shared_ptr<std::string> replymessage;
};

class Async_UDP_client
{
    public:
        Async_UDP_client()=delete; // This constructor is not usuable
        Async_UDP_client(std::string serverIP, int PortNum);
        void sendPacket(std::shared_ptr<char *> buffer, std::size_t sizeofBuffer);
        void recievePacket();
    private:
        boost::asio::io_service io_service;
        boost::asio::ip::udp::socket _socket;
        boost::asio::ip::udp::endpoint _endpoint;
        // We will be needing this later
        std::weak_ptr<char *> bufferPtr;
        std::array<char,SERVER_BUFFER_BYTES> recieve_buffer;
};

// References used :
// https://www.boost.org/doc/libs/1_37_0/doc/html/boost_asio/tutorial/tutdaytime6/src.html
// https://www.boost.org/doc/libs/1_66_0/doc/html/boost_asio/reference/ip__udp/socket.html

#endif //_ASYNC_UDP