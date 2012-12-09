#include "clienttestgroup.h"

#include "client.h"

#include <thread>

ClientTestGroup::ClientTestGroup()
{
}


ClientTestGroup::~ClientTestGroup()
{
}


void
ClientTestGroup::startClientThread()
{
  for (int i = 0; i < 1; ++i)
  {
    std::cout << "ClientTestGroup::startClientThread()" << std::endl;
    std::thread t1(&ClientTestGroup::startClient, this, i);
    std::cout << "ClientTestGroup::startClientThread()" << std::endl;
    t1.detach();
  }
}


bool
ClientTestGroup::startClient(int id)
{
  std::cout << "ClientTestGroup::startClient()" << std::endl;
  Client client;
  client.setName("Client" + std::to_string(id));
  bool isConnected = false;
  isConnected = client.tryConnect(10, 1000);

  if (!isConnected)
  {
    std::cout << "ClientTestGroup::startClient(): could not connect, closing down." << std::endl;
    return false;
  }

  for (int i = 0; i < 1000; i++)
  {
    try
    {
      std::vector<std::string> arguments;
      arguments.push_back(std::to_string(i*2));
      arguments.push_back(std::to_string(i));
      std::string reply = client.sendCommand("add", arguments);
//      std::cout << "ClientTestGroup::startClient() - client name: " << client.getName() << ", reply: " << reply << std::endl;
    }
    catch (boost::system::system_error& e)
    {
      std::cout << "client main() - ERROR sending command: " << e.what() << std::endl;
    }
  }

  client.disconnect();
//  client.tryConnect(10, 100);

//  for (std::size_t i = 0; i < 10; i++)
//  {
//    std::cout << "client main() - sending tests: " << std::endl;
//    try
//    {
//      std::vector<std::string> arguments;
//      std::string reply = client.sendCommand("test", arguments);
//      std::cout << "main() - reply: " << reply << std::endl;
//    }
//    catch (boost::system::system_error& e)
//    {
//      std::cout << "client main() - ERROR sending command: " << e.what() << std::endl;
//    }
//  }
  std::cout << "ClientTestGroup::startClient()" << std::endl;
  return true;
}

