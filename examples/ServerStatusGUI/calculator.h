#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "server/server.h"
#include "server/serverobserver.h"

class Calculator : public ServerObserver
{
public:
  Calculator(Server* server);
  ~Calculator();

  virtual void notifyReceivedData(std::vector<std::string> dataStrings, int connectionId);

private:
  Server* m_server;
};

#endif // CALCULATOR_H
