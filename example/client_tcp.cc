#include <async_tcp.hpp>
#include <iostream>


void send_something(std::string host, int port, std::string message)
{
	boost::asio::io_service ios;
			
        boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);

        boost::asio::ip::tcp::socket socket(ios);

	socket.connect(endpoint);

	std::array<char, 128> buf;
        std::copy(message.begin(),message.end(),buf.begin());
	boost::system::error_code error;
	socket.write_some(boost::asio::buffer(buf, message.size()), error);
        socket.close();

}

int main()
{
    send_something("127.0.0.1", 8090, "hello flowers team");
    return 0;
}