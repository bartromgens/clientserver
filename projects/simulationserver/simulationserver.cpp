#include "simulationserver.h"

#include "message.h"

SimulationServer::SimulationServer(Server* server)
  : ServerObserver(server),
    m_server(server)
{
}


SimulationServer::~SimulationServer()
{
//  m_server->unregisterObserver(this);
}


void
SimulationServer::sendMessage(Message* message, int connectionId)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  if (!message)
  {
    assert(0);
    return;
  }
  std::cout << message->serialize() << std::endl;
  m_server->send(message->serialize(), connectionId);
}


void
SimulationServer::notifyReceivedData(std::vector<std::string> dataStrings, int connectionId)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::string json = dataStrings.front();
  std::cout << json << std::endl;

  try
  {
    Message* received = Message::createMessageFromJson(json);
    if (!received)
    {
      return;
    }

    Message* reply = received->createReply();
    if (!reply)
    {
      return;
    }
    sendMessage(reply, connectionId);
  }
  catch (std::exception& error)
  {
    std::cerr << error.what() << std::endl;
    return;
  }

}



