#include <async_udp.hpp>

int main()
{
    try
    {
        boost::asio::io_service io_service;
        Async_UDP_server server(io_service,8090);
        io_service.run();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}