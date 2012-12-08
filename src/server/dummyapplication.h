#ifndef DUMMYAPPLICATION_H
#define DUMMYAPPLICATION_H

#include "src/server/server.h"

class DummyApplication
{
public:
  DummyApplication();
  ~DummyApplication();

  void run();


  void processIncomingData(std::vector<std::string> dataStrings);
  void write(const std::vector<std::string>& dataStrings);
  void write(const std::string& dataString);
  void startServing2();
private:

  void startServing();

private:
  std::unique_ptr<Server> m_server;
};

#endif // DUMMYAPPLICATION_H
