
#include "clientjson.h"

#include "shared/message.h"
#include "shared/messagejson.h"


ClientJSON::ClientJSON(const std::string& ip,
                       unsigned short port)
  : m_client(ip, port)
{
}


ClientJSON::~ClientJSON()
{
}


bool
ClientJSON::connect()
{
  return m_client.connect();
}


bool
ClientJSON::disconnect()
{
  return m_client.disconnect();
}


bool
ClientJSON::isConnected()
{
  return m_client.isConnected();
}


std::vector<ParameterData>
ClientJSON::getParameters()
{
  std::unique_ptr<GetParametersMessage> message(new GetParametersMessage());
  std::unique_ptr<MessageJSON> reply = sendMessage( message.get() );
  ParametersMessage* parametersMessage = dynamic_cast<ParametersMessage*>( reply.get() );
  return parametersMessage->getParameters();
}


std::unique_ptr<MessageJSON>
ClientJSON::sendMessage(MessageJSON* message)
{
  Message messageBasic;
  messageBasic.setData( message->serialize() );
  Message replyBasic = m_client.sendMessage(messageBasic);
  std::unique_ptr<MessageJSON> reply = MessageJSON::createMessageFromJson(replyBasic.getData());
  return reply;
}



