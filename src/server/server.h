#ifndef SERVER_H
#define SERVER_H

#include "connectionstatus.h"
#include "shared/clientserverdata.h"

#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/asio.hpp>

#include <memory>
#include <mutex>
#include <thread>

class ServerObserver;

class Server : boost::noncopyable
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

  /**
   * @brief Register an observer
   */
  void registerObserver(ServerObserver* observer);

  /**
   * @brief Unregister an observer
   */
  void unregisterObserver(ServerObserver* observer);

  /**
   * @brief Notifies the observers with the incoming data
   * @param dataStrings the incoming data
   * @param id the connection ID
   */
  void notifyObservers(std::vector<std::string> dataStrings, ConnectionId id);

  /**
   * @brief Returns the number of connection threads
   */
  std::size_t getNThreads() const;

  /**
   * @brief Returns a vector of open socket ids
   */
  std::vector< ConnectionId > getOpenSocketIds() const;

  /**
   * @brief Returns the number of open sockets
   */
  unsigned int getNOpenSockets() const;

  /**
   * @brief Returns a vector of ids of open threads
   */
  std::vector<Server::ConnectionId> getOpenThreadIds() const;

  std::map<ConnectionId, ConnectionStatus> getConnectionStatuses();

private:
  /**
   * @brief Starts serving
   *
   * Opens a new socket connection and waits for a client to connect.
   * When a client connects, startServerThread is called which opens a new
   * socket and waits for a client to connect on a new thread.
   *
   * @param id the connection ID
   */
  void startServing(ConnectionId id);

  /**
   * @brief Opens a new socket connection and adds it to the map of sockets.
   * @param id the connection ID
   */
  int openConnection(ConnectionId id);

  /**
   * @brief Closes the socket connection with given connection id
   *
   * A shutdown message is sent to the client and the socket closed.
   * The socket and corresponding thread are erased from the map.
   *
   * @param id the connection ID
   */
  void closeConnection(ConnectionId id);

  /**
   * @brief Returns a socket connection
   * @param id the connection ID
   * @return raw pointer to the socket with the given id
   */
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
   * @brief Returns the status of the connection with the given ID
   * @param id the connection ID
   * @return the status of the connection with the given connection ID
   */
  ConnectionStatus::EnumConnectionStatus getConnectionStatus(ConnectionId id) const;

  void updateConnectionStatuses();

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

  std::map<ConnectionId, ConnectionStatus> m_connectionStatuses;

  /** server observers that are notified when a new message comes in from a client */
  std::vector<ServerObserver*> m_observers;

  /** Connection id counter, increases with every new connection thread, never decreases */
  ConnectionId m_nIdCounter;

  mutable std::mutex m_mutex;
};

#endif // SERVER_H
