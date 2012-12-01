#include "./src/client/client.h"

#include <boost/asio.hpp>

#include <iostream>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  std::cout << "main() - start!" << std::endl;

  Client client(1);

  std::vector<std::string> arguments;
  arguments.push_back("hello!\0");

  for (std::size_t i = 0; i < 100; i++)
  {
    try
    {
      std::string reply = client.sendCommand("test", arguments);
      std::cout << "main() - reply: " << reply << std::endl;
      usleep(10000);
    }
    catch (boost::system::system_error& e)
    {
      std::cout << "client main() - ERROR sending command: " << e.what() << std::endl;
    }
  }

  std::cout << "main() - end!" << std::endl;
  return 0;
}
