#ifndef SERVER_H
#define SERVER_H

#include "shared/clientserverdata.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <mutex>
#include <thread>

class ServerObserver;

class Server
{
public:

  typedef unsigned int ConnectionId;

  /**
   * Constructor
   */
  Server(unsigned short port = ClientServerData::defaultPort);

  /**
   * Destructor
   */
  ~Server();

  /**
   * Constructor
   *
   * @param port the port number to server on
   */
  void setPort(int port);

  /**
   * @brief Starts a new server thread that waits for a new connection
   */
  void startServerThread();

  /**
   * @brief Closes all connections and stops serving
   */
  void stopServer();

  /**
   * @brief writes a string message to the socket with the given id
   * @param message the message to send
   * @param id the connection ID
   * @throws boost::system::system_error socket write error
   */
  void write(const std::string& message, ConnectionId id);

  /**
   * @brief writes a vector of strings to the socket with the given id
   * @param messageStrings the vector of strings to send to the server
   * @param id the connection ID
   * @param separationChar the character used to separate strings in the vector when sending it to the client
   * @throws boost::system::system_error socket write error
   */
  void write(const std::vector<std::string>& messageStrings,
             ConnectionId id,
             std::string separationChar = ClientServerData::separationCharacter);

  void unregisterObserver(ServerObserver* observer);
  void registerObserver(ServerObserver* observer);
  void notifyObservers(std::vector<std::string> dataStrings, ConnectionId id);

private:
  /**
   * Starts serving
   */
  void startServing(ConnectionId id);

  int openConnection(ConnectionId id);
  void closeConnection(ConnectionId id);
  boost::asio::ip::tcp::socket* getSocket(ConnectionId id) const;

  /**
   * @brief Converts a character array to a vector of strings
   * @param charArray the array
   * @param len the size of the actual array
   * @return a vector of strings
   */
  std::vector<std::string> convertCharArrayToStringVector(const std::array<char, ClientServerData::defaultBufferSize>& charArray,
                                                          size_t len,
                                                          std::string separationChar = ClientServerData::separationCharacter) const;


  /**
   * @brief Returns a vector of open socket ids
   */
  std::vector< ConnectionId > getOpenSocketIds() const;

  int getNOpenSockets() const;

  int getNThreads() const;

private:
  /** Server network port number */
  unsigned short m_port;

  /** provides the core I/O functionality for users of the synchronous I/O objects, including the tcp::socket and tcp::acceptor. */
  std::unique_ptr<boost::asio::io_service> m_io_service;
  /** provides the ability to accept new socket connections */
  std::unique_ptr<boost::asio::ip::tcp::acceptor> m_acceptor;

  /** map of connection threads, key is the connection ID */
  std::map<ConnectionId, std::unique_ptr<std::thread> > m_threads;
  /** map of sockets that provides blocking stream-oriented socket functionality */
  std::map<ConnectionId, std::unique_ptr<boost::asio::ip::tcp::socket> > m_sockets;

  /** server observers that are notified when a new message comes in from a client */
  std::vector<ServerObserver*> m_observers;

  /** Connection id counter, increases with every new connection thread, never decreases */
  ConnectionId m_nIdCounter;

  mutable std::mutex m_mutex;
};

#endif // SERVER_H
