#ifndef SIMULATIONSERVER_H
#define SIMULATIONSERVER_H

#include "server/server.h"
#include "server/serverobserver.h"

class MessageJSON;

class SimulationServer : public ServerObserver
{
public:
  explicit SimulationServer(Server* server);
  virtual ~SimulationServer();

  void startServer();

protected:
  virtual Message createReply(const Message& message);
};

#endif // SIMULATIONSERVER_H
