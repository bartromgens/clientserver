#ifndef SIMULATIONSERVER_H
#define SIMULATIONSERVER_H

#include "server/server.h"
#include "server/serverobserver.h"

class Message;

class SimulationServer : public ServerObserver
{
public:
  explicit SimulationServer(Server* server);
  virtual ~SimulationServer();

protected:
  virtual void notifyReceivedData(std::vector<std::string> dataStrings, int connectionId);

private:
  void sendMessage(Message* message, int connectionId);

private:
  Server* m_server;
};

#endif // SIMULATIONSERVER_H
