#include "simulationserver.h"

#include "shared/messagejson.h"

#include "shared/message.h"

SimulationServer::SimulationServer(Server* server)
  : ServerObserver(server)
{
}


SimulationServer::~SimulationServer()
{
}


Message
SimulationServer::createReply(const Message& message)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  std::string json = message.getData();
  std::cout << json << std::endl;

  try
  {
    std::unique_ptr<MessageJSON> received = MessageJSON::createMessageFromJson(json);
    if (!received)
    {
      return Message();
    }

    std::unique_ptr<MessageJSON> reply = received->createReply();
    if (!reply)
    {
      return Message();
    }
    Message messageRaw;
    messageRaw.setData( reply->serialize() );
    return messageRaw;
  }
  catch (std::exception& error)
  {
    std::cerr << error.what() << std::endl;
    return Message();
  }
}



