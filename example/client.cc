#include <iostream>
#include <async_udp.hpp>  

int main(int argc, char* argv[])
{
  Async_UDP_client client("127.0.0.1",8090);
  std::shared_ptr<char*> buffer ;
  char bufferData[50]="HelloThere!";
  buffer = std::make_shared<char*> (&bufferData[0]);
  client.sendPacket(buffer,sizeof(bufferData));
  client.recievePacket();
  return 0;
}