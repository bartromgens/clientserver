
#include "server/server.h"
#include "server/serverjson.h"
#include "client/client.h"
#include "client/clientjson.h"
#include "shared/message.h"
#include "shared/messagejson.h"

#include <QTime>

#include <atomic>
#include <mutex>
#include <thread>

void runTestServer(Server* server);
void runTestServerJSON(ServerJSON* server);
void runTestClients();
void runTestClientsJSON();
bool serverRunning();
bool serverJSONRunning();

int
main(int /*argc*/, char* /*argv*/[])
{
  Server* server  = new Server(2020);
  std::thread serverThread( runTestServer, server );
  serverThread.detach();

  Server* serverForJson  = new Server(2021);
  ServerJSON* serverJSON  = new ServerJSON(serverForJson);
  std::thread threadJSON( runTestServerJSON, serverJSON );
  threadJSON.detach();

//  QTime timer;
//  timer.start();
//  while (!serverRunning() || !serverJSONRunning())
//  {
//    if (timer.elapsed() > 10000)
//    {
//      assert(false);
//      break;
//    }
//  }

  std::cout << "------------------" << std::endl;
  std::cout << "START CLIENT TESTS" << std::endl;
  std::cout << "------------------" << std::endl;
  std::thread clientThread( runTestClients );
  clientThread.join();

  std::cout << "------------------" << std::endl;
  std::cout << "START CLIENT JSON TESTS" << std::endl;
  std::cout << "------------------" << std::endl;

  std::thread clientJSONThread( runTestClientsJSON );
  clientJSONThread.join();

  server->stopServer();
  std::cout << "STOP SERVERJSON" << std::endl;
  serverForJson->stopServer();

  std::cout << "END TESTS" << std::endl;

  return 0;
}


bool serverRunning()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  Client client(ClientServerData::defaultIp, 2020);
  bool connected = client.connect();
  if (connected)
  {
    client.disconnect();
  }
  return connected;
}

bool serverJSONRunning()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  Client client(ClientServerData::defaultIp, 2021);
  bool connected = client.connect();
  if (connected)
  {
    client.disconnect();
  }
  return connected;
}


void runTestServer(Server* server)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;

  server->startServer();
}


void runTestServerJSON(ServerJSON* server)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  server->startServer();
}


void runTestClients()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  Client client(ClientServerData::defaultIp, 2020);
  assert( client.connect() );
  assert( client.isConnected() );

  std::string dataIn = "test";

  Message message;
  message.setData(dataIn);

  try
  {
    Message reply = client.sendMessage(message);
    std::cout << reply.getData() << std::endl;
//    assert( dataIn==reply.getData() );
  }
  catch (std::exception& e)
  {
    std::cout << "runTestClients() - ERROR sending command: " << e.what() << std::endl;
    assert(false);
  }

  assert( client.disconnect() );
  assert( !client.isConnected() );
}


void runTestClientsJSON()
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  ClientJSON client(ClientServerData::defaultIp, 2021);
  assert( client.connect() );
  assert( client.isConnected() );

  try
  {
    std::vector<ParameterData> parameters = client.getParameters();

    for (auto iter = parameters.begin(); iter != parameters.end(); ++iter)
    {
      std::cout << iter->name << ", " << iter->id <<std::endl;
    }
  }
  catch (std::exception& e)
  {
    std::cout << "runTestClients() - ERROR sending command: " << e.what() << std::endl;
    assert(false);
  }

  assert( client.disconnect() );
  assert( !client.isConnected() );
  std::cout << __PRETTY_FUNCTION__ << " END" << std::endl;
}
