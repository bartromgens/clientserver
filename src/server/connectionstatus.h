#ifndef CONNECTIONSTATUS_H
#define CONNECTIONSTATUS_H

class ConnectionStatus
{
public:
  enum EnumConnectionStatus
  {
    unavailable,
    connected,
    listening
  };

  ConnectionStatus();
  ~ConnectionStatus();

public:
  double totalDataDown_byte;
  double totalDataUp_byte;
  EnumConnectionStatus generalStatus;
};

#endif // CONNECTIONSTATUS_H
