#include "dummyapplication.h"

#include <thread>

DummyApplication::DummyApplication()
  : m_server(new Server())
{
  std::cout << "DummyApplication::DummyApplication()" << std::endl;
  m_server->setApplication(this);
}


DummyApplication::~DummyApplication()
{
  std::cout << "DummyApplication::~DummyApplication()" << std::endl;
}


void
DummyApplication::run()
{
  std::cout << "DummyApplication:run()" << std::endl;
  startServing();
//  std::thread t1(&DummyApplication::startServing, this);
//  t1.detach();
//  std::thread t2(&DummyApplication::startServing2, this);
//  t2.detach();
}


void
DummyApplication::startServing()
{ 
  std::cout << "DummyApplication:startServing()" << std::endl;
  m_server->startServing();
}


void
DummyApplication::processIncomingData(std::vector<std::string> dataStrings)
{
  std::string command = dataStrings[0];

  // incoming server commands are processed and delegated
  if (command == "add")
  {
    int a = atoi(dataStrings[1].c_str());
    int b = atoi(dataStrings[2].c_str());
    int sum = a + b;
    std::string message = std::to_string(sum);
    message += "\0";

    write(message);
  }
  else
  {
    std::string message = "server: nothing to do!";
    message += "\0";

    write(message);
  }
}


void
DummyApplication::write(const std::vector<std::string>& dataStrings)
{
  m_server->write(dataStrings);
}


void
DummyApplication::write(const std::string& dataString)
{
  m_server->write(dataString);
}

