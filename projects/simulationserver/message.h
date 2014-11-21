#ifndef MESSAGE_H
#define MESSAGE_H

#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/json_parser.hpp"

#include <string>

class Parameters;

struct Parameter
{
  Parameter(std::string name, int id)
    : name(name), id(id)
  {}

  std::string name;
  int id;
};


class Message : boost::noncopyable
{
public:
  enum MessageType
  {
    none,
    parameters,
    getParameters
  };

public:
  explicit Message();
  virtual ~Message();

  virtual std::string serialize() const = 0;
  virtual void deserialize(const std::string& json) = 0;

  static Message* createMessageFromJson(const std::string& json);
  virtual Message* createReply() const = 0;

  virtual Message::MessageType getMessageType() const = 0;

  static MessageType getMessageTypeFromJson(const std::string& json);

protected:
  static int getVersion();

  void addCommandIdAndVersion(boost::property_tree::ptree& pt) const;
  static boost::property_tree::ptree getTreeFromJson(const std::string& json);

  static void print(boost::property_tree::ptree pt);

private:
  static const int ms_version;
};


class GetParameters : public Message
{
public:
  GetParameters();
  virtual ~GetParameters();

  virtual std::string serialize() const;
  virtual void deserialize(const std::string& json);

  virtual Message* createReply() const;

  virtual Message::MessageType getMessageType() const
  {
    return getParameters;
  }

};


class Parameters : public Message
{
public:
  Parameters();
  virtual ~Parameters();

  virtual std::string serialize() const;
  virtual void deserialize(const std::string& json);

  virtual Message* createReply() const;

  void setParameters(const std::vector<Parameter>& parameters );
  const std::vector<Parameter>& getParameters() const;

  virtual Message::MessageType getMessageType() const
  {
    return parameters;
  }

private:
  std::vector<Parameter> m_parameters;
};

#endif // MESSAGES_H
