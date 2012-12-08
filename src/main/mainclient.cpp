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
  bool isConnected = false;
  for (std::size_t i = 0; i < 10 && !isConnected; ++i)
  {
    isConnected = client.tryConnect();
    usleep(1000*1000);
  }

  if (!isConnected)
  {
    std::cout << "client main: could not connect, closing down." << std::endl;
    return 0;
  }

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

  std::cout << "main() - end!" << std::endl;
  return 0;
}
