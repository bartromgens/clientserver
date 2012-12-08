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
  std::thread t1(&DummyApplication::startServing, this);
  std::thread t2(&DummyApplication::startServing, this);
  std::thread t3(&DummyApplication::startServing, this);
  std::thread t4(&DummyApplication::startServing, this);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
}


void
DummyApplication::startServing()
{ 
  std::cout << "DummyApplication:startServing()" << std::endl;
  m_server->startServing();
}


void
DummyApplication::processIncomingData(std::vector<std::string> dataStrings, int id)
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

    write(message, id);
  }
  else
  {
    std::string message = "server: nothing to do!";
    message += "\0";

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

