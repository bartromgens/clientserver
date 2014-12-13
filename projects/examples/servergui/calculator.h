#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "server/server.h"
#include "server/serverobserver.h"
#include "shared/message.h"

class Calculator : public ServerObserver
{
public:
  explicit Calculator(Server* server);
  virtual ~Calculator();

  virtual Message createReply(const Message &message);

private:
  Server* m_server;
};

#endif // CALCULATOR_H
