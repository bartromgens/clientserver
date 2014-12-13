#ifndef CLIENTJSON_H
#define CLIENTJSON_H

#include "client/client.h"
#include "shared/messagejson.h"

class ClientJSON
{
public:
  explicit ClientJSON(const std::string& ip = ClientServerData::defaultIp,
                      unsigned short port = ClientServerData::defaultPort);
  virtual ~ClientJSON();

  bool connect();
  bool disconnect();
  bool isConnected();

  std::unique_ptr<MessageJSON> sendMessage(MessageJSON* message);

  std::vector<ParameterData> getParameters();
private:
  Client m_client;
};

#endif // CLIENTJSON_H
