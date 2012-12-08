#include "./src/client/client.h"

#include <boost/asio.hpp>

#include <iostream>
#include <string>
#include <vector>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
  std::cout << "main() - start!" << std::endl;

  Client client;
  client.connect();

  for (int i = 0; i < 10; i++)
  {
    try
    {
      std::vector<std::string> arguments;
      arguments.push_back(std::to_string(i*2));
      arguments.push_back(std::to_string(i));
      std::string reply = client.sendCommand("add", arguments);
      std::cout << "main() - reply: " << reply << std::endl;
    }
    catch (boost::system::system_error& e)
    {
      std::cout << "client main() - ERROR sending command: " << e.what() << std::endl;
    }
  }

  client.disconnect();
  // wait for the server to close the socket
  usleep(100000);
  client.connect();

  for (std::size_t i = 0; i < 10; i++)
  {
    try
    {
      std::vector<std::string> arguments;
      std::string reply = client.sendCommand("test", arguments);
      std::cout << "main() - reply: " << reply << std::endl;
    }
    catch (boost::system::system_error& e)
    {
      std::cout << "client main() - ERROR sending command: " << e.what() << std::endl;
    }
  }

  client.disconnect();
  // wait for the server to close the socket
  usleep(100000);
  client.connect();

  for (std::size_t i = 0; i < 4; i++)
  {
    try
    {
      std::vector<std::string> arguments;
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
