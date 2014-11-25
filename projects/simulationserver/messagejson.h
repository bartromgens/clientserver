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

  static std::unique_ptr<MessageJSON> createMessageFromJson(const std::string& json);
  virtual std::unique_ptr<MessageJSON> createReply() const = 0;

  virtual MessageJSON::MessageType getMessageType() const = 0;

  virtual std::string getName() const = 0;


protected:
  static int getVersion();

  static boost::property_tree::ptree getTreeFromJson(const std::string& json);
  void addTypeAndVersion(boost::property_tree::ptree& pt) const;
  bool checkMessageVersionAndTypeName(const boost::property_tree::ptree& pt) const;

  static void print(boost::property_tree::ptree pt);

private:
  static MessageType getMessageTypeFromJson(const std::string& json);

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

  virtual std::unique_ptr<MessageJSON> createReply() const;

  virtual MessageJSON::MessageType getMessageType() const
  {
    return getParameters;
  }

  virtual std::string getName() const
  {
    return "GetParameters";
  }
};


class Parameters : public MessageJSON
{
public:
  Parameters();
  Parameters(const std::string& json);
  virtual ~Parameters();

  virtual std::string serialize() const;
  virtual void deserialize(const std::string& json);

  virtual std::unique_ptr<MessageJSON> createReply() const;

  void setParameters(const std::vector<Parameter>& parameters);
  const std::vector<Parameter>& getParameters() const;

  virtual MessageJSON::MessageType getMessageType() const
  {
    return parameters;
  }

  virtual std::string getName() const
  {
    return "Parameters";
  }

private:
  std::vector<Parameter> m_parameters;
};

#endif // MESSAGEJSON_H
