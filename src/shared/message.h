#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

/**
 * message:
 * version@type@message
 */

class Message
{
public:
  explicit Message();
  explicit Message(unsigned int version, unsigned int id);
  virtual ~Message();

  const std::string& getData() const
  {
    return m_data;
  }

  void setData(const std::string& data)
  {
    m_data = data;
  }

  std::string createMessage() const;
  void fromRawString(const std::string& dataRaw);

private:
  unsigned int m_version;
  unsigned int m_id;
  std::string m_data;
  std::string m_sepChar;
};

#endif // MESSAGE_H
