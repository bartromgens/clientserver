#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>

class Message
{
public:
  explicit Message();
  explicit Message(unsigned int version, unsigned int id);
  virtual ~Message();

  std::string createMessage() const;
  void fromRawString(const std::string& dataRaw);

  const std::string& getData() const;
  void setData(const std::string& data);

private:
  unsigned int m_version;
  unsigned int m_id;
  std::string m_data;

  static const std::string m_separationCharacter;
};


class MessageError : public Message
{
public:
  enum Error
  {
    unknown,
    unknownMessageType,
    noObersverToReply
  };

public:
  explicit MessageError();
  explicit MessageError(unsigned int version, unsigned int id, MessageError::Error errorType);
  virtual ~MessageError();

private:
  Error m_errorType;
};

#endif // MESSAGE_H
