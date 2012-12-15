#ifndef CONNECTIONSTATUS_H
#define CONNECTIONSTATUS_H

//#include <mutex>

class ConnectionStatus
{
public:
  enum EnumConnectionStatus
  {
    unavailable,
    connected,
    listening
  };

public:
  ConnectionStatus();
  ~ConnectionStatus();

  void addTotalDataDown(unsigned int nBytes);
  void addTotalDataUp(unsigned int nBytes);
  void setStatus(EnumConnectionStatus status);

public:
  unsigned int m_id;
  EnumConnectionStatus generalStatus;
  unsigned int totalDataDown_byte;
  unsigned int totalDataUp_byte;
};

#endif // CONNECTIONSTATUS_H
