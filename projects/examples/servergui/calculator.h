#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "server/server.h"
#include "server/serverobserver.h"

class Calculator : public ServerObserver
{
public:
  explicit Calculator(Server* server);
  virtual ~Calculator();

  virtual void notifyReceivedData(std::vector<std::string> dataStrings, int connectionId);

private:
  Server* m_server;
};

#endif // CALCULATOR_H
