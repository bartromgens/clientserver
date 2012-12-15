#ifndef SERVEROBSERVER_H
#define SERVEROBSERVER_H

#include <string>
#include <vector>

/**
 * @brief Observer for Servers, will be notified of new incoming data from clients.
 */
class ServerObserver
{
public:
  /**
   * Constructor.
   */
  ServerObserver();

  /**
   * Destructor
   */
  ~ServerObserver();

  /**
   * @brief notifyReceivedData
   * @param dataStrings the list of strings that are send by a client with the connectionId
   * @param connectionId the connection ID of the connection that sends the message
   */
  virtual void notifyReceivedData(std::vector<std::string> dataStrings, int id) = 0;
};

#endif // SERVEROBSERVER_H
