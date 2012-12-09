#ifndef SERVEROBSERVER_H
#define SERVEROBSERVER_H

#include <string>
#include <vector>

class ServerObserver
{
public:
  ServerObserver();
  ~ServerObserver();

  virtual void notifyReceivedData(std::vector<std::string> dataStrings, int id) = 0;
};

#endif // SERVEROBSERVER_H
