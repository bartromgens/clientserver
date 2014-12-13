#include "serverobserver.h"

#include "server/server.h"


ServerObserver::ServerObserver(Server* server)
  : m_server(server)
{
  assert(m_server);
  m_server->setObserver(this);
}


ServerObserver::~ServerObserver()
{
  m_server->removeObserver(this);
}

Server*
ServerObserver::getServer()
{
  return m_server;
}
