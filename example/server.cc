#include <async_udp.hpp>

int main()
{

    Async_UDP_server server(8090);
    server.run();

    return 0;
}