#include <async_tcp.hpp>

int main()
{
    Async_TCP_server server(8090);
    server.run();
    return 0;
}