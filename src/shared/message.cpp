#include "message.h"

#include <iostream>

Message::Message(unsigned int version, unsigned int id) :
  m_version(version),
  m_id(id),
  m_data(),
  m_sepChar("@")
{
}


Message::Message() :
  m_version(0),
  m_id(0),
  m_data(),
  m_sepChar("@")
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
  message += m_sepChar;
  message += std::to_string(m_id);
  message += m_sepChar;
  message += m_data;
  return message;
}


void
Message::fromRawString(const std::string& data)
{
  std::cout << __PRETTY_FUNCTION__ << std::endl;
  auto posVersion = data.find_first_of( m_sepChar, 0 );
  std::string version( data.begin(), data.begin() + posVersion );
  auto posId = data.find_first_of( m_sepChar, posVersion+1 );
  std::string id( data.begin()+posVersion+1, data.begin() + posId );

  std::cout << version << std::endl;
  std::cout << id << std::endl;

  m_version = std::stoi(version);
  m_id = std::stoi(id);
  m_data = std::string(data.begin()+posId+1, data.end());
  std::cout << m_data << std::endl;
}
