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

protected:
  virtual void notifyReceivedData(const Message& message, int connectionId);

private:
  void sendMessage(MessageJSON* message, int connectionId);

private:
  Server* m_server;
};

#endif // SIMULATIONSERVER_H
