#ifndef SERVEROBSERVER_H
#define SERVEROBSERVER_H

#include "shared/message.h"

#include <string>
#include <vector>

class Server;

/**
 * Observer for Servers, will be notified of new incoming data from clients.
 */
class ServerObserver
{
public:
  /**
   * Constructor.
   */
  explicit ServerObserver(Server* server);

  /**
   * Destructor
   */
  virtual ~ServerObserver();

  Server* getServer();

  /**
   * createReply
   * @param dataStrings the list of strings that are send by a client with the connectionId
   * @param connectionId the connection ID of the connection that sends the message
   */
  virtual Message createReply(const Message& message) = 0;

private:
  Server* m_server;
};

#endif // SERVEROBSERVER_H
