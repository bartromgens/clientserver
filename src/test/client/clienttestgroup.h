#ifndef CLIENTTESTGROUP_H
#define CLIENTTESTGROUP_H

#include <thread>

class ClientTestGroup
{
public:
  ClientTestGroup();
  ~ClientTestGroup();
  void startClientThread();
  bool startClient(int id);

private:
};

#endif // CLIENTTESTGROUP_H
