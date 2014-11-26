#include "simulationserver.h"

#include "shared/messagejson.h"

#include "shared/message.h"

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
SimulationServer::sendMessage(MessageJSON* message, int connectionId)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  if (!message)
  {
    assert(0);
    return;
  }
  std::cout << message->serialize() << std::endl;

  Message messageRaw;
  messageRaw.setData( message->serialize() );
  m_server->send( messageRaw, connectionId);
}


void
SimulationServer::notifyReceivedData(const Message& message, int connectionId)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::string json = message.getData();
  std::cout << json << std::endl;

  try
  {
    std::unique_ptr<MessageJSON> received = MessageJSON::createMessageFromJson(json);
    if (!received)
    {
      return;
    }

    std::unique_ptr<MessageJSON> reply = received->createReply();
    if (!reply)
    {
      return;
    }
    sendMessage(reply.get(), connectionId);
  }
  catch (std::exception& error)
  {
    std::cerr << error.what() << std::endl;
    return;
  }

}



