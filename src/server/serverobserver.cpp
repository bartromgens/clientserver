#include "serverobserver.h"

#include "server/server.h"


ServerObserver::ServerObserver(Server* server)
  : m_server(server)
{
  assert(m_server);
  m_server->registerObserver(this);
}


ServerObserver::~ServerObserver()
{
  m_server->unregisterObserver(this);
}
