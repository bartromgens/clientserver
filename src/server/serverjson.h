#ifndef SERVERJSON_H
#define SERVERJSON_H

#include "server/server.h"
#include "server/serverobserver.h"

class MessageJSON;

class ServerJSON : public ServerObserver
{
public:
  explicit ServerJSON(Server* server);
  virtual ~ServerJSON();

  void startServer();

protected:
  virtual Message createReply(const Message& message);
};

#endif // SERVERJSON_H
