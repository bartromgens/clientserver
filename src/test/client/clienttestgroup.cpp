#include "clienttestgroup.h"

#include "client.h"


ClientTestGroup::ClientTestGroup()
{
}


ClientTestGroup::~ClientTestGroup()
{
}


void
ClientTestGroup::startClientThread()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  for (int i = 0; i < 4; ++i)
  {
    std::thread t1(&ClientTestGroup::startClient, this, i);
    t1.detach();
  }
}


bool
ClientTestGroup::startClient(int id)
{
  std::cout << "ClientTestGroup::startClient() -id: " << id << std::endl;
  Client client;
  client.connect();

  int nRounds = 1000*1000;
  for (int i = 0; i < nRounds; i++)
  {
    try
    {
      std::vector<std::string> arguments;
      arguments.push_back(std::to_string(i*2));
      arguments.push_back(std::to_string(i));
      std::string reply = client.sendCommand("add", arguments);
      int sum = atoi(reply.c_str());
      assert(sum == i*3);
//      std::cout << "ClientTestGroup::startClient() - " << client.getName() << ", reply: " << reply << std::endl;
    }
    catch (std::exception& e)
    {
      std::cout << "ClientTestGroup::startClient() - ERROR sending command: " << e.what() << std::endl;
      assert(0);
      throw;
    }
  }

  client.disconnect();

  std::cout << "ClientTestGroup::startClient()" << std::endl;
  return true;
}

