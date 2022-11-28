#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::udp;

int main(int argc, char* argv[])
{
  try
  {
    if (argc != 2)
    {
      std::cerr << "Usage: client <host>" << std::endl;
      return 1;
    }

    boost::asio::io_service io_service;

    udp::endpoint remote_endpoint = udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8090);

    udp::socket socket(io_service);
    socket.open(udp::v4());

    boost::array<char, 13> send_buf  = {'a','v','d'};
    socket.send_to(boost::asio::buffer(send_buf), remote_endpoint);

    boost::array<char, 128> recv_buf;
    udp::endpoint sender_endpoint;
    size_t len = socket.receive_from(
        boost::asio::buffer(recv_buf), sender_endpoint);

    std::cout.write(recv_buf.data(), len);
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}