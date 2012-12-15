#include "dummyapplication.h"


#include <thread>

DummyApplication::DummyApplication()
  : ServerObserver(),
    m_server(new Server())
{
  m_server->registerObserver(this);
}


DummyApplication::~DummyApplication()
{
  std::cout << "DummyApplication::~DummyApplication()" << std::endl;
}


void
DummyApplication::run()
{
  std::cout << "DummyApplication:run()" << std::endl;
  m_server->startServerThread();
  std::cout << "DummyApplication:run() - end!" << std::endl;
}


void
DummyApplication::startServing()
{ 
  std::cout << "DummyApplication:startServing()" << std::endl;
//  m_server->startServing();
}


void
DummyApplication::notifyReceivedData(std::vector<std::string> dataStrings, int id)
{
  std::string command = dataStrings[0];

  // incoming server commands are processed and delegated
  if (command == "add")
  {
    int a = atoi(dataStrings[1].c_str());
    int b = atoi(dataStrings[2].c_str());
    int sum = a + b;
    std::string message = std::to_string(sum);
    write(message, id);
  }
  else
  {
    std::string message = "server: nothing to do!";

    write(message, id);
  }
}


void
DummyApplication::write(const std::vector<std::string>& dataStrings, int id)
{
  m_server->write(dataStrings, id);
}


void
DummyApplication::write(const std::string& dataString, int id)
{
  m_server->write(dataString, id);
}

