#ifndef DUMMYAPPLICATION_H
#define DUMMYAPPLICATION_H

#include "server.h"
#include "serverobserver.h"

class DummyApplication : public ServerObserver
{
public:
  DummyApplication();
  ~DummyApplication();

  void run();

  void write(const std::vector<std::string>& dataStrings, int id);
  void write(const std::string& dataString, int id);

  virtual void notifyReceivedData(std::vector<std::string> dataStrings, int id);

private:

  void startAccepting();

private:
  std::unique_ptr<Server> m_server;
};

#endif // DUMMYAPPLICATION_H
