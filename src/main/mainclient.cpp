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
  client.connect();

  std::vector<std::string> arguments;
  arguments.push_back("1");
  arguments.push_back("2");

  for (std::size_t i = 0; i < 10; i++)
  {
    try
    {
      std::string reply = client.sendCommand("add", arguments);
      std::cout << "main() - reply: " << reply << std::endl;
    }
    catch (boost::system::system_error& e)
    {
      std::cout << "client main() - ERROR sending command: " << e.what() << std::endl;
    }
  }

  client.disconnect();

  usleep(100000);
  client.connect();

  for (std::size_t i = 0; i < 10; i++)
  {
    try
    {
      std::string reply = client.sendCommand("test", arguments);
      std::cout << "main() - reply: " << reply << std::endl;
    }
    catch (boost::system::system_error& e)
    {
      std::cout << "client main() - ERROR sending command: " << e.what() << std::endl;
    }
  }

  sleep(5);

  std::cout << "main() - end!" << std::endl;
  return 0;
}
