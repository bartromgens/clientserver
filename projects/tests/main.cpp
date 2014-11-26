
#include "server/server.h"
#include "client/client.h"
#include "shared/message.h"
#include "shared/messagejson.h"

#include "test/client/clienttestgroup.h"

#include <thread>

void runTestServer();
void runTestClients();
void runTestServerJSON();
void runTestClientsJSON();


int
main(int /*argc*/, char* /*argv*/[])
{
  std::thread serverThread( runTestServer );
  serverThread.detach();

  std::thread threadJSON( runTestServerJSON );
  threadJSON.detach();

  std::thread clientThread( runTestClients );
  clientThread.join();

  std::thread clientJSONThread( runTestClientsJSON );
  clientJSONThread.join();

  return 0;
}


void runTestServer()
{
  unsigned short port = 2020;
  Server* server  = new Server(port);
  server->startServer();
}


void runTestServerJSON()
{
  unsigned short port = 2021;
  Server* server  = new Server(port);
  server->startServer();
}


void runTestClients()
{
  Client client(ClientServerData::defaultIp, 2020);
  assert( client.connect() );
  assert( client.isConnected() );

  std::string dataIn = "test";

  Message message;
  message.setData(dataIn);

  try
  {
    Message reply = client.sendMessage(message);
    assert( dataIn==reply.getData() );
  }
  catch (std::exception& e)
  {
    std::cout << "runTestClients() - ERROR sending command: " << e.what() << std::endl;
    assert(false);
  }

  client.disconnect();
  assert( !client.isConnected() );
}


void runTestClientsJSON()
{
  Client client(ClientServerData::defaultIp, 2021);
  assert( client.connect() );
  assert( client.isConnected() );

  GetParameters command;
  std::string json = command.serialize();

  Message message(0, 0);
  message.setData( json );

  try
  {
    Message reply = client.sendMessage( message );
    std::cout << "runTestClientsJSON() : " << reply.getData() << std::endl;
    Parameters parametersMessage;
    parametersMessage.deserialize( reply.getData() );
    std::vector<Parameter> parameters = parametersMessage.getParameters();
  }
  catch (std::exception& e)
  {
    std::cout << "runTestClients() - ERROR sending command: " << e.what() << std::endl;
    assert(false);
  }

  client.disconnect();
  assert( !client.isConnected() );
}
