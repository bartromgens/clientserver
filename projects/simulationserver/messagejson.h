#ifndef MESSAGEJSON_H
#define MESSAGEJSON_H

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


class MessageJSON : boost::noncopyable
{
public:
  enum MessageType
  {
    none,
    parameters,
    getParameters
  };

public:
  explicit MessageJSON();
  virtual ~MessageJSON();

  virtual std::string serialize() const = 0;
  virtual void deserialize(const std::string& json) = 0;

  static MessageJSON* createMessageFromJson(const std::string& json);
  virtual MessageJSON* createReply() const = 0;

  virtual MessageJSON::MessageType getMessageType() const = 0;

  static MessageType getMessageTypeFromJson(const std::string& json);

protected:
  static int getVersion();

  void addCommandIdAndVersion(boost::property_tree::ptree& pt) const;
  static boost::property_tree::ptree getTreeFromJson(const std::string& json);

  static void print(boost::property_tree::ptree pt);

private:
  static const int ms_version;
};


class GetParameters : public MessageJSON
{
public:
  GetParameters();
  virtual ~GetParameters();

  virtual std::string serialize() const;
  virtual void deserialize(const std::string& json);

  virtual MessageJSON* createReply() const;

  virtual MessageJSON::MessageType getMessageType() const
  {
    return getParameters;
  }

};


class Parameters : public MessageJSON
{
public:
  Parameters();
  virtual ~Parameters();

  virtual std::string serialize() const;
  virtual void deserialize(const std::string& json);

  virtual MessageJSON* createReply() const;

  void setParameters(const std::vector<Parameter>& parameters );
  const std::vector<Parameter>& getParameters() const;

  virtual MessageJSON::MessageType getMessageType() const
  {
    return parameters;
  }

private:
  std::vector<Parameter> m_parameters;
};

#endif // MESSAGEJSON_H
