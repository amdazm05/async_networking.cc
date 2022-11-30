#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  try
  {

    boost::asio::io_context io_context;

    tcp::endpoint remote_endpoint = tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8090);

    tcp::socket socket(io_context);
    boost::asio::connect(socket, remote_endpoint);

    
      boost::array<char, 128> buf = {'a', 'b','c'};
      boost::system::error_code error;

      size_t len = socket.write_some(boost::asio::buffer(buf), error);

     
      if (error)
        throw boost::system::system_error(error); // Some other error.
    
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  }

  return 0;
}