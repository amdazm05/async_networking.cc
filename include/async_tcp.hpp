#ifndef _ASYNC_TCP
#define _ASYNC_TCP
// Standard library imports
#include <string>
#include <iostream>
#include <memory>
// Boost imports
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

// This server class Servers accepts incoming connections
class TCP_connection
  : public boost::enable_shared_from_this<TCP_connection>
{
    public:
        static boost::shared_ptr<TCP_connection> create(boost::asio::io_context& io_context);
        boost::asio::ip::tcp::socket& socket();
        void start_listening();
        static const std::array<char,1<<15> * get_buffer();
        static const std::size_t get_bytes_recieved();
        static void clearRecievedData();
    private:
        TCP_connection(boost::asio::io_context& io_context);
        void handle_read(const boost::system::error_code error,size_t bytesrecieved );
        boost::asio::ip::tcp::socket _socket;
        static std::array<char,1<<15> _message;
        static std::size_t bytesRecieved; 
};

class Async_TCP_server
{
    public:
        Async_TCP_server()=delete;
        Async_TCP_server(int PortNum);
        void run();
        void setPolling(bool flag);
        bool send_bytes_to_client(char* buffer , std::size_t sizeofBuffer);
        const std::array<char,1<<15> * getData();
        const std::size_t getSizeofData();
        bool isServerLatchedtoAClient();
        void clear_read_data();
    private:
        void start_accept();
        void handle_accept(boost::shared_ptr<TCP_connection> connection_request,
            const boost::system::error_code& error);
        void handlewrite(const boost::system::error_code error,size_t bytestransfered );
        boost::asio::io_context io_context_;
        boost::asio::ip::tcp::acceptor _acceptor;
        std::size_t buffersizetosend;
        std::array<char, 1<<15 > buffer;
        std::weak_ptr<char> sendBuffer;
        boost::shared_ptr<TCP_connection> latched_connection;
        bool latched_to_a_client;
        bool pollingFlag;
};

class Async_TCP_client
{
    public:
        Async_TCP_client()=delete;
        Async_TCP_client(std::string serverIP,int PortNum);
    private:
        void sendPacket(std::shared_ptr<char *> buffer, std::size_t sizeofBuffer);
        void recievePacket();
};

#endif //_ASYNC_TCP