#include "messagejson.h"

#include <vector>
#include <algorithm>

// MessageJSON //
const int MessageJSON::ms_version = 1;

MessageJSON::MessageJSON()
{
}

MessageJSON::~MessageJSON()
{
}

std::unique_ptr<MessageJSON>
MessageJSON::createMessageFromJson(const std::string& json)
{
  MessageType type = MessageJSON::getMessageTypeFromJson(json);

  std::unique_ptr<MessageJSON> message;
  switch (type)
  {
    case parameters:
    {
      message.reset( new Parameters() ) ;
      break;
    }
    case getParameters:
    {
      message.reset( new GetParameters() ) ;
      break;
    }
    case none:
    {
      message = 0;
      break;
    }
  }
  if (message)
  {
    message->deserialize(json);
  }

  std::cout << __PRETTY_FUNCTION__ << " - created type: " << message->getName() << std::endl;

  return message;
}

int
MessageJSON::getVersion()
{
  return ms_version;
}

boost::property_tree::ptree
MessageJSON::getTreeFromJson(const std::string& json)
{
  boost::property_tree::ptree pt;
  if (json.empty())
  {
    return pt;
  }

  try
  {
    std::stringstream ss;
    ss << json;
    boost::property_tree::read_json(ss, pt);
  }
  catch (std::exception& error)
  {

    std::cout << json << std::endl;
    std::cerr << error.what() << std::endl;
  }

  return pt;
}

MessageJSON::MessageType
MessageJSON::getMessageTypeFromJson(const std::string& json)
{
  boost::property_tree::ptree pt = getTreeFromJson(json);

  if (pt.empty())
  {
    return none;
  }

  MessageType type = static_cast<MessageType>( pt.get<int>("message.type") );
  return type;
}

void
MessageJSON::addTypeAndVersion(boost::property_tree::ptree& pt) const
{
  pt.put("message.type", getMessageType());
  pt.put("message.version", getVersion());
  pt.put("message.type_name", getName());
}

bool
MessageJSON::checkMessageVersionAndTypeName(const boost::property_tree::ptree& pt) const
{
  int version = pt.get<int>("message.version");
  std::string typeName = pt.get<std::string>("message.type_name");
  if ( version == getVersion() || typeName == getName() )
  {
    return false;
  }
  return true;
}

void
MessageJSON::print(boost::property_tree::ptree pt)
{
  auto end = pt.end();
  for (auto it = pt.begin(); it != end; ++it)
  {
    std::cout << it->first << ": " << it->second.get_value<std::string>() << std::endl;
    print(it->second);
  }
}


// GetParameters //
Parameters::Parameters() :
  MessageJSON(),
  m_parameters()
{
}

Parameters::~Parameters()
{
}

void
Parameters::setParameters(const std::vector<Parameter>& parameters)
{
  m_parameters = parameters;
}

const std::vector<Parameter>&
Parameters::getParameters() const
{
  return m_parameters;
}

std::string
Parameters::serialize() const
{
  boost::property_tree::ptree pt;
  addTypeAndVersion(pt);

  boost::property_tree::ptree parameters;
  for (auto iter = m_parameters.begin(); iter != m_parameters.end(); ++iter)
  {
    boost::property_tree::ptree parameter;
    parameter.put("id", iter->id);
    parameter.put("name", iter->name);
    parameters.push_back(std::make_pair("", parameter));
  }
  pt.add_child("parameters", parameters);

  std::stringstream ss;
  write_json(ss, pt);
  return ss.str();
}

void
Parameters::deserialize(const std::string& json)
{
  if (json.empty())
  {
    return;
  }

  boost::property_tree::ptree pt = getTreeFromJson(json);
  if (pt.empty())
  {
    return;
  }

  assert( checkMessageVersionAndTypeName(pt) );

  try
  {
    boost::property_tree::ptree parameterTree = pt.get_child("parameters");

    m_parameters.clear();

    for (auto it = parameterTree.begin(); it != parameterTree.end(); ++it)
    {
      auto iter = it->second.begin();
      int id = iter->second.get_value<int>();
      iter++;
      std::string name = iter->second.get_value<std::string>();
      m_parameters.push_back( Parameter(name, id) )  ;
    }
  }
  catch (std::exception const& e)
  {
    std::cerr << e.what() << std::endl;
  }
}

std::unique_ptr<MessageJSON>
Parameters::createReply() const
{
  return 0;
}


// GetParameters //
GetParameters::GetParameters()
{
}

GetParameters::~GetParameters()
{
}

std::string
GetParameters::serialize() const
{
  boost::property_tree::ptree pt;
  addTypeAndVersion(pt);

  std::stringstream ss;
  write_json(ss, pt);
  return ss.str();
}

void
GetParameters::deserialize(const std::string& /*json*/)
{

}

std::unique_ptr<MessageJSON>
GetParameters::createReply() const
{
  std::vector<Parameter> parameters;
  parameters.push_back( Parameter("force", 1) );
  parameters.push_back( Parameter("diameter", 2) );

  Parameters* paramRaw = new Parameters();
  paramRaw->setParameters(parameters);
  return std::unique_ptr<MessageJSON>( paramRaw );
}
