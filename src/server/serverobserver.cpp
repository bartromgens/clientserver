#include "serverobserver.h"

#include "server/server.h"


ServerObserver::ServerObserver(Server* server)
{
  server->registerObserver(this);
}


ServerObserver::~ServerObserver()
{
}
