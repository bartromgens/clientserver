#include "connectionstatus.h"

ConnectionStatus::ConnectionStatus()
  : m_id(-1),
    generalStatus(),
    totalDataDown_byte(0),
    totalDataUp_byte(0)
{
}


ConnectionStatus::~ConnectionStatus()
{
}


void
ConnectionStatus::addTotalDataDown(unsigned int nBytes)
{
  totalDataDown_byte += nBytes;
}


void
ConnectionStatus::addTotalDataUp(unsigned int nBytes)
{
  totalDataUp_byte += nBytes;
}


void
ConnectionStatus::setStatus(ConnectionStatus::EnumConnectionStatus status)
{
  generalStatus = status;
}
