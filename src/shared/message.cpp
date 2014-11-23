#include "message.h"

#include <iostream>

const std::string Message::m_separationCharacter = "@";

Message::Message(unsigned int version, unsigned int id) :
  m_version(version),
  m_id(id),
  m_data()
{
}


Message::Message() :
  m_version(0),
  m_id(0),
  m_data()
{
}


Message::~Message()
{
}


std::string
Message::createMessage() const
{
  std::string message;
  message += std::to_string(m_version);
  message += m_separationCharacter;
  message += std::to_string(m_id);
  message += m_separationCharacter;
  message += m_data;
  return message;
}


void
Message::fromRawString(const std::string& data)
{
  auto posVersion = data.find_first_of( m_separationCharacter, 0 );
  std::string version( data.begin(), data.begin() + posVersion );
  auto posId = data.find_first_of( m_separationCharacter, posVersion+1 );
  std::string id( data.begin()+posVersion+1, data.begin() + posId );

  m_version = std::stoi(version);
  m_id = std::stoi(id);
  m_data = std::string(data.begin()+posId+1, data.end());
}
