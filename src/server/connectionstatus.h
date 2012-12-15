#ifndef CONNECTIONSTATUS_H
#define CONNECTIONSTATUS_H

class ConnectionStatus
{
public:
  ConnectionStatus();
  ~ConnectionStatus();

public:
  double totalDataDown_byte;
  double totalDataUp_byte;
};

#endif // CONNECTIONSTATUS_H
