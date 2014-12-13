#include "message.h"

#include <iostream>

const std::string Message::m_separationCharacter = "@";

Message::Message(unsigned int version, unsigned int id) :
  m_version(version),
  m_id(id),
  m_data("nodata")
{
}


Message::Message() :
  m_version(0),
  m_id(0),
  m_data("nodata")
{
}


Message::~Message()
{
}


const std::string&
Message::getData() const
{
  return m_data;
}


void
Message::setData(const std::string &data)
{
  m_data = data;
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


MessageError::MessageError()
: Message(),
  m_errorType(unknown)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  setData( "ERROR: type:" + std::to_string(m_errorType) );
}

MessageError::MessageError(unsigned int version, unsigned int id, MessageError::Error errorType)
: Message(version, id),
  m_errorType(errorType)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  setData( "ERROR: type:" + std::to_string(m_errorType) );
}

MessageError::~MessageError()
{
}
